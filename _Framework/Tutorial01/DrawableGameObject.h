#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include "DDSTextureLoader.h"
#include "resource.h"
#include <iostream>
#include "structures.h"

#include "Utilities/Helper.h"

#include <vector>

using namespace DirectX;

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 TexCoord;
};

enum BlockType
{
	BlockType_Default = 0,
	BlockType_Grass,
	BlockType_Dirt,
	BlockType_Water,
	BlockType_Stone,
	BlockType_Wood,
	BlockType_Sand,
	BlockType_NumTypes,
};

enum BlockFace
{
	BACK,
	FRONT,
	TOP,
	BOTTOM,
	LEFT,
	RIGHT,
	NumFaces,
};

class DrawableGameObject
{
public:
	DrawableGameObject();
	~DrawableGameObject() { Release(); };

	HRESULT							Init(ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext);
	HRESULT							InitMesh(ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext);

	void							Update(float t);
	void							Draw(ID3D11DeviceContext *pContext);
	void							Release();

	ID3D11Buffer*					GetVertexBuffer()		 { return m_pVertexBuffer; }
	ID3D11Buffer*					GetIndexBuffer()		 { return m_pIndexBuffer; }

	ID3D11ShaderResourceView**		GetTextureResourceView() { return &m_pTextureResourceView; }

	XMFLOAT4X4*						GetTransform()			 { return &m_World; }
	ID3D11SamplerState**			GetTextureSamplerState() { return &m_pSamplerLinear; }

	void							SetPosition(XMFLOAT3 position);
	XMFLOAT3						GetPosition()			 { return m_position; }

	ID3D11ShaderResourceView*		GetTextureArr(int val)	 { return g_pTextureArr[val]; }

	ID3D11ShaderResourceView*		g_pTextureArr[2]{ nullptr, nullptr };

	//Blocks
	bool							isActive() { return m_IsActive; }
	void							SetActive(bool active) { m_IsActive = active; }

	bool							isEmptyBlock() { return m_IsEmptyBlock; }
	void							SetEmptyBlock(bool IsEmpty) { m_IsEmptyBlock = IsEmpty; }


	//Picking
	std::vector<XMFLOAT3>			GetVertPosArray() { return m_VertPosArray; }
	std::vector<WORD>				GetIndexPosArray() { return m_IndexPosArray; }


	void UpdateIndices(BlockFace);

private:


	XMFLOAT4X4 m_World;

	ID3D11InputLayout*					m_pVertexLayout = nullptr;
	ID3D11Buffer*						m_pVertexBuffer = nullptr;
	ID3D11Buffer*						m_pIndexBuffer = nullptr;
	ID3D11Buffer*						m_pMaterialConstantBuffer = nullptr;

	ID3D11ShaderResourceView*			m_pTextureResourceView = nullptr;
	ID3D11SamplerState *				m_pSamplerLinear = nullptr;
//	MaterialPropertiesConstantBuffer    m_material;
	XMFLOAT3							m_position = XMFLOAT3(0,0,0);


	ID3D11VertexShader*					m_pVertexShader = nullptr;
    ID3D11PixelShader*					m_pPixelShader = nullptr;
	ID3D11PixelShader*				    m_pPixelShaderSolid = nullptr;

	
	//Blocks
	bool								m_IsActive = true;
	BlockType							m_BlockType;
	bool								m_IsEmptyBlock = false;

	//Picking
	std::vector<XMFLOAT3>				m_VertPosArray;
	std::vector<WORD>					m_IndexPosArray;

	//Instancing
	const int							m_NumCubes = 32;
	ConstantBufferPerScene*				cbPerInst;
	ID3D11Buffer*						m_cbPerInstanceBuffer = nullptr;
	ID3D11Buffer*						m_CubesInstanceBuffer = nullptr;


	HRESULT								CreateVertexlShader(ID3DBlob*& pVSBlob,ID3D11Device* pDevice);
	HRESULT								CreateInputLayout(ID3DBlob*& pVSBlob, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	HRESULT								CreatePixelShader(ID3D11Device* pDevice);

	vector<WORD>m_Indices;
	
};
