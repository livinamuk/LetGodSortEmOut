#include <glad/glad.h>

class Extension
{
public:

    LPRDATA rdPtr;
    LPRH    rhPtr;
    
    Edif::Runtime Runtime;

    static const int MinimumBuild = 251;
    static const int Version = 1;

    static const int OEFLAGS = OEFLAG_NEVERKILL | OEFLAG_BACKSAVE | OEFLAG_NEVERKILL | OEFLAG_NEVERSLEEP | OEFLAG_SPRITES;
    static const int OEPREFS = OEPREFS_INKEFFECTS | OEPREFS_BACKSAVE | OEPREFS_BACKEFFECTS | OEPREFS_SCROLLINGINDEPENDANT;
    
    static const int WindowProcPriority = 100;

    Extension(LPRDATA rdPtr, LPEDATA edPtr, fpcob cobPtr);
    ~Extension();

    HWND hWnd; // Window handle
    HDC hDC; // Device context handle
    HGLRC hRC; // Render context handle
    cSurface surface; // Surface that gets rendered to the screen
    std::vector<BYTE> alphaBuffer; // Necessary to read the alpha values from GL - Fusion surfaces separate color and alpha 

    int counter{};

    GLuint mainPassProgram{};
    GLuint colorUniform{};
    GLuint triangleVBO{}, triangleVAO{};

    // If true, the below GLuints are used to create an FBO that the main pass is rendered to,
    // which is then rendered onto the screen in a compositing texture that has access to the
    // background of the OpenGL object.
    // Note that it's currently unfinished as it assumes the OpenGL object covers the entire screen.
    static constexpr bool EnableCompositing = false;

    GLuint mainTexture{};
    GLuint mainFB{};
    GLuint mainDepthBuffer{};
    GLuint backgroundTexture{};
    GLuint compositePassProgram{};
    GLuint mainTextureUniform{}, backgroundTextureUniform{};
    GLuint screenQuadVBO{}, screenQuadVAO{};



    /*  Add your actions, conditions and expressions as real class member
        functions here. The arguments (and return type for expressions) must
        match EXACTLY what you defined in the JSON.

        Remember to link the actions, conditions and expressions to their
        numeric IDs in the class constructor (Extension.cpp)
    */

    /// Actions

    void Resize(int width, int height);
    void SetCamera(int x, int y);
    void SetPlayerPosition(int x, int y);
    void SetRenderMode(int mode);
    void NextRenderMode();
    void StartRendering();
    //void PauseRendering();
    void PairNewLightToActive(int fixedValue, bool hotspot, int xOffset, int yOffset, float r, float g, float b, float scale, const char* textureName, float brightness, int rotate, bool castShadows);
    void NewLight(const char* name, int x, int y, float r, float g, float b, float scale, const char* textureName, float brightness, int rotate, bool castShadows);
    void SetLightPosition(int key, int x, int y);
    void SetLightColor(int key, float r, float g, float b);
    void SetLightScale(int key, float scale);
    void SetLightTexture(int key, const char* textureName);
    void SetLightBrightness(int key, float brightness);
    void SetLightAngle(int key, float angle);
    void SetLightShadowCasting(int key, int value);
    void SetCellValue(int x, int y, int value);
    void ToggleLOS();
    void ToggleLIGHTING();
    //void RemoveLightByID(const char* name);
    void RemoveLightByKey(int key);
    void SetLOSShadowSoftness(int levels);
    void SetLightingShadowSoftness(int levels);
    void ToggleEditor();
    void SetWallEdgeInset(int inset);
    void SaveMap(const char* filepath);
    void LoadMap(const char* filepath);
    void NewShadowCastingShape(const char* name, int x, int y, int width, int height, int angle);
    void PairNewShadowCastingShapeToActive(int fixedValue, int width, int height, int angle);

    /// Conditions
    bool IsEditorOpen();
    //bool AreTwoNumbersEqual(int FirstNumber, int SecondNumber);
	//bool ObjectsXEquals(RunObject* Obj, int XPosition);

    /// Expressions
        
    int GetWidth() const;
    int GetHeight() const;
    int GetTileValue(int x, int y);
    int GetGridSize();
    int GetMapWidth();
    int GetMapHeight();
    int GetLightCount();
    //const char* GetLightIDbyIndex(int key);
    int GetLightKeyByName(const char* name);
    int GetLightXByKey(int key);
    int GetLightYByKey(int key);
    float GetLightAngleByKey(int key);
    float GetLightScaleByKey(int key);
    float GetLightRedComponentByKey(int key);
    float GetLightGreenComponentByKey(int key);
    float GetLightBlueComponentByKey(int key);
    float GetLightBrightnessByKey(int key);
    const char* GetLightTextureByKey(int key);
    int GetLOSSoftShadowsAmount();
    int GetLightingSoftShadowsAmount();
    int GetWallEdgeInset();
    int GetLightsDrawn();



    /* These are called if there's no function linked to an ID */

    void Action(int ID, LPRDATA rdPtr, long param1, long param2);
    long Condition(int ID, LPRDATA rdPtr, long param1, long param2);
    long Expression(int ID, LPRDATA rdPtr, long param);




    /*  These replace the functions like HandleRunObject that used to be
        implemented in Runtime.cpp. They work exactly the same, but they're
        inside the extension class.
    */

    short Handle();
    short Display();

    short Pause();
    short Continue();

    bool Save(HANDLE File);
    bool Load(HANDLE File);

private:

    void RecreateSurface();
    bool CreateGLWindow();
    void DestroyGLWindow();
    void InitGL();
    void DeinitGL();
    void RenderGLFrame(cSurface * screenSurface);
    void RenderMainPass();

    void UpdateFixedValueLights();
};