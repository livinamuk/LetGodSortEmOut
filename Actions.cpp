
#include "Common.h"
#include "RaycastEngine.h"

void Extension::Resize(int width, int height)
{
    rdPtr->rHo.hoImgWidth = max(1, width);
    rdPtr->rHo.hoImgHeight = max(1, height);
    rdPtr->rc->rcChanged = true;

    RecreateSurface();

    SetWindowPos(hWnd, HWND_TOP, 0, 0, surface.GetWidth(), surface.GetHeight(), SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOZORDER);
}

void Extension::SetCamera(int x, int y)
{
    Camera2D::s_scrollX = x;
    Camera2D::s_scrollY = y;
}

void Extension::SetPlayerPosition(int x, int y)
{
    Input::s_mouseWorldX = x;
    Input::s_mouseWorldY = y;
}

void Extension::SetRenderMode(int mode)
{
    if (mode < 0 || mode > 2)
        return;
    Renderer::s_renderMode = mode;
}

void Extension::NextRenderMode()
{
    Renderer::NextRenderMode();
}

void Extension::ToggleLOS()
{
    Renderer::ToggleLOS();
}

void Extension::ToggleLIGHTING()
{
    Renderer::ToggleLIGHTING();
}

void Extension::RemoveLightByID(const char* name)
{
    for (int i = 0; i < Scene::s_lights.size(); i++)
    {
        Light* light = &Scene::s_lights[i];

        std::string queryName = name;;
        if (light->m_name == queryName) {
            Scene::s_lights.erase(Scene::s_lights.begin() + i);
            return;
        }
    }
}

void Extension::StartRendering()
{
    RaycastEngine::s_render = true;
}

/*
void Extension::PauseRendering()
{
    RaycastEngine::s_render = false;
}*/

void Extension::PairNewLightToActive(int fixedValue, bool hotspot, int xOffset, int yOffset, float r, float g, float b, float scale, int type, float strength, int rotation)
{

    LPRO objectPtr = Runtime.LPROFromFixed(fixedValue);

    // If fixed value wasn't found, BAIL
    if (!objectPtr)
        return;

    // Otherwise, create new fixed value light
    Scene::AddFixedValueLight(fixedValue, hotspot, xOffset, yOffset, r, g, b, scale, type, strength, rotation);
}

void Extension::NewLight(const char* name, int x, int y, float r, float g, float b, float scale, int type, float strength, int rotate)
{
    //Scene::AddRuntimeLight(name, x, y, glm::vec3(r, g, b), scale, type, strength, rotate);
    Scene::AddRuntimeLight(name, x, y, glm::vec3(r, g, b), scale, type, strength, rotate);
}

void Extension::SetLightPosition(const char* name, int x, int y)
{
    Light* light = Scene::GetLightByName(name);
    if (light)
        light->m_position = glm::vec2(x, y);
}

void Extension::SetLightColor(const char* name, float r, float g, float b)
{
    Light* light = Scene::GetLightByName(name);
    if (light)
        light->m_color = glm::vec3(r, g, b);
}

void Extension::SetLightScale(const char* name, float scale)
{
    Light* light = Scene::GetLightByName(name);
    if (light)
        light->m_scale = scale;
}

void Extension::SetLightType(const char* name, int type)
{
    Light* light = Scene::GetLightByName(name);
    if (light)
        light->m_type = type;
}

void Extension::SetLightStrength(const char* name, float strength)
{
    Light* light = Scene::GetLightByName(name);
    if (light)
        light->m_strength = strength;
}

void Extension::SetLightRotation(const char* name, int rotation)
{
    Light* light = Scene::GetLightByName(name);
    if (light)
        light->m_rotate = rotation;
}

void Extension::SetCellValue(int x, int y, int value)
{
    RaycastEngine::SetCell(x, y, value);
}