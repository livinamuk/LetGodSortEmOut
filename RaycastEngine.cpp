#include "RaycastEngine.h"

GLuint RaycastEngine::s_mainFrameBuffer;
bool RaycastEngine::s_render;

void RaycastEngine::Init()
{
	AssetManager::FindAllFiles("res/textures/");
	Renderer::Init();
	WorldMap::LoadMap("res/WorldMap.txt");
	Scene::LoadScene("res/scene.txt");
	Renderer::s_renderMode = RENDER_MODE_FUSION;
}

void RaycastEngine::RenderLoop()
{
	if (!s_render)
		return;

	WorldMap::Update();
	Camera2D::AdjustProjection();
	Renderer::RenderFrame();

	Scene::ResetShadowCastingObjectModifiedFlags();
}

void RaycastEngine::SetCell(int x, int y, int value)
{
	Tile tile;

	if (value == 0)
		tile = Tile::DIRT;
	else if (value == 1)
		tile = Tile::WALL;

	WorldMap::SetCell(x, y, tile);	
}