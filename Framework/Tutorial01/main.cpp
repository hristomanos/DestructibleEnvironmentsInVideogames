//--------------------------------------------------------------------------------------
// File: main.cpp
//
// This application demonstrates animation using matrix transformations
//
// http://msdn.microsoft.com/en-us/library/windows/apps/ff729722.aspx
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#define _XM_NO_INTRINSICS_

#include "main.h"

DirectX::XMFLOAT4 g_EyePosition(0.0f, 2.0f, -5, 1.0f);

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
//HRESULT		InitDevice();
//HRESULT		InitMesh();
HRESULT InitWorld(int width, int height);
void CleanupDevice();
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Render();

void HandleKeyboardInput(float dt);
void OnMouseDown(WPARAM btnState, int x, int y);
void OnMouseUp(WPARAM btnState, int x, int y);
void OnMouseMove(WPARAM btnState, int x, int y);

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
//HINSTANCE g_hInst = nullptr;
//HWND g_hWnd = nullptr;
//D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
//D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
//ID3D11Device*           g_pd3dDevice = nullptr;
//ID3D11Device1*          g_pd3dDevice1 = nullptr;
//ID3D11DeviceContext*    g_pImmediateContext = nullptr;
//ID3D11DeviceContext1*   g_pImmediateContext1 = nullptr;
//IDXGISwapChain*         g_pSwapChain = nullptr;
//IDXGISwapChain1*        g_pSwapChain1 = nullptr;
//ID3D11RenderTargetView* g_pRenderTargetView = nullptr;
//ID3D11Texture2D*        g_pDepthStencil = nullptr;
//ID3D11DepthStencilView* g_pDepthStencilView = nullptr;
//ID3D11VertexShader *g_pVertexShader = nullptr;

//ID3D11PixelShader *g_pPixelShader = nullptr;
ID3D11PixelShader *g_pPixelShaderSolid = nullptr;

ID3D11InputLayout *g_pVertexLayout = nullptr;
//ID3D11Buffer*           g_pVertexBuffer = nullptr;
//ID3D11Buffer*           g_pIndexBuffer = nullptr;
//ID3D11Buffer*           g_pConstantBuffer = nullptr;
//ID3D11Buffer*           g_pMaterialConstantBuffer = nullptr;
//ID3D11Buffer*           g_pLightConstantBuffer = nullptr;

ID3D11ShaderResourceView *g_pTextureRV = nullptr;
ID3D11ShaderResourceView *g_pTextureArr[3]{nullptr, nullptr};

//ID3D11SamplerState *	g_pSamplerLinear = nullptr;
//ID3D11SamplerState *	g_pSamplerNormal = nullptr;

XMMATRIX g_World1;
XMMATRIX g_View;
XMMATRIX g_Projection;

//int g_viewWidth;
//int g_viewHeight;

//DrawableGameObject g_GameObject;

POINT g_LastMousePos;

Camera2 g_Camera;

#define DEFAULT_WINDOW_TITLE L"Framework"
#define MS_PER_UPDATE 16.66666666667
#define APPLICATION_WINDOW_WIDTH 1280;
#define APPLICATION_WINDOW_HEIGHT 720;

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	/*if (FAILED(InitWindow(hInstance, nCmdShow)))
		return 0;

	if (FAILED(InitDevice()))
	{
		CleanupDevice();
		return 0;
	}*/

	//Create an application instance
	Application* application = new Application(DEFAULT_WINDOW_TITLE, 1280, 720);
	if (FAILED(application->Init(hInstance, nCmdShow)))
		return -1;

	// Main message loop
	MSG msg = {0};

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			application->Update(MS_PER_UPDATE / 1000.0f);
			application->Draw();

		}
	}

	application->Release();

	delete application;
	application = nullptr;

	return (int)msg.wParam;
}

//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
//HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
//{
//	// Register class
//	WNDCLASSEX wcex;
//	wcex.cbSize = sizeof(WNDCLASSEX);
//	wcex.style = CS_HREDRAW | CS_VREDRAW;
//	wcex.lpfnWndProc = WndProc;
//	wcex.cbClsExtra = 0;
//	wcex.cbWndExtra = 0;
//	wcex.hInstance = hInstance;
//	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
//	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
//	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//	wcex.lpszMenuName = nullptr;
//	wcex.lpszClassName = L"TutorialWindowClass";
//	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
//	if (!RegisterClassEx(&wcex))
//		return E_FAIL;
//
//	// Create window
//	g_hInst = hInstance;
//	RECT rc = {0, 0, 1280, 720};
//
//	g_viewWidth = 1280;
//	g_viewHeight = 720;
//
//	g_LastMousePos.x = 0;
//	g_LastMousePos.y = 0;
//
//	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
//	g_hWnd = CreateWindow(L"TutorialWindowClass", L"Direct3D 11 Tutorial 5",
//						  WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
//						  CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
//						  nullptr);
//	if (!g_hWnd)
//		return E_FAIL;
//
//	ShowWindow(g_hWnd, nCmdShow);
//
//	return S_OK;
//}

//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DCompile
//
// With VS 11, we could load up prebuilt .cso files instead...
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile(const WCHAR *szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob **ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows
	// the shaders to be optimized and to run exactly the way they will run in
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob *pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
							dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugStringA(reinterpret_cast<const char *>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob)
		pErrorBlob->Release();

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
//HRESULT InitDevice()
//{
//	HRESULT hr = S_OK;
//
//	RECT rc;
//	//GetClientRect(g_hWnd, &rc);
//	UINT width = rc.right - rc.left;
//	UINT height = rc.bottom - rc.top;
//
//	UINT createDeviceFlags = 0;
//#ifdef _DEBUG
//	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
//#endif
//
//	D3D_DRIVER_TYPE driverTypes[] =
//		{
//			D3D_DRIVER_TYPE_HARDWARE,
//			D3D_DRIVER_TYPE_WARP,
//			D3D_DRIVER_TYPE_REFERENCE,
//		};
//	UINT numDriverTypes = ARRAYSIZE(driverTypes);
//
//	D3D_FEATURE_LEVEL featureLevels[] =
//		{
//			D3D_FEATURE_LEVEL_11_1,
//			D3D_FEATURE_LEVEL_11_0,
//			D3D_FEATURE_LEVEL_10_1,
//			D3D_FEATURE_LEVEL_10_0,
//		};
//	UINT numFeatureLevels = ARRAYSIZE(featureLevels);
//
//	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
//	{
//		//g_driverType = driverTypes[driverTypeIndex];
//		//hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
//							  // D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
//
//		if (hr == E_INVALIDARG)
//		{
//			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
//			hr = D3D11CreateDevice(nullptr, g_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
//								   D3D11_SDK_VERSION, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
//		}
//
//		if (SUCCEEDED(hr))
//			break;
//	}
//	if (FAILED(hr))
//		return hr;
//
//	// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
//	IDXGIFactory1 *dxgiFactory = nullptr;
//	{
//		IDXGIDevice *dxgiDevice = nullptr;
//		hr = g_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void **>(&dxgiDevice));
//		if (SUCCEEDED(hr))
//		{
//			IDXGIAdapter *adapter = nullptr;
//			hr = dxgiDevice->GetAdapter(&adapter);
//			if (SUCCEEDED(hr))
//			{
//				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void **>(&dxgiFactory));
//				adapter->Release();
//			}
//			dxgiDevice->Release();
//		}
//	}
//	if (FAILED(hr))
//		return hr;
//
//	// Create swap chain
//	IDXGIFactory2 *dxgiFactory2 = nullptr;
//	hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void **>(&dxgiFactory2));
//	if (dxgiFactory2)
//	{
//		// DirectX 11.1 or later
//		hr = g_pd3dDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void **>(&g_pd3dDevice1));
//		if (SUCCEEDED(hr))
//		{
//			(void)g_pImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void **>(&g_pImmediateContext1));
//		}
//
//		DXGI_SWAP_CHAIN_DESC1 sd = {};
//		sd.Width = width;
//		sd.Height = height;
//		sd.Format = DXGI_FORMAT_R16G16B16A16_FLOAT; //  DXGI_FORMAT_R16G16B16A16_FLOAT;////DXGI_FORMAT_R8G8B8A8_UNORM;
//		sd.SampleDesc.Count = 1;
//		sd.SampleDesc.Quality = 0;
//		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//		sd.BufferCount = 1;
//
//		hr = dxgiFactory2->CreateSwapChainForHwnd(g_pd3dDevice, g_hWnd, &sd, nullptr, nullptr, &g_pSwapChain1);
//		if (SUCCEEDED(hr))
//		{
//			hr = g_pSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void **>(&g_pSwapChain));
//		}
//
//		dxgiFactory2->Release();
//	}
//	else
//	{
//		// DirectX 11.0 systems
//		DXGI_SWAP_CHAIN_DESC sd = {};
//		sd.BufferCount = 1;
//		sd.BufferDesc.Width = width;
//		sd.BufferDesc.Height = height;
//		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//		sd.BufferDesc.RefreshRate.Numerator = 60;
//		sd.BufferDesc.RefreshRate.Denominator = 1;
//		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//		sd.OutputWindow = g_hWnd;
//		sd.SampleDesc.Count = 1;
//		sd.SampleDesc.Quality = 0;
//		sd.Windowed = TRUE;
//
//		hr = dxgiFactory->CreateSwapChain(g_pd3dDevice, &sd, &g_pSwapChain);
//	}
//
//	// Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
//	dxgiFactory->MakeWindowAssociation(g_hWnd, DXGI_MWA_NO_ALT_ENTER);
//
//	dxgiFactory->Release();
//
//	if (FAILED(hr))
//		return hr;
//
//	// Create a render target view
//	ID3D11Texture2D *pBackBuffer = nullptr;
//	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&pBackBuffer));
//	if (FAILED(hr))
//		return hr;
//
//	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_pRenderTargetView);
//	pBackBuffer->Release();
//	if (FAILED(hr))
//		return hr;
//
//	// Create depth stencil texture
//	D3D11_TEXTURE2D_DESC descDepth = {};
//	descDepth.Width = width;
//	descDepth.Height = height;
//	descDepth.MipLevels = 1;
//	descDepth.ArraySize = 2;
//	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
//	descDepth.SampleDesc.Count = 1;
//	descDepth.SampleDesc.Quality = 0;
//	descDepth.Usage = D3D11_USAGE_DEFAULT;
//	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
//	descDepth.CPUAccessFlags = 0;
//	descDepth.MiscFlags = 0;
//	hr = g_pd3dDevice->CreateTexture2D(&descDepth, nullptr, &g_pDepthStencil);
//	if (FAILED(hr))
//		return hr;
//
//	// Create the depth stencil view
//	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
//	descDSV.Format = descDepth.Format;
//	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
//	descDSV.Texture2D.MipSlice = 0;
//	hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);
//	if (FAILED(hr))
//		return hr;
//
//	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, g_pDepthStencilView);
//
//	// Setup the viewport
//	D3D11_VIEWPORT vp;
//	vp.Width = (FLOAT)width;
//	vp.Height = (FLOAT)height;
//	vp.MinDepth = 0.0f;
//	vp.MaxDepth = 1.0f;
//	vp.TopLeftX = 0;
//	vp.TopLeftY = 0;
//	g_pImmediateContext->RSSetViewports(1, &vp);
//
//	hr = InitMesh();
//	if (FAILED(hr))
//	{
//		MessageBox(nullptr,
//				   L"Failed to initialise mesh.", L"Error", MB_OK);
//		return hr;
//	}
//
//	hr = InitWorld(width, height);
//	if (FAILED(hr))
//	{
//		MessageBox(nullptr,
//				   L"Failed to initialise world.", L"Error", MB_OK);
//		return hr;
//	}
//
//	hr = g_GameObject.initMesh(g_pd3dDevice, g_pImmediateContext);
//	if (FAILED(hr))
//		return hr;
//
//	return S_OK;
//}

// ***************************************************************************************
// InitMesh
// ***************************************************************************************
//HRESULT InitMesh()
//{
//	// Compile the vertex shader
//	ID3DBlob *pVSBlob = nullptr;
//	HRESULT hr = CompileShaderFromFile(L"shader.fx", "VS", "vs_4_0", &pVSBlob);
//	if (FAILED(hr))
//	{
//		MessageBox(nullptr,
//				   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
//		return hr;
//	}
//
//	// Create the vertex shader
//	hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader);
//	if (FAILED(hr))
//	{
//		pVSBlob->Release();
//		return hr;
//	}
//
//	// Define the input layout
//	D3D11_INPUT_ELEMENT_DESC layout[] =
//		{
//			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
//			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
//			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
//			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
//			{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
//		};
//	UINT numElements = ARRAYSIZE(layout);
//
//	// Create the input layout
//	hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
//										 pVSBlob->GetBufferSize(), &g_pVertexLayout);
//	pVSBlob->Release();
//	if (FAILED(hr))
//		return hr;
//
//	// Set the input layout
//	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);
//
//	// Compile the pixel shader
//	ID3DBlob *pPSBlob = nullptr;
//	hr = CompileShaderFromFile(L"shader.fx", "PS", "ps_4_0", &pPSBlob);
//	if (FAILED(hr))
//	{
//		MessageBox(nullptr,
//				   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
//		return hr;
//	}
//
//	// Create the pixel shader
//	hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader);
//	pPSBlob->Release();
//	if (FAILED(hr))
//		return hr;
//
//	// Compile the SOLID pixel shader
//	pPSBlob = nullptr;
//	hr = CompileShaderFromFile(L"shader.fx", "PSSolid", "ps_4_0", &pPSBlob);
//	if (FAILED(hr))
//	{
//		MessageBox(nullptr,
//				   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
//		return hr;
//	}
//
//	// Create the SOLID pixel shader
//	hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShaderSolid);
//	pPSBlob->Release();
//	if (FAILED(hr))
//		return hr;
//
//	// Create vertex buffer
//	SimpleVertex vertices[] =
//		{
//			{XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//			{XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//			{XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//			{XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//
//			{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//			{XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//			{XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//			{XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//
//			{XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//			{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//			{XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//			{XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//
//			{XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//			{XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//			{XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//			{XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//
//			{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//			{XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//			{XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//			{XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//
//			{XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//			{XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//			{XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//			{XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
//		};
//
//	D3D11_BUFFER_DESC bd = {};
//	bd.Usage = D3D11_USAGE_DEFAULT;
//	bd.ByteWidth = sizeof(SimpleVertex) * 24;
//	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bd.CPUAccessFlags = 0;
//
//	D3D11_SUBRESOURCE_DATA InitData = {};
//	InitData.pSysMem = vertices;
//	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
//	if (FAILED(hr))
//		return hr;
//
//	// Set vertex buffer
//	UINT stride = sizeof(SimpleVertex);
//	UINT offset = 0;
//	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
//
//	// Create index buffer
//	WORD indices[] =
//		{
//			3, 1, 0,
//			2, 1, 3,
//
//			6, 4, 5,
//			7, 4, 6,
//
//			11, 9, 8,
//			10, 9, 11,
//
//			14, 12, 13,
//			15, 12, 14,
//
//			19, 17, 16,
//			18, 17, 19,
//
//			22, 20, 21,
//			23, 20, 22};
//
//	bd.Usage = D3D11_USAGE_DEFAULT;
//	bd.ByteWidth = sizeof(WORD) * 36; // 36 vertices needed for 12 triangles in a triangle list
//	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
//	bd.CPUAccessFlags = 0;
//	InitData.pSysMem = indices;
//	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
//	if (FAILED(hr))
//		return hr;
//
//	// Set index buffer
//	g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
//
//	// Set primitive topology
//	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	// Create the constant buffer
//	bd.Usage = D3D11_USAGE_DEFAULT;
//	bd.ByteWidth = sizeof(ConstantBuffer);
//	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	bd.CPUAccessFlags = 0;
//	hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pConstantBuffer);
//	if (FAILED(hr))
//		return hr;
//
//	// Create the material constant buffer
//	bd.Usage = D3D11_USAGE_DEFAULT;
//	bd.ByteWidth = sizeof(MaterialPropertiesConstantBuffer);
//	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	bd.CPUAccessFlags = 0;
//	hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pMaterialConstantBuffer);
//	if (FAILED(hr))
//		return hr;
//
//	// Create the light constant buffer
//	bd.Usage = D3D11_USAGE_DEFAULT;
//	bd.ByteWidth = sizeof(LightPropertiesConstantBuffer);
//	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	bd.CPUAccessFlags = 0;
//	hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pLightConstantBuffer);
//	if (FAILED(hr))
//		return hr;
//
//	// load and setup textures
//	// HRESULT texturesArray[2] = {
//	//	CreateDDSTextureFromFile(g_pd3dDevice, L"Resources\\stone.dds", nullptr, &g_pTextureRV),
//	//	CreateDDSTextureFromFile(g_pd3dDevice, L"Resources\\conenormal.dds", nullptr, &g_pNormalTextureRV)
//	//};
//
//	//hr = texturesArray[1];
//	hr = CreateDDSTextureFromFile(g_pd3dDevice, L"Resources\\color.dds", nullptr, &g_pTextureArr[0]);
//	hr = CreateDDSTextureFromFile(g_pd3dDevice, L"Resources\\normals.dds", nullptr, &g_pTextureArr[1]);
//	hr = CreateDDSTextureFromFile(g_pd3dDevice, L"Resources\\displacement.dds", nullptr, &g_pTextureArr[2]);
//	if (FAILED(hr))
//		return hr;
//
//	D3D11_SAMPLER_DESC sampDesc;
//	ZeroMemory(&sampDesc, sizeof(sampDesc));
//	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
//	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
//	sampDesc.MinLOD = 0;
//	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
//	hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
//
//	return hr;
//}

// ***************************************************************************************
// InitWorld
// ***************************************************************************************
HRESULT InitWorld(int width, int height)
{
	// Initialize the world matrix
	g_World1 = XMMatrixIdentity();

	// Initialize the view matrix
	XMVECTOR Eye = XMLoadFloat4(&g_EyePosition);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);

	// Initialize the projection matrix
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / (FLOAT)height, 0.01f, 100.0f);

	// Initialize Camera lens
	XMFLOAT3 eye = XMFLOAT3(0.0f, 2.0f, -5.0f);
	XMFLOAT3 at = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);

	//_camera = new Camera(eye, at, up, width, width, 0.01f, 200.0f);
	g_Camera.SetPosition(0.0f, 0.0f, -2.0f);
	g_Camera.SetProjectionValues(90.0f, static_cast<float>(width) / static_cast<float>(height), 0.01f, 1000.0f);
	//static_cast<float>(width) / static_cast<float>(height)

	

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
//void CleanupDevice()
//{
//	if (g_pImmediateContext)
//		g_pImmediateContext->ClearState();
//
//	if (g_pConstantBuffer)
//		g_pConstantBuffer->Release();
//	if (g_pVertexBuffer)
//		g_pVertexBuffer->Release();
//	if (g_pIndexBuffer)
//		g_pIndexBuffer->Release();
//	if (g_pVertexLayout)
//		g_pVertexLayout->Release();
//	if (g_pVertexShader)
//		g_pVertexShader->Release();
//	if (g_pPixelShader)
//		g_pPixelShader->Release();
//	if (g_pDepthStencil)
//		g_pDepthStencil->Release();
//	if (g_pDepthStencilView)
//		g_pDepthStencilView->Release();
//	if (g_pRenderTargetView)
//		g_pRenderTargetView->Release();
//	if (g_pSwapChain1)
//		g_pSwapChain1->Release();
//	if (g_pSwapChain)
//		g_pSwapChain->Release();
//	if (g_pImmediateContext1)
//		g_pImmediateContext1->Release();
//	if (g_pImmediateContext)
//		g_pImmediateContext->Release();
//	if (g_pd3dDevice1)
//		g_pd3dDevice1->Release();
//	if (g_pd3dDevice)
//		g_pd3dDevice->Release();
//}

//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		//OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	}
	case WM_MOUSEMOVE:
	{
		OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	}
	case WM_LBUTTONUP:
	{
		OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

		// Note that this tutorial does not handle resizing (WM_SIZE) requests,
		// so we created the window without the resize border.

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//--------------------------------------------------------------------------------------
// Render a frame
//--------------------------------------------------------------------------------------
void Render()
{
	// Update our time
	/*static float t = 0.0f;
	if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += (float)XM_PI * 0.0125f;
	}
	else
	{
		static ULONGLONG timeStart = 0;
		ULONGLONG timeCur = GetTickCount64();
		if (timeStart == 0)
			timeStart = timeCur;
		t = (timeCur - timeStart) / 1000.0f;
	}*/

	// Clear the back buffer
	//g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, Colors::MidnightBlue);

	// Clear the depth buffer to 1.0 (max depth)
	//g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//HandleKeyboardInput(t);

	/*XMFLOAT4X4 viewAsFloats = _camera->GetView();
	XMFLOAT4X4 projectionAsFloats = _camera->GetProjection();

	XMMATRIX view = XMLoadFloat4x4(&viewAsFloats);
	XMMATRIX projection = XMLoadFloat4x4(&projectionAsFloats);*/

	// Update variables for a cube
	//g_GameObject.update(t);

	// Update variables for the cube
	//XMMATRIX *mGO = g_GameObject.getTransform();

	//ConstantBuffer cb1;
	//cb1.mWorld = XMMatrixTranspose(*mGO);
	//cb1.mView = XMMatrixTranspose(g_Camera.GetViewMatrix());			 //XMMatrixTranspose( g_View );
	//cb1.mProjection = XMMatrixTranspose(g_Camera.GetProjectionMatrix()); //XMMatrixTranspose( g_Projection );
	//cb1.vOutputColor = XMFLOAT4(0, 0, 0, 0);
	//g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb1, 0, 0);

	//MaterialPropertiesConstantBuffer redPlasticMaterial;
	//redPlasticMaterial.Material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//redPlasticMaterial.Material.Specular = XMFLOAT4(1.0f, 0.2f, 0.2f, 1.0f);
	//redPlasticMaterial.Material.SpecularPower = 32.0f;
	//redPlasticMaterial.Material.UseTexture = true;
	//redPlasticMaterial.Material.UseBumpMap = true;
	//g_pImmediateContext->UpdateSubresource(g_pMaterialConstantBuffer, 0, nullptr, &redPlasticMaterial, 0, 0);

	//Light light;
	//light.Enabled = static_cast<int>(true);
	//light.LightType = PointLight;
	//light.Color = XMFLOAT4(Colors::White);
	//light.SpotAngle = XMConvertToRadians(45.0f);
	//light.ConstantAttenuation = 1.0f;
	//light.LinearAttenuation = 1;
	//light.QuadraticAttenuation = 1;

	//// set up the light
	//XMFLOAT3 cameraPosition = g_Camera.GetPositionFloat3();
	//XMFLOAT4 LightPosition(cameraPosition.x, cameraPosition.y, cameraPosition.z, 1);
	//light.Position = LightPosition;
	//XMVECTOR LightDirection = XMVectorSet(-LightPosition.x, -LightPosition.y, -LightPosition.z, 0.0f);
	//LightDirection = XMVector3Normalize(LightDirection);
	//XMStoreFloat4(&light.Direction, LightDirection);

	//LightPropertiesConstantBuffer lightProperties;
	//lightProperties.EyePosition = LightPosition;
	//lightProperties.Lights[0] = light;
	//g_pImmediateContext->UpdateSubresource(g_pLightConstantBuffer, 0, nullptr, &lightProperties, 0, 0);

	// Render the cube
	//g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
	//g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	//g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);

	//g_pImmediateContext->PSSetConstantBuffers(1, 1, &g_pMaterialConstantBuffer);
	//g_pImmediateContext->PSSetConstantBuffers(2, 1, &g_pLightConstantBuffer);

	//g_pImmediateContext->PSSetShaderResources(0, 2, g_pTextureArr);
	//g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);

	//g_pImmediateContext->DrawIndexed(36, 0, 0); // Move it to drawable gameobject class

	// Present our back buffer to our front buffer
	//g_pSwapChain->Present(0, 0);
}

void HandleKeyboardInput(float dt)
{
	//
	// Control the camera.
	//
	//XMFLOAT3 cameraPos = _camera->GetPosition();
	const float cameraSpeed = 0.001f;

	if (GetAsyncKeyState('W') & 0x8000)
	{
		//_camera->TranslateByAmount(0.0f, 0.0f, 0.001f);
		g_Camera.AdjustPosition(0.0f, 0.0f, 0.001f);
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		//g_Camera.Walk(-1.0f * dt);
		//g_EyePosition.z -= 1.0f;
		//cameraPos.z -= 0.1f * dt;
		//_camera->TranslateByAmount(0.0f, 0.0f, -0.001f);
		g_Camera.AdjustPosition(0.0f, 0.0f, -0.001f);
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		//g_Camera.Strafe(-1.0f * dt);
		//g_EyePosition.x -= 1.0f;
		//cameraPos.x -= 0.1f * dt;
		//_camera->TranslateByAmount(-0.001f, 0.0f, 0.0f);
		g_Camera.AdjustPosition(-0.001f, 0.0f, 0.0f);
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		//g_Camera.Strafe(1.0f * dt);
		//g_EyePosition.x += 1.0f;
		//cameraPos.x += 0.1f * dt;
		//_camera->TranslateByAmount(0.001f, 0.0f, 0.0f);
		g_Camera.AdjustPosition(0.001f, 0.0f, 0.0f);
	}

	if (GetAsyncKeyState('E'))
	{
		g_Camera.AdjustRotation(0.0f, 0.001f, 0);
	}

	if (GetAsyncKeyState('Q'))
	{
		g_Camera.AdjustRotation(0.0f, -0.001f, 0);
	}
}

void OnMouseDown(WPARAM btnState, int x, int y)
{
	g_LastMousePos.x = x;
	g_LastMousePos.y = y;

	//SetCapture(g_hWnd);
}

void OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - g_LastMousePos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - g_LastMousePos.y));

		//g_Camera.Pitch(dy);
		//g_Camera.RotateY(dx);
		g_Camera.AdjustRotation(dy, dx, 0);
	}

	g_LastMousePos.x = x;
	g_LastMousePos.y = y;
}
