#pragma once
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/BlurBuffer.h"
#include "Core/CoreGL.h"
#include "Renderer/GBuffer.h"
#include "Renderer/Quad2D.h"
#include "Core/WorldMap.h"
#include "Core/Camera2D.h"
#include "Core/Scene.h"
#include "Helpers/Util.h"

class Renderer
{

public: // functions
	static void Init();
	static void RenderFrame();
	static void CheckForKeyPresses();
	static void RenderTiledWorld(Shader* shader);
	static void RenderEdgeMap(Shader* shader);
	static void RenderPlayerLineOfSight(Shader* shader);
	static void RenderLightIcons(Shader* shader);
	static void RenderFinalImage(Shader* shader);
	static void TextBlitterPass(Shader* shader);
	static void DrawLine(Shader* shader, Line line, glm::mat4 modelMatrix);
	static void HotLoadShaders();
	static void DrawPoint(Shader* shader, int x, int y, glm::vec3 color);
	static void DrawLineByWorldCoords(Shader* shader, int x, int y, int x2, int y2, glm::vec3 color);
	static void DrawTile(Shader* shader, Tile tile, int gridX, int gridY);
	static void LightingPass();
	static void HandleEditorInput();
	static void DownScale(int sourceFBO, int sourceColorAttachment, int levels);
	static void ToggleLOS();
	static void ToggleLIGHTING();
	static void SetLineOfSightBlurLevels(int levels);
	static void SetLightingBlurLevels(int levels);
	static void RenderAABB(Shader* shader, AABB& aabb);

public: // variables
	static Shader s_solid_color_shader;
	static Shader s_textued_2D_quad_shader;
	static Shader s_player_line_of_sight_shader;
	static Shader s_blurVerticalShader;
	static Shader s_blurHorizontalShader;
	static Shader s_composite;
	static Shader s_shadowCastingLightShader;
	static Shader s_nonShadowCastingLightShader;
	static GBuffer s_gBuffer;
	static std::vector<BlurBuffer> s_BlurBuffers;

	// These are used to store the the shadow casting light vertices, recreated for each light each frame.
	static GLuint s_lightVAO;
	static GLuint s_lightVBO;
	static GLuint s_playerVisibilityVAO;
	static GLuint s_playerVisibilityVBO;

	static int s_selectedLight;
	static int s_hoveredLight;
	static int s_renderMode;

	static bool s_editorMode;
	static bool s_renderLOS;
	static bool s_renderLIGHTING;

	static int s_wallEdgeInset;

	static int s_softShadowsAmountLOS;
	static int s_softShadowsAmountLighting;

	static int s_testCounter;

	static int s_lightUpdateCounter;
	static int s_maxLightUpdatesPerFrame;
};

