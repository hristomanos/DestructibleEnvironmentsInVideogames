#pragma once

#include <d3d11_1.h>
#include <string>
#include <windows.h>
#include <windowsx.h>

#include "DDSTextureLoader.h"
#include "Camera2.h"
#include "Camera.h"
#include "DirectXColors.h"
#include "DrawableGameObject.h"
#include "Graphics.h"
#include "resource.h"
#include "structures.h"
#include "Utilities/Helper.h"

#include "DirectInput.h"

#define DEFAULT_WINDOW_TITLE L"Framework";
#define DEFAULT_WINDOW_CLASS L"FrameworkClass"

#define DEFAULT_WINDOW_WIDTH 1280;
#define DEFAULT_WINDOW_HEIGHT 720;

class Application
{
public:
	Application(wstring title, UINT width, UINT height) : m_windowTitle(title.c_str()), m_windowClass((title + L"Class").c_str()), m_windowWidth(width), m_windowHeight(height) {  };
	~Application() { Release(); };

	HRESULT Init(HINSTANCE hInstance, int nCmdShow);

	void Draw();
	void Update(float deltaTime);
	void Release();

	Camera* m_pCamera = nullptr;

private:

	HWND m_hWnd = nullptr;
	HINSTANCE m_hInst = nullptr;
	LPCWSTR m_windowTitle = DEFAULT_WINDOW_CLASS;
	LPCWSTR m_windowClass = DEFAULT_WINDOW_TITLE;
	UINT m_windowWidth = DEFAULT_WINDOW_WIDTH;
	UINT m_windowHeight = DEFAULT_WINDOW_HEIGHT;

	//Initilization functions
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);

	//Camera2* m_pCamera = nullptr;
	Camera2 m_Camera2;

	// Graphics
	Graphics* m_pGraphics = nullptr;

	//GameObject
	DrawableGameObject* m_pGameObject = nullptr;

	//DirectInput
	DirectInput* m_pDirectInput = nullptr;

	XMMATRIX g_World1;
	XMMATRIX g_View;
	XMMATRIX g_Projection;


};
