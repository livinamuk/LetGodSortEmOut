#pragma once
#include "Header.h"
#include "Renderer/Texture.h"

struct PairedObjectData {
	bool isPaired = false;
	bool followHotSpot = true;
	int fixedValue = 0;
	int xOffset;
	int yOffset;
};

class Light
{
public: // fields
	glm::vec2 m_position;
	glm::vec3 m_color;
	int m_type;
	int m_rotate;
	float m_strength;
	float m_scale;
	std::vector<std::tuple<float, float, float>> m_lightVisibilityPolygonPoints;
	Texture* m_texture;
	PairedObjectData m_pairedObjectData;
	std::string m_name;

public: // functions
	//Light();
	Light();
	Light(std::string name, glm::vec2 position, glm::vec3 lightColor, float scale, int type, float strength, int rotate);
	void DrawSolidPolygon(Shader* shader);
	void DrawOutline(Shader* shader);
	void DrawShadowCastingLight(Shader* shader, int gBufferID);
	void NextType();
	void RotateLight();
	void SetTextureFromType();
	bool IsInScreenBounds();
	bool IsShadowCasting();
	bool IsPairedToObject();
};