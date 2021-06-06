#include "Quad2D.h"
#include <algorithm>

GLuint Quad2D::s_smallerQuadVAO;
GLuint Quad2D::s_fullScreenQuadVAO;

void Quad2D::Init()
{
	{
		unsigned int s_smallerQuadVBO;
		glGenVertexArrays(1, &s_smallerQuadVAO);
		glGenBuffers(1, &s_smallerQuadVBO);
		float quadVertices[] = {
			// positions         texcoords
			0.0f,  1.0f, 0.0f,  0.0f, 0.0f,
			0.0f,  0.0f, 0.0f,  0.0f, 1.0f,
			1.0f,  1.0f, 0.0f,  1.0f, 0.0f,
			1.0f,  0.0f, 0.0f,  1.0f, 1.0f,
		};
		glGenVertexArrays(1, &s_smallerQuadVAO);
		glGenBuffers(1, &s_smallerQuadVBO);
		glBindVertexArray(s_smallerQuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, s_smallerQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	{
		unsigned int s_fullScreenQuadVBO;
		glGenVertexArrays(1, &s_fullScreenQuadVAO);
		glGenBuffers(1, &s_fullScreenQuadVBO);
		float quadVertices[] = {
			// positions         texcoords
			-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
				1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
				1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		};
		glGenVertexArrays(1, &s_fullScreenQuadVAO);
		glGenBuffers(1, &s_fullScreenQuadVBO);
		glBindVertexArray(s_fullScreenQuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, s_fullScreenQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
}

void Quad2D::RenderFullScreenQuad(Shader* shader)
{
	shader->setMat4("model", glm::mat4(1));
	glBindVertexArray(s_fullScreenQuadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void Quad2D::RenderQuad(Shader* shader, Texture* texture, int xScreenCoord, int yScreenCoord, float scale)
{
	if (texture == nullptr)
		return;

	float width = (texture->width / SCR_WIDTH) * 2;
	float height = (texture->height / SCR_HEIGHT) * 2;
	glm::mat4 modelMatrix = glm::mat4(1.0f);

	float x = (xScreenCoord / SCR_WIDTH) * 2 - 1;;
	float y = ((SCR_HEIGHT - yScreenCoord - texture->height * scale) / SCR_HEIGHT) * 2 - 1;

	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(width * scale, height * scale, 1));
	shader->setMat4("model", modelMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->ID);

	glBindVertexArray(s_smallerQuadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	shader->setVec3("color", glm::vec3(1));
}

