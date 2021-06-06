#pragma once
#include "Header.h"
#include "Core/CoreGL.h"
#include "Core/Input.h"
#include "Core/WorldMap.h"
#include "Helpers/Util.h"
#include "Renderer/Texture.h"
#include "Renderer/TextBlitter.h"
#include "Renderer/Renderer.h"
#include "Core/Camera2D.h"
#include "Core/Scene.h"


class RaycastEngine
{
public: // static functions
	static void Init();
	static void RenderLoop();

	static void SetCell(int x, int y, int value);

	static GLuint s_mainFrameBuffer;

	static bool s_render;
};