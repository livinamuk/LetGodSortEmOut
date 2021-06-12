
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
   
}

void Extension::ToggleLOS()
{
    Renderer::ToggleLOS();
}

void Extension::ToggleLIGHTING()
{
    Renderer::ToggleLIGHTING();
}

/*
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
}*/

void Extension::RemoveLightByKey(int key)
{
    Scene::s_lights.erase(key);
}

void Extension::SetLOSShadowSoftness(int levels)
{
    Renderer::SetLineOfSightBlurLevels(levels);
}

void Extension::SetLightingShadowSoftness(int levels)
{
    Renderer::SetLightingBlurLevels(levels);
}

void Extension::ToggleEditor()
{
    Renderer::s_editorMode = !Renderer::s_editorMode;
}

void Extension::SetWallEdgeInset(int inset)
{
    Renderer::s_wallEdgeInset = inset;
}

void Extension::SaveMap(const char* filepath)
{
    WorldMap::SaveMap(filepath);
}

void Extension::LoadMap(const char* filepath)
{
    WorldMap::LoadMap(filepath);
}

void Extension::NewShadowCastingShape(const char* name, int x, int y, int width, int height, int angle)
{
    Scene::AddShadowCastingShape(name, x, y, width, height, angle);
}

void Extension::PairNewShadowCastingShapeToActive(int fixedValue, int width, int height, int angle)
{
    Scene::AddFixedValueShadowCastingShape(fixedValue, width, height, angle);
}

bool Extension::IsEditorOpen()
{
    return Renderer::s_editorMode;
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

void Extension::PairNewLightToActive(int fixedValue, bool hotspot, int xOffset, int yOffset, float r, float g, float b, float scale, const char* textureName, float brightness, int angle, bool castShadows)
{

    LPRO objectPtr = Runtime.LPROFromFixed(fixedValue);

    // If fixed value wasn't found, BAIL
    if (!objectPtr)
        return;

    // Otherwise, create new fixed value light
    Scene::AddFixedValueLight(fixedValue, hotspot, xOffset, yOffset, r, g, b, scale, textureName, brightness, angle, castShadows);
}

void Extension::NewLight(const char* name, int x, int y, float r, float g, float b, float scale, const char* textureName, float brightness, int angle, bool castShadows)
{
     Scene::AddNamedLight(name, x, y, glm::vec3(r, g, b), scale, textureName, brightness, angle, castShadows);
}

void Extension::SetLightPosition(int key, int x, int y)
{
    if (Scene::LightExists(key))
        Scene::s_lights[key].SetPosition(x, y);
    else {

        Util::Log(std::to_string(key) + " not found\n");

        for (auto& it : Scene::s_lights)
        {
            int k = it.first;
            //Util::Log(std::to_string(k) + "");
        }
    }
}

void Extension::SetLightColor(int key, float r, float g, float b)
{
    if (Scene::LightExists(key))
        Scene::s_lights[key].m_color = glm::vec3(r, g, b);
}

void Extension::SetLightScale(int key, float scale)
{
    if (Scene::LightExists(key))
        Scene::s_lights[key].SetScale(scale);
}

void Extension::SetLightTexture(int key, const char* textureName)
{
    if (Scene::LightExists(key))
        Scene::s_lights[key].SetTexture(textureName);
}


void Extension::SetLightBrightness(int key, float brightness)
{
    if (Scene::LightExists(key))
        Scene::s_lights[key].m_brightness = brightness;
}

void Extension::SetLightAngle(int key, float angle)
{
    if (Scene::LightExists(key))
        Scene::s_lights[key].m_angle = angle;
}

void Extension::SetLightShadowCasting(int key, int value)
{
    if (Scene::LightExists(key))
        Scene::s_lights[key].SetCastShadows(value);
}

void Extension::SetCellValue(int x, int y, int value)
{
    RaycastEngine::SetCell(x, y, value);
}