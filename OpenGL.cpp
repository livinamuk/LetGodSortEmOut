
#include "Common.h"
#include "RaycastEngine.h"

// OpenGL initialization code taken from
// https://gist.github.com/nickrolfe/1127313ed1dbf80254b614a721b3ee9c#file-windows_modern_opengl_context-c-L7
// The render function is at the bottom.

typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext,
													 const int* attribList);
wglCreateContextAttribsARB_type* wglCreateContextAttribsARB;

// See https://www.opengl.org/registry/specs/ARB/wgl_create_context.txt for all values
#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126
#define WGL_CONTEXT_FLAGS_ARB					  0x2094

#define WGL_CONTEXT_DEBUG_BIT_ARB				  0x00000001

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001

typedef BOOL WINAPI wglChoosePixelFormatARB_type(HDC hdc, const int* piAttribIList,
												 const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
wglChoosePixelFormatARB_type* wglChoosePixelFormatARB;

// See https://www.opengl.org/registry/specs/ARB/wgl_pixel_format.txt for all values
#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011
#define WGL_PIXEL_TYPE_ARB                        0x2013
#define WGL_COLOR_BITS_ARB                        0x2014
#define WGL_DEPTH_BITS_ARB                        0x2022
#define WGL_STENCIL_BITS_ARB                      0x2023
#define WGL_ALPHA_BITS_ARB                        0x201B

#define WGL_FULL_ACCELERATION_ARB                 0x2027
#define WGL_TYPE_RGBA_ARB                         0x202B

static void ShowError(const char* msg)
{
	MessageBoxA(NULL, msg, "Error", MB_OK | MB_ICONEXCLAMATION);
}

#define GL_WINDOW_CLASS "Dummy_WGL_djuasiodwa"

static bool InitGLExtensions()
{
	// Before we can load extensions, we need a dummy OpenGL context, created using a dummy window.
	// We use a dummy window because you can only set the pixel format for a window once. For the
	// real window, we want to use wglChoosePixelFormatARB (so we can potentially specify options
	// that aren't available in PIXELFORMATDESCRIPTOR), but we can't load and use that before we
	// have a context.
	WNDCLASSA window_class{};
	window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window_class.lpfnWndProc = DefWindowProcA;
	window_class.hInstance = GetModuleHandle(nullptr);
	window_class.lpszClassName = GL_WINDOW_CLASS;

	if (!RegisterClassA(&window_class)) {
		ShowError("Failed to register dummy OpenGL window.");
		return false;
	}

	HWND dummy_window = CreateWindowExA(
		0,
		window_class.lpszClassName,
		"Dummy OpenGL Window",
		0,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		window_class.hInstance,
		0);

	if (!dummy_window) {
		ShowError("Failed to create dummy OpenGL window.");
		return false;
	}

	HDC dummy_dc = GetDC(dummy_window);

	PIXELFORMATDESCRIPTOR pfd{};
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.cColorBits = 32;
	pfd.cAlphaBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

	int pixel_format = ChoosePixelFormat(dummy_dc, &pfd);
	if (!pixel_format) {
		ShowError("Failed to find a suitable pixel format.");
		return false;
	}
	if (!SetPixelFormat(dummy_dc, pixel_format, &pfd)) {
		ShowError("Failed to set the pixel format.");
		return false;
	}

	HGLRC dummy_context = wglCreateContext(dummy_dc);
	if (!dummy_context) {
		ShowError("Failed to create a dummy OpenGL rendering context.");
		return false;
	}

	if (!wglMakeCurrent(dummy_dc, dummy_context)) {
		ShowError("Failed to activate dummy OpenGL rendering context.");
		return false;
	}

	wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)wglGetProcAddress(
		"wglCreateContextAttribsARB");
	wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*)wglGetProcAddress(
		"wglChoosePixelFormatARB");

	wglMakeCurrent(dummy_dc, 0);
	wglDeleteContext(dummy_context);
	ReleaseDC(dummy_window, dummy_dc);
	DestroyWindow(dummy_window);
	UnregisterClassA(GL_WINDOW_CLASS, GetModuleHandle(nullptr));
	return true;
}


static HGLRC CreateGLContext(HDC real_dc)
{
	if (!InitGLExtensions())
		return NULL;

	// Now we can choose a pixel format the modern way, using wglChoosePixelFormatARB.
	int pixel_format_attribs[] = {
		WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
		WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
		WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB,         32,
		WGL_DEPTH_BITS_ARB,         24,
		WGL_STENCIL_BITS_ARB,       8,
		WGL_ALPHA_BITS_ARB,			8,
		0
	};

	int pixel_format;
	UINT num_formats;
	wglChoosePixelFormatARB(real_dc, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
	if (!num_formats) {
		ShowError("Failed to set the OpenGL 4.4 pixel format.");
	}

	PIXELFORMATDESCRIPTOR pfd;
	DescribePixelFormat(real_dc, pixel_format, sizeof(pfd), &pfd);
	if (!SetPixelFormat(real_dc, pixel_format, &pfd)) {
		ShowError("Failed to set the OpenGL 4.4 pixel format.");
	}

	// Specify that we want to create an OpenGL 4.4 core profile context
	int gl44_attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 4,
		WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
#ifdef _DEBUG
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
#endif
		0,
	};

	HGLRC gl44_context = wglCreateContextAttribsARB(real_dc, 0, gl44_attribs);
	if (!gl44_context) {
		ShowError("Failed to create OpenGL 4.4 context.");
	}

	if (!wglMakeCurrent(real_dc, gl44_context)) {
		ShowError("Failed to activate OpenGL 4.4 rendering context.");
	}

	return gl44_context;
}

bool Extension::CreateGLWindow()
{
	hWnd = CreateWindow("STATIC", 0, WS_CHILDWINDOW, rdPtr->rHo.hoX, rdPtr->rHo.hoY, rdPtr->rHo.hoImgWidth, rdPtr->rHo.hoImgHeight,
						rdPtr->rHo.hoAdRunHeader->rhHEditWin, 0, rdPtr->rHo.hoAdRunHeader->rh4.rh4Instance, 0);
	hDC = GetDC(hWnd);
	hRC = CreateGLContext(hDC);
	if (!hRC)
		return false;

	if (!wglMakeCurrent(hDC, hRC))
	{
		ShowError("Cannot make GL context current");
		return false;
	}

	const auto gladLoader = [](const char* name) -> void* {
		auto proc = wglGetProcAddress(name);
		static auto glInstance = LoadLibraryA("opengl32.dll");
		if (proc)
			return proc;
		return GetProcAddress(glInstance, name);
	};

	if (!gladLoadGLLoader(gladLoader))
	{
		ShowError("Cannot initialize GLAD");
		return false;
	}

	return true;
}

void Extension::DestroyGLWindow()
{
	wglMakeCurrent(nullptr, nullptr);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd, hDC);
	DestroyWindow(hWnd);
}

void GLAPIENTRY
MessageCallback(GLenum source,
				GLenum type,
				GLuint id,
				GLenum severity,
				GLsizei length,
				const GLchar* message,
				const void* userParam)
{
	char foo[1000];
	sprintf(foo, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
	OutputDebugString(foo);
}


// https://antongerdelan.net/opengl/hellotriangle.html
void Extension::InitGL()
{
#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);
#endif
}

void Extension::DeinitGL()
{
	wglMakeCurrent(hDC, hRC);
	glDeleteProgram(mainPassProgram);
	glDeleteVertexArrays(1, &triangleVAO);
	glDeleteBuffers(1, &triangleVBO);
	if constexpr (EnableCompositing)
	{
		glDeleteProgram(compositePassProgram);
		glDeleteVertexArrays(1, &screenQuadVAO);
		glDeleteBuffers(1, &screenQuadVBO);
	}
}

void Extension::RenderGLFrame(cSurface* screenSurface)
{
	RenderMainPass();
	
	glFlush();
	glFinish();

	// Retrieve the OpenGL output as Fusion surface

	BYTE* buffer = surface.LockBuffer();

	// Fusion weirdness, ignore...
	int pitch = surface.GetPitch();
	if (pitch < 0)
	{
		pitch *= -1;
		buffer -= pitch * (surface.GetHeight() - 1);
	}

	// Read color data
	glReadBuffer(GL_BACK);
	glReadPixels(0, 0, surface.GetWidth(), surface.GetHeight(), GL_BGR, GL_UNSIGNED_BYTE, reinterpret_cast<GLvoid*>(buffer));
	surface.UnlockBuffer(buffer);
}

// Renders our custom OpenGL scene to the screen FBO
void Extension::RenderMainPass()
{
	RaycastEngine::RenderLoop();
}