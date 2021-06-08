#include "WorldMap.h"
#include "Core/Input.h"
#include "Renderer/Renderer.h"

Cell WorldMap::s_map[MAP_WIDTH][MAP_HEIGHT];
std::vector<Edge> WorldMap::s_edges;
std::vector<glm::vec2> WorldMap::s_uniqueEdgePoints;
std::vector<std::tuple<float, float, float>> WorldMap::s_visibilityPolygonPoints;

std::vector<glm::vec2> WorldMap::s_newPoints;

int WorldMap::s_rayCount;

void WorldMap::Update()
{
	s_rayCount = 0;

	// Draw tiles
	if (Input::s_leftMouseDown && Renderer::s_editorMode)
		WorldMap::SetCell(Input::s_gridX, Input::s_gridY, Tile::WALL);

	if (Input::s_rightMouseDown && Renderer::s_editorMode)
		WorldMap::SetCell(Input::s_gridX, Input::s_gridY, Tile::DIRT);

	if (Input::s_keyPressed[HELL_KEY_N])
		NewMap();




}

void WorldMap::NewMap()
{
	// Initialise map to dirt
	for (int x = 0; x < MAP_WIDTH; x++)
		for (int y = 0; y < MAP_HEIGHT; y++)
			s_map[x][y].tile = Tile::DIRT;
}

void WorldMap::LoadMap()
{
	std::string line;
	std::ifstream MyFile("res/WorldMap.txt");
	int y = 0;

	while (std::getline(MyFile, line)) {
		for (int x = 0; x < line.length(); x++) {

			int num;
			std::stringstream ss;
			ss << line[x];
			ss >> num;

			if (num == 0)
				s_map[x][y].tile = Tile::DIRT;
			else
				s_map[x][y].tile = Tile::WALL;

			std::cout << num;
		}
		std::cout << "\n";
		y++;
	}
	MyFile.close();
}

void WorldMap::SaveMap()
{
	std::ofstream MyFile("res/WorldMap.txt");

	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
			MyFile << s_map[x][y].tile;
		MyFile << "\n";
	}
	MyFile.close();

}

void WorldMap::SaveInt(rapidjson::Value* object, std::string elementName, int number, rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value name(elementName.c_str(), allocator);
	rapidjson::Value value(rapidjson::kObjectType);
	value.SetInt(number);
	object->AddMember(name, value, allocator);
}


bool WorldMap::IsCellWithinAABB(int x, int y, AABB* aabb)
{
	// this actually returns if the cell is outside the AABB 

	int cellThreshold = 5; // OPTIMIZATION: you could consider setting this to 0 and increasing the size of the width and height of light somewhere...

	if (x < (aabb->lowerX / GRID_SIZE) - cellThreshold)
		return true;

	if (x > (aabb->upperX / GRID_SIZE) + cellThreshold)
		return true;

	if (y < (aabb->lowerY / GRID_SIZE) - cellThreshold)
		return true;

	if (y > (aabb->upperY / GRID_SIZE) + cellThreshold)
		return true;

	// what is this even doing??
	return s_map[x][y].IsObstacle();
}

bool WorldMap::IsObstacleOrOutOfMapBounds(int x, int y)
{
	if (x < 0 || y < 0 << x >= MAP_WIDTH || y >= MAP_HEIGHT || s_map[x][y].IsObstacle())
		return true;
	else
		return false;
}

bool WorldMap::IsPixelPositionInMapBounds(int x, int y)
{
	if (x < 0 || y < 0 || x > MAP_WIDTH * GRID_SIZE || y > MAP_HEIGHT * GRID_SIZE)
		return false;
	else
		return true;
}

void WorldMap::SetCell(int x, int y, Tile tile)
{
	if (x < 0 || y < 0 || x > MAP_WIDTH || y > MAP_HEIGHT)
		return;
	s_map[x][y].tile = tile;

	Scene::UpdateAllLights();
}

void WorldMap::BuildEdgeMapFromWorldMap(AABB* aabb, int inset)
{
	// Clear "PolyMap"
	s_edges.clear();

	for (int x = 0; x < MAP_WIDTH; x++)
		for (int y = 0; y < MAP_HEIGHT; y++)
			for (int j = 0; j < 4; j++)
			{
				s_map[x][y].edge_exists[j] = false;
				s_map[x][y].edge_id[j] = -1;
			}

	// container to store found obstacles within AABB
	std::vector<Coord2D> wallsWithinRange;

	// Iterate through region from top left to bottom right
	for (int x = 0; x < MAP_WIDTH; x++)
	{
		for (int y = 0; y < MAP_HEIGHT; y++)
		{
			Cell* cell = &s_map[x][y];
			Cell* n = GetCellPointer(x, y - 1);
			Cell* s = GetCellPointer(x, y + 1);
			Cell* w = GetCellPointer(x - 1, y);
			Cell* e = GetCellPointer(x + 1, y);

			// If this cell exists, check if it needs edges
			//if (cell->IsObstacle())
			if (cell->IsObstacle() && IsCellWithinAABB(x, y, aabb))
			{
				// store it
				wallsWithinRange.push_back(Coord2D(x, y));

				// If this cell has no western neighbour, it needs a western edge
				if (w && !w->IsObstacle() && !IsCellWithinAABB(x - 1, y, aabb))
				{
					// It can either extend it from its northern neighbour if they have
					// one, or It can start a new one.
					if (n && n->edge_exists[WEST])
					{
						// Northern neighbour has a western edge, so grow it downwards
						s_edges[n->edge_id[WEST]].eY += GRID_SIZE;
						cell->edge_id[WEST] = n->edge_id[WEST];
						cell->edge_exists[WEST] = true;
					}
					else
					{
						// Northern neighbour does not have one, so create one
						Edge edge;
						edge.sX = x * GRID_SIZE + inset;
						edge.sY = y * GRID_SIZE + inset;
						edge.eX = x * GRID_SIZE + inset;
						edge.eY = (y + 1) * GRID_SIZE - inset;
						// Add edge to Polygon Pool
						int edge_id = s_edges.size();
						s_edges.push_back(edge);
						// Update tile information with edge information
						cell->edge_id[WEST] = edge_id;
						cell->edge_exists[WEST] = true;
					}
				}

				// If this cell dont have an eastern neignbour, It needs a eastern edge
				if (e && !e->IsObstacle() && !IsCellWithinAABB(x + 1, y, aabb))
				{
					// It can either extend it from its northern neighbour if they have
					// one, or It can start a new one.
					if (n && n->edge_exists[EAST])
					{
						// Northern neighbour has one, so grow it downwards
						s_edges[n->edge_id[EAST]].eY += GRID_SIZE;
						cell->edge_id[EAST] = n->edge_id[EAST];
						cell->edge_exists[EAST] = true;
					}
					else
					{
						// Northern neighbour does not have one, so create one
						Edge edge;
						edge.sX = (x + 1) * GRID_SIZE - inset;
						edge.sY = y * GRID_SIZE + inset;
						edge.eX = (x + 1) * GRID_SIZE - inset;
						edge.eY = (y + 1) * GRID_SIZE - inset;
						// Add edge to Polygon Pool
						int edge_id = s_edges.size();
						s_edges.push_back(edge);
						// Update tile information with edge information
						cell->edge_id[EAST] = edge_id;
						cell->edge_exists[EAST] = true;
					}
				}
				// If this cell doesnt have a northern neignbour, It needs a northern edge
				if (n && !n->IsObstacle() && !IsCellWithinAABB(x, y - 1, aabb))
				{
					// It can either extend it from its western neighbour if they have
					// one, or It can start a new one.
					if (w && w->edge_exists[NORTH])
					{
						// Western neighbour has one, so grow it eastwards
						s_edges[w->edge_id[NORTH]].eX += GRID_SIZE;
						cell->edge_id[NORTH] = w->edge_id[NORTH];
						cell->edge_exists[NORTH] = true;
					}
					else
					{
						// Western neighbour does not have one, so create one
						Edge edge;
						edge.sX = (x)*GRID_SIZE + inset;
						edge.sY = (y)*GRID_SIZE + inset;
						edge.eX = (x + 1) * GRID_SIZE - inset;
						edge.eY = (y)*GRID_SIZE + inset;
						// Add edge to Polygon Pool
						int edge_id = s_edges.size();
						s_edges.push_back(edge);
						// Update tile information with edge information
						cell->edge_id[NORTH] = edge_id;
						cell->edge_exists[NORTH] = true;
					}
				}
				// If this cell doesnt have a southern neignbour, It needs a southern edge
				if (s && !s->IsObstacle() && !IsCellWithinAABB(x, y + 1, aabb))
				{
					// It can either extend it from its western neighbour if they have
					// one, or It can start a new one.
					if (w && w->edge_exists[SOUTH])
					{
						// Western neighbour has one, so grow it eastwards
						s_edges[w->edge_id[SOUTH]].eX += GRID_SIZE;
						cell->edge_id[SOUTH] = w->edge_id[SOUTH];
						cell->edge_exists[SOUTH] = true;
					}
					else
					{
						// Western neighbour does not have one, so I need to create one
						Edge edge;
						edge.sX = (x)*GRID_SIZE + inset;
						edge.sY = (y + 1) * GRID_SIZE - inset;
						edge.eX = (x + 1) * GRID_SIZE - inset;
						edge.eY = (y + 1) * GRID_SIZE - inset;
						// Add edge to Polygon Pool
						int edge_id = s_edges.size();
						s_edges.push_back(edge);
						// Update tile information with edge information
						cell->edge_id[SOUTH] = edge_id;
						cell->edge_exists[SOUTH] = true;
					}
				}
			}
		}
	}

	for (Coord2D& wall : wallsWithinRange)
	{
		int x = wall.x;
		int y = wall.y;
		Cell* cell = &s_map[x][y];
		Cell* n = GetCellPointer(x, y - 1);
		Cell* s = GetCellPointer(x, y + 1);
		Cell* w = GetCellPointer(x - 1, y);
		Cell* e = GetCellPointer(x + 1, y);
		Cell* sw = GetCellPointer(x - 1, y + 1);
		Cell* se = GetCellPointer(x + 1, y + 1);
		Cell* ne = GetCellPointer(x + 1, y - 1);
		Cell* nw = GetCellPointer(x - 1, y - 1);

		// South west
		if (w && w->IsObstacle() && s && s->IsObstacle() && sw && !sw->IsObstacle()) {
			if (s->edge_id[WEST] != -1)
				s_edges[s->edge_id[WEST]].sY -= inset * 2;
			if (w->edge_id[SOUTH] != -1)
				s_edges[w->edge_id[SOUTH]].eX += inset * 2;
		}

		// South east
		if (e && e->IsObstacle() && s && s->IsObstacle() && se && !se->IsObstacle()) {
			if (s->edge_id[EAST] != -1)
				s_edges[s->edge_id[EAST]].sY -= inset * 2;
			if (e->edge_id[SOUTH] != -1)
				s_edges[e->edge_id[SOUTH]].sX -= inset * 2;
		}

		// North east
		if (e && e->IsObstacle() && n && n->IsObstacle() && ne && !ne->IsObstacle()) {
			if (n->edge_id[EAST] != -1)
				s_edges[n->edge_id[EAST]].eY += inset * 2;
			if (e->edge_id[NORTH] != -1)
				s_edges[e->edge_id[NORTH]].sX -= inset * 2;
		}
		// North west
		if (w && w->IsObstacle() && n && n->IsObstacle() && nw && !nw->IsObstacle()) {
			if (n->edge_id[WEST] != -1)
				s_edges[n->edge_id[WEST]].eY += inset * 2;
			if (w->edge_id[NORTH] != -1)
				s_edges[w->edge_id[NORTH]].eX += inset * 2;
		}
	}

	FindUniqueEdgePoints();
}

void WorldMap::FindUniqueEdgePoints()
{
	s_uniqueEdgePoints.clear();
	for (auto& e : s_edges)
	{
		glm::vec2 startPoint(e.sX, e.sY);
		glm::vec2 endPoint(e.eX, e.eY);

		if (std::find(s_uniqueEdgePoints.begin(), s_uniqueEdgePoints.end(), startPoint) == s_uniqueEdgePoints.end())
			s_uniqueEdgePoints.push_back(startPoint);

		if (std::find(s_uniqueEdgePoints.begin(), s_uniqueEdgePoints.end(), endPoint) == s_uniqueEdgePoints.end())
			s_uniqueEdgePoints.push_back(endPoint);
	}
}

std::vector<std::tuple<float, float, float>> WorldMap::CalculateVisibilityPolygon(float ox, float oy, float radius)
{
	s_newPoints.clear();

	// Get rid of existing polygon
	std::vector<std::tuple<float, float, float>> visibilityPolygonPoints;

	for (auto& point : s_uniqueEdgePoints)
	{
		// Take the start point, then the end point (we could use a pool of
		// non-duplicated points here, it would be more optimal)
		//for (int i = 0; i < 2; i++)
		{
			float rdx, rdy;
			//rdx = (i == 0 ? e1.sX : e1.eX) - ox;
			//rdy = (i == 0 ? e1.sY : e1.eY) - oy;

			rdx = point.x - ox;
			rdy = point.y - oy;

			float base_ang = atan2f(rdy, rdx);

			float ang = 0;
			// For each point, cast 3 rays, 1 directly at point
			// and 1 a little bit either side
			for (int j = 0; j < 3; j++)
			{
				if (j == 1) continue;

				if (j == 0)	ang = base_ang - 0.0001f;
				if (j == 1)	ang = base_ang;
				if (j == 2)	ang = base_ang + 0.0001f;

				// Create ray along angle for required distance
				rdx = radius * cosf(ang);
				rdy = radius * sinf(ang);

				float min_t1 = INFINITY;
				float min_px = 0, min_py = 0, min_ang = 0;
				bool bValid = false;

				// Check for ray intersection with all edges
				for (auto& e2 : s_edges)
				{
					s_rayCount++;

					// Create line segment vector
					float sdx = e2.eX - e2.sX;
					float sdy = e2.eY - e2.sY;

					if (fabs(sdx - rdx) > 0.0f && fabs(sdy - rdy) > 0.0f)
					{
						// t2 is normalised distance from line segment start to line segment end of intersect point
						float t2 = (rdx * (e2.sY - oy) + (rdy * (ox - e2.sX))) / (sdx * rdy - sdy * rdx);
						// t1 is normalised distance from source along ray to ray length of intersect point
						float t1 = (e2.sX + sdx * t2 - ox) / rdx;

						// If intersect point exists along ray, and along line 
						// segment then intersect point is valid
						if (t1 > 0 && t2 >= 0 && t2 <= 1.0f)
						{
							// Check if this intersect point is closest to source. If
							// it is, then store this point and reject others
							if (t1 < min_t1)
							{
								min_t1 = t1;
								min_px = ox + rdx * t1;
								min_py = oy + rdy * t1;
								min_ang = atan2f(min_py - oy, min_px - ox);
								bValid = true;
							}
						}
					}
				}

				if (bValid)// Add intersection point to visibility polygon perimeter
				{
					//float ox, float oy

				/*	glm::vec2 difference = glm::vec2(ox, oy) - glm::vec2(min_px, min_py);
					difference = glm::normalize(difference);

					float scale = 15;
					min_px -= difference.x * scale;
					min_py -= difference.y * scale;*/


					visibilityPolygonPoints.push_back({ min_ang, min_px, min_py });
				}
			}
		}
	}

	// Sort perimeter points by angle from source. This will allow
	// us to draw a triangle fan.
	std::sort(
		visibilityPolygonPoints.begin(),
		visibilityPolygonPoints.end(),
		[&](const std::tuple<float, float, float>& t1, const std::tuple<float, float, float>& t2)
	{
		return std::get<0>(t1) < std::get<0>(t2);
	});

	return visibilityPolygonPoints;
}

Cell* WorldMap::GetCellPointer(int x, int y)
{
	if (x < 0 || y < 0 || x >= MAP_WIDTH || y >= MAP_HEIGHT)
		return nullptr;
	else
		return &s_map[x][y];
}
