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
	glm::vec3 m_color;
	int m_type;
	float m_brightness;
	float m_scale;
	float m_angle = 0;
	std::vector<std::tuple<float, float, float>> m_lightVisibilityPolygonPoints;
	Texture* m_texture;
	PairedObjectData m_pairedObjectData;
	std::string m_name;
	bool m_visibilityPolygonNeedsUpdate = true;
	GLuint m_vao;
	GLuint m_vbo;
	int m_vertexCount;

private:
	glm::vec2 m_position;

public: // functions
	//Light();
	Light();
	Light(std::string name, glm::vec2 position, glm::vec3 lightColor, float scale, int type, float strength, float angle);
	void DrawShadowCastingLight(Shader* shader, int gBufferID);
	void NextType();
	void SetTextureFromType();
	bool IsInScreenBounds();
	bool IsShadowCasting();
	bool IsPairedToObject();
	void SetPosition(float x, float y);
	float GetX();
	float GetY();
	AABB GetAABB();

	static int s_lightsDrawn;
};