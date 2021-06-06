
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

const char* Extension::GetLightIDbyIndex(int index)
{
	if (index < 0 || index >= Scene::s_lights.size())
		return "out of range";
	return Scene::s_lights[index].m_name.c_str();
}

int Extension::GetLightXByIndex(int index)
{
	if (index < 0 || index >= Scene::s_lights.size())
		return -1;
	return (int)Scene::s_lights[index].m_position.x;
}

int Extension::GetLightYByIndex(int index)
{
	if (index < 0 || index >= Scene::s_lights.size())
		return -1;
	return (int)Scene::s_lights[index].m_position.y;
}

int Extension::GetLightRotationByIndex(int index)
{
	if (index < 0 || index >= Scene::s_lights.size())
		return -1;
	return Scene::s_lights[index].m_rotate;
}

float Extension::GetLightScaleByIndex(int index)
{
	if (index < 0 || index >= Scene::s_lights.size())
		return -1;
	return Scene::s_lights[index].m_scale;
}

float Extension::GetLightRedComponentByIndex(int index)
{
	if (index < 0 || index >= Scene::s_lights.size())
		return -1;
	return Scene::s_lights[index].m_color.x;
}

float Extension::GetLightGreenComponentByIndex(int index)
{
	if (index < 0 || index >= Scene::s_lights.size())
		return -1;
	return Scene::s_lights[index].m_color.g;
}

float Extension::GetLightBlueComponentByIndex(int index)
{
	if (index < 0 || index >= Scene::s_lights.size())
		return -1;
	return Scene::s_lights[index].m_color.b;
}

float Extension::GetLightBrightnessByIndex(int index)
{
	if (index < 0 || index >= Scene::s_lights.size())
		return -1;
	return Scene::s_lights[index].m_strength;
}

const char* Extension::GetLightTextureByIndex(int index)
{
	return "TO DO";
}
