#pragma once
#include "Header.h"
#include "Core/Light.h"
#include "Helpers/Util.h"
#include <unordered_map>

class Scene
{
public: // static functions
	static void AddLight(int x, int y, glm::vec3 color, float scale, int type, float strength, float angle);
	static void AddNamedLight(const char* name, int x, int y, glm::vec3 color, float scale, int type, float strength, float angle);
	static void AddFixedValueLight(int fixedValue, int hotspot, int xOffset, int yOffset, float r, float g, float b, float scale, int type, float strength, float angle);
	static void SaveScene(std::string filename);
	static void LoadScene(std::string filename);
	static void ResetScene();
	//static Light* GetLightByName(std::string name);
	static void UpdateAllLights();
	static int GetLightKeyByName(const char* name);
	static bool LightExists(int key);

public: // static class varaibles
	//static std::vector<Light> s_lights;
	static std::unordered_map<int, Light> s_lights;
	static unsigned int s_lightKeyCounter;

private:
	static void SaveString(rapidjson::Value* object, std::string elementName, std::string string, rapidjson::Document::AllocatorType& allocator);
	static void SaveFloat(rapidjson::Value* object, std::string elementName, float number, rapidjson::Document::AllocatorType& allocator);
	static void SaveInt(rapidjson::Value* object, std::string elementName, int number, rapidjson::Document::AllocatorType& allocator);
	static void SaveVec3(rapidjson::Value* object, std::string elementName, glm::vec3 vector, rapidjson::Document::AllocatorType& allocator);
	static void SaveVec2(rapidjson::Value* object, std::string elementName, glm::vec2 vector, rapidjson::Document::AllocatorType& allocator);
};
