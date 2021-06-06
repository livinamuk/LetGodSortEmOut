#pragma once
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

class Quad2D
{
public:
	static void Init();
	static void RenderFullScreenQuad(Shader* shader);
	static void RenderQuad(Shader* shader, Texture* texture, int xScreenCoord, int yScreenCoord, float scale = 1.0f);

private:
	static GLuint s_smallerQuadVAO;
	static GLuint s_fullScreenQuadVAO;
};
