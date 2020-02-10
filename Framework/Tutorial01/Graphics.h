#pragma once

#include <d3d11_1.h>
#include "Utilities/Helper.h"
#include "Resource.h"
#include <directxmath.h>
#include "structures.h"
#include "Camera2.h"
#include "DrawableGameObject.h"

#define DEFAULT_WINDOW_TITLE L"Framework"
#define DEFAULT_WINDOW_CLASS L"FrameworkClass"

#define DEFAULT_WINDOW_WIDTH 1280;
#define DEFAULT_WINDOW_HEIGHT 720;


class Graphics
{
public:
	~Graphics() { Release(); };

	HRESULT Init(HWND hWnd);
	void Draw();
	void Release();

	ID3D11Device* Getd3dDevice() { return m_pd3dDevice; };
	ID3D11Device1* Getd3dDevice1() { return m_pd3dDevice1; };
	ID3D11DeviceContext* GetImmediateContext() { return m_pImmediateContext; };
	ID3D11DeviceContext1* GetImmediateContext1() { return m_pImmediateContext1; };
	IDXGISwapChain* GetSwapChain() { return m_pSwapChain; };
	IDXGISwapChain1* GetSwapChain1() { return m_pSwapChain1; };
	ID3D11RenderTargetView* GetRenderTargetView() { return m_pRenderTargetView; };
	ID3D11Texture2D* GetDepthStencil() { return m_pDepthStencil; };
	ID3D11DepthStencilView* GetDepthStencilView() { return m_pDepthStencilView; };

	ID3D11Buffer* GetVertexBuffer() { return m_pVertexBuffer; };
	ID3D11Buffer* GetIndexBuffer() { return m_pIndexBuffer; };
	ID3D11Buffer* GetConstantBuffer() { return m_pConstantBuffer; };
	ID3D11Buffer* GetMaterialConstantBuffer() { return m_pMaterialConstantBuffer; };
	ID3D11Buffer* GetLightConstantBuffer() { return m_pLightConstantBuffer; };

	ID3D11SamplerState* GetSamplerLinear() { return m_pSamplerLinear; };
	ID3D11SamplerState* GetSamplerNormal() { return m_pSamplerNormal; };
	
	void Setd3dDevice(ID3D11Device *pd3dDevice) { m_pd3dDevice = pd3dDevice; };
	void Setd3dDevice1(ID3D11Device1 *pd3dDevice1) { m_pd3dDevice1 = pd3dDevice1; };
	void SetImmediateContext(ID3D11DeviceContext *pImmediateContext) { m_pImmediateContext = pImmediateContext; };
	void SetImmediateContext1(ID3D11DeviceContext1 *pImmediateContext1) { m_pImmediateContext1 = pImmediateContext1; };
	void SetSwapChain(IDXGISwapChain *pSwapChain) { m_pSwapChain = pSwapChain; };
	void SetSwapChain1(IDXGISwapChain1 *pSwapChain1) { m_pSwapChain1 = pSwapChain1; };
	void SetRenderTargetView(ID3D11RenderTargetView *pRenderTargetView) { m_pRenderTargetView = pRenderTargetView; };
	void SetDepthStencil(ID3D11Texture2D *pDepthStencil) { m_pDepthStencil = pDepthStencil; };
	void SetDepthStencilView(ID3D11DepthStencilView *pDepthStencilView) { m_pDepthStencilView = pDepthStencilView; };

	void SetVertexBuffer(ID3D11Buffer *pVertexBuffer) { m_pVertexBuffer = pVertexBuffer; };
	void SetIndexBuffer(ID3D11Buffer *pIndexBuffer) { m_pIndexBuffer = pIndexBuffer; };
	void SetConstantBuffer(ID3D11Buffer *pConstantBuffer) { m_pConstantBuffer = pConstantBuffer; };
	void SetMaterialConstantBuffer(ID3D11Buffer *pMaterialConstantBuffer) { m_pMaterialConstantBuffer = pMaterialConstantBuffer; };
	void SetLightConstantBuffer(ID3D11Buffer *pLightConstantBuffer) { m_pLightConstantBuffer = pLightConstantBuffer; };

	void SetSamplerLinear(ID3D11SamplerState *pSamplerLinear) { m_pSamplerLinear = pSamplerLinear; };
	void SetSamplerNormal(ID3D11SamplerState *pSamplerNormal) { m_pSamplerNormal = pSamplerNormal; };

	HRESULT InitDevice(HWND hwnd);
	



	XMMATRIX g_World1;
	XMMATRIX g_View;
	XMMATRIX g_Projection;

	XMFLOAT4 g_EyePosition = XMFLOAT4(0.0f, 2.0f, -5, 1.0f);

	Camera2 m_Camera;

	DrawableGameObject* m_pDrawableGameObject;


private:
	D3D_DRIVER_TYPE m_driverType;
	D3D_FEATURE_LEVEL m_featureLevel;

	ID3D11Device *m_pd3dDevice = nullptr;
	ID3D11Device1 *m_pd3dDevice1 = nullptr;
	ID3D11DeviceContext *m_pImmediateContext = nullptr;
	ID3D11DeviceContext1 *m_pImmediateContext1 = nullptr;
	IDXGISwapChain *m_pSwapChain = nullptr;
	IDXGISwapChain1 *m_pSwapChain1 = nullptr;
	ID3D11RenderTargetView*  m_pRenderTargetView = nullptr;
	ID3D11Texture2D *m_pDepthStencil = nullptr;
	ID3D11DepthStencilView *m_pDepthStencilView = nullptr;

	ID3D11Buffer *m_pVertexBuffer = nullptr;
	ID3D11Buffer *m_pIndexBuffer = nullptr;
	ID3D11Buffer *m_pConstantBuffer = nullptr;
	ID3D11Buffer *m_pMaterialConstantBuffer = nullptr;
	ID3D11Buffer *m_pLightConstantBuffer = nullptr;

	ID3D11SamplerState *m_pSamplerLinear = nullptr;
	ID3D11SamplerState *m_pSamplerNormal = nullptr;





};
