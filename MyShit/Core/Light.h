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
	float m_angle = 0;
	std::vector<std::tuple<float, float, float>> m_lightVisibilityPolygonPoints;
	PairedObjectData m_pairedObjectData;
	std::string m_name;
	bool m_visibilityPolygonNeedsUpdate = true;
	int m_vertexCount;

private:
	glm::vec2 m_position;
	AABB m_aabb;
	float m_scale;
	GLuint m_vao;
	GLuint m_vbo;
	bool m_castShadows;
	Texture* m_texture;

public: // functions
	Light();
	Light(std::string name, glm::vec2 position, glm::vec3 lightColor, float scale, const char* textureName, float strength, float angle, bool castShadows);
	void DrawShadowCastingLight(Shader* shader);
	void NextType();
	void SetTextureFromType();
	bool IsInScreenBounds();
	bool IsShadowCasting();
	void SetCastShadows(bool value);
	bool IsPairedToObject();
	void SetScale(float scale);
	float GetScale();
	void SetPosition(float x, float y);
	float GetX();
	float GetY();
	void SetTexture(std::string textureName);
	const char* GetTextureName();
	AABB GetAABB();

	static int s_lightsDrawn;
};