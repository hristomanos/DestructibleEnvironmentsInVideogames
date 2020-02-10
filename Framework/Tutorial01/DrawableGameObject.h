#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include "DDSTextureLoader.h"
#include "resource.h"
#include <iostream>
#include "structures.h"

using namespace DirectX;

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 TexCoord;
	XMFLOAT3 tangent;
	XMFLOAT3 biTangent;
};

class DrawableGameObject
{
public:
	DrawableGameObject();
	~DrawableGameObject();

	HRESULT Init(ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext);
	HRESULT initMesh(ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext);
	void Update(float t);
	void Draw(ID3D11DeviceContext *pContext);
	ID3D11Buffer *getVertexBuffer() { return m_pVertexBuffer; }
	ID3D11Buffer *getIndexBuffer() { return m_pIndexBuffer; }
	ID3D11ShaderResourceView **getTextureResourceView() { return &m_pTextureResourceView; }
	XMMATRIX *getTransform() { return &m_World; }
	ID3D11SamplerState **getTextureSamplerState() { return &m_pSamplerLinear; }
	MaterialPropertiesConstantBuffer getMaterial() { return m_material; }
	void setPosition(XMFLOAT3 position);

private:
	XMMATRIX m_World;

	ID3D11Buffer *m_pVertexBuffer;
	ID3D11Buffer *m_pIndexBuffer;
	ID3D11ShaderResourceView *m_pTextureResourceView;
	ID3D11SamplerState *m_pSamplerLinear;
	MaterialPropertiesConstantBuffer m_material;
	XMFLOAT3 m_position;

	void CalculateModelVectors(SimpleVertex *vertices, int vertexCount);
	void CalculateTangentBinormal2(SimpleVertex v0, SimpleVertex v1, SimpleVertex v2, XMFLOAT3 &normal, XMFLOAT3 &tangent, XMFLOAT3 &binormal);

	ID3D11VertexShader* g_pVertexShader = nullptr;

	ID3D11PixelShader* g_pPixelShader = nullptr;
	ID3D11ShaderResourceView* g_pTextureArr[3]{ nullptr, nullptr };
};
