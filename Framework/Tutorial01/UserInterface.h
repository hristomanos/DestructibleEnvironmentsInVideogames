#pragma once


#include "Utilities/Helper.h"
#include "ImGui\\imgui.h"
#include "ImGui\\imgui_internal.h"
#include "ImGui\\imgui_impl_dx11.h"
#include "ImGui\\imgui_impl_win32.h"
#include <directxmath.h>
#include "structures.h"


#include "Graphics.h"
#include "DrawableGameObject.h"
#include "DirectInput.h"

class UserInterface
{
public:
	UserInterface(Graphics* graphics, DrawableGameObject* drawableGameObject);
	~UserInterface();

	void Init(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void Render(bool rayHit);

	Graphics* m_pGraphics;
	DrawableGameObject* m_pDrawableGameObject;
	void LightWindow();
	void CubeWindow();
	
	void ShowOverlay();
};

