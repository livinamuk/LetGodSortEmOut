
#include "Common.h"
#include "RaycastEngine.h"

///
/// EXTENSION CONSTRUCTOR/DESTRUCTOR
///

Extension::Extension(LPRDATA _rdPtr, LPEDATA edPtr, fpcob cobPtr)
    : rdPtr(_rdPtr), rhPtr(_rdPtr->rHo.hoAdRunHeader), Runtime(_rdPtr)
{
    /*
        Link all your action/condition/expression functions to their IDs to match the
        IDs in the JSON here
    */

    LinkAction(0, Resize);
    LinkAction(1, SetCamera);
    LinkAction(2, SetPlayerPosition);
    LinkAction(3, SetRenderMode);
    LinkAction(4, PairNewLightToActive);
    LinkAction(5, StartRendering);
    LinkAction(6, NewLight);
    LinkAction(7, SetLightPosition);
    LinkAction(8, SetLightColor);
    LinkAction(9, SetLightScale);
    LinkAction(10, SetLightType);
    LinkAction(11, SetLightStrength);
    LinkAction(12, SetLightRotation);
    LinkAction(13, SetCellValue);
    LinkAction(14, ToggleLOS);
    LinkAction(15, ToggleLIGHTING);
    LinkAction(16, RemoveLightByID);

    //LinkCondition(0, AreTwoNumbersEqual);
    //LinkCondition(1, ObjectsXEquals);

    LinkExpression(0, GetWidth);
    LinkExpression(1, GetHeight);
    LinkExpression(2, GetTileValue);
    LinkExpression(3, GetGridSize);
    LinkExpression(4, GetMapWidth);
    LinkExpression(5, GetMapHeight);
    LinkExpression(6, GetLightCount);
    LinkExpression(7, GetLightIDbyIndex);
    LinkExpression(8, GetLightXByIndex);
    LinkExpression(9, GetLightYByIndex);
    LinkExpression(10, GetLightRotationByIndex);
    LinkExpression(11, GetLightScaleByIndex);
    LinkExpression(12, GetLightRedComponentByIndex);
    LinkExpression(13, GetLightGreenComponentByIndex);
    LinkExpression(14, GetLightBlueComponentByIndex);
    LinkExpression(15, GetLightBrightnessByIndex);
    LinkExpression(16, GetLightTextureByIndex);

    // Load our object's width from the edittime data.
    rdPtr->rHo.hoImgWidth = edPtr->swidth;
    rdPtr->rHo.hoImgHeight = edPtr->sheight;

    if (!CreateGLWindow())
    {
        //MessageBoxA(nullptr, "Unable to create OpenGL window", "OpenGL", 0);
        return;
    }

    // Creates the rendering surface on first call
    RecreateSurface();

    InitGL();

    RaycastEngine::Init();






}

void Extension::RecreateSurface()
{
    if (surface.IsValid())
        surface.Delete();

    // Initialize our screen surface with an appropriate prototype - should match the pixel format in CreateGLWindow.
    cSurface* prototype{};
    GetSurfacePrototype(&prototype, 24, ST_MEMORY, SD_DIB);
    surface.Create(rdPtr->rHo.hoImgWidth, rdPtr->rHo.hoImgHeight, prototype);
   // surface.CreateAlpha();

    // Needed to read alpha data from GL separately; Fusion doesn't allow RGBA buffers.
    //alphaBuffer.resize(rdPtr->rHo.hoImgWidth * rdPtr->rHo.hoImgHeight);
}

Extension::~Extension()
{
    DeinitGL();
    DestroyGLWindow();
}

short Extension::Handle()
{
    if (!surface.IsValid())
        return REFLAG_ONESHOT;

    counter++;

    return REFLAG_DISPLAY;
}

short Extension::Display()
{
    if (!surface.IsValid())
        return 0;

    // Update Fixed Value Lights
    UpdateFixedValueLights();

    LPRH rhPtr = rdPtr->rHo.hoAdRunHeader;
    cSurface* screenSurface = WinGetSurface((int)rhPtr->rhIdEditWin);

    wglMakeCurrent(hDC, hRC);
    RenderGLFrame(screenSurface);

    auto blitMode = ((rdPtr->rs->rsEffect & EFFECTFLAG_TRANSPARENT) != 0) ? BMODE_TRANSP : BMODE_OPAQUE;
    auto blitOp = static_cast<BlitOp>(rdPtr->rs->rsEffect & EFFECT_MASK);
    auto blitParam = rdPtr->rs->rsEffectParam;
    //surface.Blit(*screenSurface, rdPtr->rHo.hoX, rdPtr->rHo.hoY, blitMode, blitOp, blitParam);
    surface.Blit(*screenSurface, rdPtr->rHo.hoX - rhPtr->rhWindowX, rdPtr->rHo.hoY - rhPtr->rhWindowY, blitMode, blitOp, blitParam);
    WinAddZone(rdPtr->rHo.hoAdRunHeader->rhIdEditWin, &rdPtr->rHo.hoRect);

    return 0;   
}



short Extension::Pause()
{

    // Ok
    return 0;
}

short Extension::Continue()
{

    // Ok
    return 0;
}

bool Extension::Save(HANDLE File)
{
	bool OK = false;

    #ifndef VITALIZE

	    // Save the object's data here

	    OK = true;

    #endif

	return OK;
}

bool Extension::Load(HANDLE File)
{
	bool OK = false;

    #ifndef VITALIZE

	    // Load the object's data here

	    OK = true;

    #endif

	return OK;
}


// These are called if there's no function linked to an ID

void Extension::Action(int ID, LPRDATA rdPtr, long param1, long param2)
{

}

long Extension::Condition(int ID, LPRDATA rdPtr, long param1, long param2)
{
    return false;
}

long Extension::Expression(int ID, LPRDATA rdPtr, long param)
{
	// Default: read parameters otherwise the Windows runtime can crash in expressions
    LPEVENTINFOS2 Infos = GetEventInformations((LPEVENTINFOS2) &::SDK->ExpressionInfos[0], ID);
    for(int i = 0; i < Infos->infos.nParams; ++ i)
    {
        switch(EVINFO2_PARAM(Infos, i))
        {
        case EXPPARAM_STRING:
            if(!i)
            {
                CNC_GetFirstExpressionParameter(rdPtr, param, TYPE_STRING);
                break;
            }
            CNC_GetNextExpressionParameter(rdPtr, param, TYPE_STRING);
            break;  

        case EXPPARAM_LONG:

            int Type = ((::SDK->ExpressionFloatFlags[ID] & (1 << i)) != 0) ? TYPE_FLOAT : TYPE_LONG;
            if(!i)
            {
                CNC_GetFirstExpressionParameter(rdPtr, param, Type);
                break;
            }
            CNC_GetNextExpressionParameter(rdPtr, param, Type);
            break;
        }
    }

    return 0;
}

void Extension::UpdateFixedValueLights()
{

    // Update positions
    for (int i = 0; i < Scene::s_lights.size(); i++)
    {
       Light* light = &Scene::s_lights[i];

        if (light->IsPairedToObject())
        {
            LPRO objectPtr = Runtime.LPROFromFixed(light->m_pairedObjectData.fixedValue);

            // Remove dead lights
            if (!objectPtr) {
                Scene::s_lights.erase(Scene::s_lights.begin() + i);
                i--;
            }
            else
            // update position
            {
                int xPos = objectPtr->roHo.hoX;
                int yPos = objectPtr->roHo.hoY;
                light->m_position = glm::vec2(xPos, yPos);
            }
        }
    }
}
