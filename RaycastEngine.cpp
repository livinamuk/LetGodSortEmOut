#include "RaycastEngine.h"

GLuint RaycastEngine::s_mainFrameBuffer;
bool RaycastEngine::s_render;

void RaycastEngine::Init()
{
	Renderer::Init();
	WorldMap::LoadMap();
	Scene::LoadScene("res/scene.txt");
	Renderer::s_renderMode = RENDER_MODE_FUSION_WORLD_PLUS_LIGHTING;
}

void RaycastEngine::RenderLoop()
{
	if (!s_render)
		return;

	WorldMap::Update();
	Camera2D::AdjustProjection();
	Renderer::RenderFrame();
}

void RaycastEngine::SetCell(int x, int y, int value)
{
	Tile tile;

	if (value == 0)
		tile = Tile::DIRT;
	else if (value == 1)
		tile = Tile::WALL;

	WorldMap::s_map[x][y].tile = tile;

	
}