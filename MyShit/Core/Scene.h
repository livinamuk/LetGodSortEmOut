#pragma once
#include "Header.h"
#include "Core/ShadowCastingShape.h"
#include "Core/Light.h"
#include "Helpers/Util.h"
#include <unordered_map>

class Scene
{
public: // static functions
	static void Update();
	static void AddLight(int x, int y, glm::vec3 color, float scale, const char* textureName, float strength, float angle, bool castShadows);
	static void AddNamedLight(const char* name, int x, int y, glm::vec3 color, float scale, const char* textureName, float strength, float angle, bool castShadows);
	static void AddFixedValueLight(int fixedValue, int hotspot, int xOffset, int yOffset, float r, float g, float b, float scale, const char* textureName, float strength, float angle, bool castShadows);
	static void SaveScene(std::string filename);
	static void LoadScene(std::string filename);
	static void ResetScene();
	static void UpdateAllLights();
	static int GetLightKeyByName(const char* name);
	static bool LightExists(int key);
	static void AddShadowCastingShape(const char* name, int x, int y, int width, int height, float angle);
	static void AddFixedValueShadowCastingShape(int fixedValue, int width, int height, float angle);
	static void ResetShadowCastingObjectModifiedFlags();


public: // static class varaibles
	static std::unordered_map<int, Light> s_lights;
	static std::unordered_map<int, ShadowCastingShape> s_shadowCastingShape;
	static unsigned int s_lightKeyCounter;
	static unsigned int s_shapeKeyCounter;

private:
	static void SaveString(rapidjson::Value* object, std::string elementName, std::string string, rapidjson::Document::AllocatorType& allocator);
	static void SaveFloat(rapidjson::Value* object, std::string elementName, float number, rapidjson::Document::AllocatorType& allocator);
	static void SaveInt(rapidjson::Value* object, std::string elementName, int number, rapidjson::Document::AllocatorType& allocator);
	static void SaveVec3(rapidjson::Value* object, std::string elementName, glm::vec3 vector, rapidjson::Document::AllocatorType& allocator);
	static void SaveVec2(rapidjson::Value* object, std::string elementName, glm::vec2 vector, rapidjson::Document::AllocatorType& allocator);
};
