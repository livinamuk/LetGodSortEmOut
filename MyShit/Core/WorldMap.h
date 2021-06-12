#pragma once
#include "Header.h"
#include "Helpers/Util.h"

class WorldMap
{
public:
	static void Update();
	static void NewMap();
	static void LoadMap(const char* filepath);
	static void SaveMap(const char* filepath);
	static void SaveInt(rapidjson::Value* object, std::string elementName, int number, rapidjson::Document::AllocatorType& allocator);
	static void BuildEdgeMapFromWorldMap(AABB* aabb, int inset);
	static void FindUniqueEdgePoints();
	static Cell* GetCellPointer(int x, int y);
	static std::vector<std::tuple<float, float, float>> CalculateVisibilityPolygon(float ox, float oy, float radius);

	static Cell s_map[MAP_WIDTH][MAP_HEIGHT];
	static std::vector<Edge> s_edges; 
	static std::vector<glm::vec2> s_uniqueEdgePoints;
	static std::vector<std::tuple<float, float, float>> s_visibilityPolygonPoints;
	static int s_rayCount;
	static std::vector<glm::vec2> s_newPoints;

	static bool IsCellOusideAABB(int x, int y, AABB* aabb);
	static bool IsObstacleOrOutOfMapBounds(int x, int y);
	static bool IsPixelPositionInMapBounds(int x, int y);
	static void SetCell(int x, int y, Tile tile);
	static bool IsTileObstacle(Tile tile);
};

