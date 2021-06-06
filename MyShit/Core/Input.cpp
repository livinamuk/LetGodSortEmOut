//#include "hellpch.h"
#include "Input.h"
#include "keycodes.h"
#include "CoreGL.h"
//#include "Core/CoreImGui.h"
//#include "Game.h"
//#include "Logic/WeaponLogic.h"
#include <algorithm>
#include "Camera2D.h"

int Input::s_gridX;
int Input::s_gridY;
int Input::s_mouseX;
int Input::s_mouseY;
int Input::s_mouseWorldX;
int Input::s_mouseWorldY;
int Input::s_storedMouseX;
int Input::s_storedMouseY;
int Input::s_mouseX_Editor;
int Input::s_mouseY_Editor;
bool Input::s_showCursor = false;
bool Input::s_keyPressed[350] = { false };
bool Input::s_keyDown[350] = { false };
bool Input::s_keyDownLastFrame[350] = { false };
bool Input::s_leftMousePressed = false;
bool Input::s_rightMousePressed = false;
bool Input::s_leftMouseDown = false;
bool Input::s_rightMouseDown = false;
bool Input::s_leftMouseDownLastFrame = false;
bool Input::s_rightMouseDownLastFrame = false;
bool Input::s_showBulletDebug = false;
	
void Input::MouseUpdate(double xpos, double ypos)
{
	s_mouseX = xpos;
	s_mouseY = ypos;


	s_gridX = s_mouseWorldX / GRID_SIZE;
	s_gridY = s_mouseWorldY / GRID_SIZE;

	if (s_showCursor) {
		s_mouseX_Editor = xpos;
		s_mouseY_Editor = ypos;
	}

	Camera2D::AdjustProjection();
}

void Input::HandleKeypresses()
{
	// Toggle Bullet Debug Drawer
	if (s_keyPressed[HELL_KEY_B])
		s_showBulletDebug = !s_showBulletDebug;

	// Toggle cursor
	if (s_keyPressed[HELL_KEY_M])
		s_showCursor = !s_showCursor;

	// Toggle fullscreen
	if (s_keyPressed[HELL_KEY_F]) {
		CoreGL::ToggleFullScreen();

		/*for (Light& light : Scene::s_lights) {
			light.ConfigureFrameBuffer(CoreGL::s_windowWidth, CoreGL::s_windowHeight);
		}*/
	}
}