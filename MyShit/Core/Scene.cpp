#include "Scene.h"

std::unordered_map<int, Light> Scene::s_lights;
std::unordered_map<int, ShadowCastingShape> Scene::s_shadowCastingShape;
unsigned int Scene::s_lightKeyCounter = 0;
unsigned int Scene::s_shapeKeyCounter = 0;

void Scene::Update()
{

}

void Scene::AddLight(int x, int y, glm::vec3 lightColor, float lightScale, const char* textureName, float brightness, float angle, bool castShadows)
{
	s_lights[s_lightKeyCounter] = Light("noname", glm::vec2(x, y), lightColor, lightScale, textureName, brightness, angle, castShadows);
	s_lightKeyCounter++;
}

void Scene::AddNamedLight(const char* name, int x, int y, glm::vec3 color, float scale, const char* textureName, float brightness, float angle, bool castShadows)
{
	Light light(name, glm::vec2(x, y), color, scale, textureName, brightness, angle, castShadows);
	s_lights[s_lightKeyCounter] = light;
	s_lightKeyCounter++;
}

void Scene::AddFixedValueLight(int fixedValue, int hotspot, int xOffset, int yOffset, float r, float g, float b, float scale, const char* textureName, float brightness, float angle, bool castShadows)
{
	Light light("fixed_value_light", glm::vec2(0, 0), glm::vec3(r, g, b), scale, textureName, brightness, angle, castShadows);
	light.m_pairedObjectData.isPaired = true;
	light.m_pairedObjectData.fixedValue = fixedValue;
	light.m_pairedObjectData.followHotSpot = hotspot;
	light.m_pairedObjectData.xOffset = xOffset;
	light.m_pairedObjectData.yOffset = yOffset;
	s_lights[s_lightKeyCounter] = light;
	s_lightKeyCounter++;
}

void Scene::AddShadowCastingShape(const char* name, int x, int y, int width, int height, float angle)
{
	ShadowCastingShape shadowCastingShape(name, x, y, width, height, angle);
	s_shadowCastingShape[s_shapeKeyCounter] = shadowCastingShape;
	s_shapeKeyCounter++;
}

void Scene::AddFixedValueShadowCastingShape(int fixedValue, int width, int height, float angle)
{
	s_shadowCastingShape[fixedValue] = ShadowCastingShape(fixedValue, width, height, angle);
}

void Scene::ResetShadowCastingObjectModifiedFlags()
{
	for (auto& shape : s_shadowCastingShape)
		shape.second.ResetModifiedFlag();
}

void Scene::SaveScene(std::string filename)
{
	rapidjson::Document document;
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	rapidjson::Value savefileArray(rapidjson::kArrayType);
	document.SetObject();
	rapidjson::Value object(rapidjson::kObjectType);

	// Lights
	rapidjson::Value lightsArray;
	lightsArray.SetArray();

	//for (Light& light : s_lights) {
	for (auto& it : s_lights)
	{
		Light& light = it.second;
		rapidjson::Value flagObject;
		flagObject.SetObject();
		SaveInt(&flagObject, "X", light.GetX(), allocator);
		SaveInt(&flagObject, "Y", light.GetY(), allocator);
		SaveFloat(&flagObject, "ColorR", light.m_color.x, allocator);
		SaveFloat(&flagObject, "ColorG", light.m_color.y, allocator);
		SaveFloat(&flagObject, "ColorB", light.m_color.z, allocator);
		SaveString(&flagObject, "Texture", light.GetTextureName(), allocator);
		SaveFloat(&flagObject, "Angle", light.m_angle, allocator);
		SaveFloat(&flagObject, "Brightness", light.m_brightness, allocator);
		SaveFloat(&flagObject, "Scale", light.GetScale(), allocator);
		SaveInt(&flagObject, "ShadowCasting", light.IsShadowCasting(), allocator);
		lightsArray.PushBack(flagObject, allocator);
	}
	object.AddMember("Lights", lightsArray, allocator);


	savefileArray.PushBack(object, allocator);
	document.AddMember("Savefile", savefileArray, allocator);

	// Convert JSON document to string
	rapidjson::StringBuffer strbuf;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strbuf);
	document.Accept(writer);

	// Save it
	std::string data = strbuf.GetString();
	std::ofstream out(filename);
	out << data;
	out.close();
}
void Scene::LoadScene(std::string filename)
{
	ResetScene();

	std::string fileName = filename;
	FILE* pFile = fopen(fileName.c_str(), "rb");
	char buffer[65536];
	rapidjson::FileReadStream is(pFile, buffer, sizeof(buffer));
	rapidjson::Document document;
	document.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);

	// Check for errors
	if (document.HasParseError())
		std::cout << "Error  : " << document.GetParseError() << '\n' << "Offset : " << document.GetErrorOffset() << '\n';

	const rapidjson::Value& a = document["Savefile"];
	assert(a.IsArray());

	for (rapidjson::SizeType i = 0; i < a.Size(); i++)
	{
		auto element = a[i].GetObject();

		// Quests
		auto lights = element["Lights"].GetArray();
		for (rapidjson::SizeType j = 0; j < lights.Size(); j++) {
			auto light = lights[j].GetObject();

			int x = light["X"].GetInt();
			int y = light["Y"].GetInt();
			float r = light["ColorR"].GetFloat();
			float g = light["ColorG"].GetFloat();
			float b = light["ColorB"].GetFloat();
			const char* textureName = light["Texture"].GetString();
			float angle = light["Angle"].GetFloat();
			float brightness = light["Brightness"].GetFloat();
			float scale = light["Scale"].GetFloat();
			bool castShadows = light["ShadowCasting"].GetInt();

			Scene::AddLight(x, y, glm::vec3(r, g, b), scale, textureName, brightness, angle, castShadows);
		}
	}
}

void Scene::ResetScene()
{
	s_lights.clear();
	s_shadowCastingShape.clear();
	s_lightKeyCounter = 0;
	s_shapeKeyCounter = 0;
}

void Scene::UpdateAllLights()
{
	for (auto& it : s_lights)
	{
		Light& light = it.second;
		light.m_visibilityPolygonNeedsUpdate = true;
	}
}

int Scene::GetLightKeyByName(const char* name)
{
	std::string query = name;
	for (auto& it : s_lights)
		if (it.second.m_name == query)
			return it.first;
	return -1;
}

bool Scene::LightExists(int key)
{
	if (Scene::s_lights.find(key) != Scene::s_lights.end())
		return true;
	else
		return false;
}

void Scene::SaveString(rapidjson::Value* object, std::string elementName, std::string string, rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value name(elementName.c_str(), allocator);
	rapidjson::Value value(rapidjson::kObjectType);
	value.SetString(string.c_str(), static_cast<rapidjson::SizeType>(string.length()), allocator);
	object->AddMember(name, value, allocator);
}
void Scene::SaveFloat(rapidjson::Value* object, std::string elementName, float number, rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value name(elementName.c_str(), allocator);
	rapidjson::Value value(rapidjson::kObjectType);
	value.SetFloat(number);
	object->AddMember(name, value, allocator);
}
void Scene::SaveInt(rapidjson::Value* object, std::string elementName, int number, rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value name(elementName.c_str(), allocator);
	rapidjson::Value value(rapidjson::kObjectType);
	value.SetInt(number);
	object->AddMember(name, value, allocator);
}
void Scene::SaveVec3(rapidjson::Value* object, std::string elementName, glm::vec3 vector, rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value array(rapidjson::kArrayType);
	rapidjson::Value name(elementName.c_str(), allocator);
	array.PushBack(rapidjson::Value().SetFloat(vector.x), allocator);
	array.PushBack(rapidjson::Value().SetFloat(vector.y), allocator);
	array.PushBack(rapidjson::Value().SetFloat(vector.z), allocator);
	object->AddMember(name, array, allocator);
}
void Scene::SaveVec2(rapidjson::Value* object, std::string elementName, glm::vec2 vector, rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value array(rapidjson::kArrayType);
	rapidjson::Value name(elementName.c_str(), allocator);
	array.PushBack(rapidjson::Value().SetFloat(vector.x), allocator);
	array.PushBack(rapidjson::Value().SetFloat(vector.y), allocator);
	object->AddMember(name, array, allocator);
}

