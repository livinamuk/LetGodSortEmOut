
#include "Common.h"
#include "RaycastEngine.h"

int Extension::GetWidth() const
{
	return surface.GetWidth();
}

int Extension::GetHeight() const
{
	return surface.GetHeight();
}

int Extension::GetTileValue(int x, int y)
{
	Tile tile = WorldMap::s_map[x][y].tile;
	
	if (tile == Tile::DIRT)
		return 0;
	else
		return 1;
}

int Extension::GetGridSize()
{
	return GRID_SIZE;
}

int Extension::GetMapWidth()
{
	return MAP_WIDTH;
}

int Extension::GetMapHeight()
{
	return MAP_HEIGHT;
}

int Extension::GetLightCount()
{
	int count = Scene::s_lights.size();
	return count;
}

int Extension::GetLightKeyByName(const char* name)
{
	return Scene::GetLightKeyByName(name);
}

int Extension::GetLightXByKey(int key)
{
	if (Scene::LightExists(key))
		return (int)Scene::s_lights[key].GetX();
	else
		return -1;
}

int Extension::GetLightYByKey(int key)
{
	if (Scene::LightExists(key))
		return (int)Scene::s_lights[key].GetY();
	else
		return -1;
}

float Extension::GetLightAngleByKey(int key)
{
	if (Scene::LightExists(key))
		return Scene::s_lights[key].m_angle;
	else
		return 0.0f;
}

float Extension::GetLightScaleByKey(int key)
{
	if (Scene::LightExists(key))
		return Scene::s_lights[key].GetScale();
	else
		return 0.0f;
}

float Extension::GetLightRedComponentByKey(int key)
{
	if (Scene::LightExists(key))
		return Scene::s_lights[key].m_color.x;
	else
		return 0.0f; 
}

float Extension::GetLightGreenComponentByKey(int key)
{
	if (Scene::LightExists(key))
		return Scene::s_lights[key].m_color.g;
	else
		return 0.0f; 
}

float Extension::GetLightBlueComponentByKey(int key)
{
	if (Scene::LightExists(key))
		return Scene::s_lights[key].m_color.b;
	else
		return 0.0f; 
}

float Extension::GetLightBrightnessByKey(int key)
{
	if (Scene::LightExists(key))
		return Scene::s_lights[key].m_brightness;
	else
		return 0.0f; 
}

const char* Extension::GetLightTextureByKey(int key)
{
	//if (Scene::LightExists(key))
//		return Scene::s_lights[key].GetTextureName();
	//else
		return "light not found";
}

int Extension::GetLOSSoftShadowsAmount()
{
	return Renderer::s_softShadowsAmountLOS;
}

int Extension::GetLightingSoftShadowsAmount()
{
	return Renderer::s_softShadowsAmountLighting;
}

int Extension::GetWallEdgeInset()
{
	return Renderer::s_wallEdgeInset;
}

int Extension::GetLightsDrawn()
{
	return Light::s_lightsDrawn;
}
