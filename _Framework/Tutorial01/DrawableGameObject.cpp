#include "DrawableGameObject.h"


using namespace std;
using namespace DirectX;

#define NUM_VERTICES 36

DrawableGameObject::DrawableGameObject()
{
	// Initialize the world matrix
	XMMATRIX world = XMMatrixIdentity();
	XMStoreFloat4x4(&m_World, world);
}

void DrawableGameObject::Release() 
{
	if (m_pVertexBuffer)
		m_pVertexBuffer->Release();

	if (m_pIndexBuffer)
		m_pIndexBuffer->Release();

	if (m_pVertexLayout)
		m_pVertexLayout->Release();

	if (m_pVertexShader)
		m_pVertexShader->Release();
	if (m_pPixelShader)
		m_pPixelShader->Release();

	if (m_pTextureResourceView)
		m_pTextureResourceView->Release();

	if (m_pSamplerLinear)
		m_pSamplerLinear->Release();

	if (m_pPixelShaderSolid)
		m_pPixelShaderSolid->Release();
}


HRESULT DrawableGameObject::Init(ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext)
{
	ID3DBlob* pVSBlob = nullptr;
	HRESULT hr;

	hr = CreateVertexlShader(pVSBlob, pd3dDevice);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"Creating Vertex Shader - FAILED", L"Error", MB_OK);
		return hr;
	}

	hr = CreateInputLayout(pVSBlob, pd3dDevice, pContext);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"Creating Input layout - FAILED", L"Error", MB_OK);
		return hr;
	}

	hr = CreatePixelShader(pd3dDevice);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"Creating pixel shader - FAILED", L"Error", MB_OK);
		return hr;
	}

	return hr;
}

HRESULT DrawableGameObject::InitMesh(ID3D11Device *pd3dDevice, ID3D11DeviceContext *pContext)
{
	HRESULT hr;
	
	// Create vertex buffer
	SimpleVertex vertices[] =
		{
			//TOP FACE
			{XMFLOAT3(-1.0f, 1.0f, -1.0f),  XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)},
			{XMFLOAT3( 1.0f, 1.0f, -1.0f),  XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)},
			{XMFLOAT3( 1.0f, 1.0f,  1.0f),  XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)},
			{XMFLOAT3(-1.0f, 1.0f,  1.0f),  XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)},

			//BOTTOM FACE
			{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)},
			{XMFLOAT3( 1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)},
			{XMFLOAT3( 1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)},
			{XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)},

			//LEFT FACE
			{XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)},
			{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)},
			{XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)},
			{XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)},

			//RIGHT FACE
			{XMFLOAT3( 1.0f, -1.0f, 1.0f), XMFLOAT3( 1.0f, 0.0f, 0.0f), XMFLOAT2( 1.0f, 1.0f)},
			{XMFLOAT3( 1.0f, -1.0f,-1.0f), XMFLOAT3( 1.0f, 0.0f, 0.0f), XMFLOAT2( 0.0f, 1.0f)},
			{XMFLOAT3( 1.0f,  1.0f,-1.0f), XMFLOAT3( 1.0f, 0.0f, 0.0f), XMFLOAT2( 0.0f, 0.0f)},
			{XMFLOAT3( 1.0f,  1.0f, 1.0f), XMFLOAT3( 1.0f, 0.0f, 0.0f), XMFLOAT2( 1.0f, 0.0f)},

			//FRONT FACE
			{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f)},
			{XMFLOAT3( 1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f)},
			{XMFLOAT3( 1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f)},
			{XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f)},

			//BACK FACE
			{XMFLOAT3(-1.0f, -1.0f, 1.0f),  XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2( 1.0f, 1.0f)},
			{XMFLOAT3( 1.0f, -1.0f, 1.0f),  XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2( 0.0f, 1.0f)},
			{XMFLOAT3( 1.0f, 1.0f, 1.0f),   XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2( 0.0f, 0.0f)},
			{XMFLOAT3(-1.0f, 1.0f, 1.0f),   XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2( 1.0f, 0.0f)},
		};

	for (SimpleVertex vertex : vertices)
			m_VertPosArray.push_back(vertex.Pos);

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 24;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = vertices;
	 hr = pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
	if (FAILED(hr))
		return hr;

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	for (WORD index : m_Indices)
		m_IndexPosArray.push_back(index);

	//Describe index buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * m_Indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = &m_Indices[0];
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer);
	if (FAILED(hr))
		return hr;

	m_Indices.clear();

	//Textures
	hr = CreateDDSTextureFromFile(pd3dDevice, L"Resources\\color.dds", nullptr, &g_pTextureArr[0]);
	hr = CreateDDSTextureFromFile(pd3dDevice, L"Resources\\normals.dds", nullptr, &g_pTextureArr[1]);
	if (FAILED(hr))
		return hr;

	//Sampler description
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = pd3dDevice->CreateSamplerState(&sampDesc, &m_pSamplerLinear);

	//Create the material constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MaterialPropertiesConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = pd3dDevice->CreateBuffer(&bd, nullptr, &m_pMaterialConstantBuffer);
	if (FAILED(hr))
		return hr;

	return hr;
}

HRESULT DrawableGameObject::CreateVertexlShader(ID3DBlob*& pVSBlob, ID3D11Device* pDevice)
{

	HRESULT hr = Helper::CompileShaderFromFile(L"shader.fx", "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	return hr;
}

HRESULT DrawableGameObject::CreateInputLayout(ID3DBlob*& pVSBlob, ID3D11Device* pDevice,ID3D11DeviceContext* pContext)
{
	HRESULT hr;

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = pDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(), &m_pVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr))
		return hr;

	// Set the input layout
	pContext->IASetInputLayout(m_pVertexLayout);

	return hr;
}

HRESULT DrawableGameObject::CreatePixelShader(ID3D11Device* pDevice)
{
	HRESULT hr;

	// Compile the pixel shader
	ID3DBlob *pPSBlob = nullptr;
	hr = Helper::CompileShaderFromFile(L"shader.fx", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
				   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;

	return hr;
}

void DrawableGameObject::SetPosition(XMFLOAT3 position)
{
	m_position = position;
}

//add face of mesh in indices buffer
void DrawableGameObject::UpdateIndices(BlockFace blockFace)
{
	
	switch (blockFace)
	{
	case BlockFace::BACK:
			//BACK FACE
		m_Indices.push_back(22);
		m_Indices.push_back(20);
		m_Indices.push_back(21);
		m_Indices.push_back(23);
		m_Indices.push_back(20);
		m_Indices.push_back(22);
		break;
	case BlockFace::FRONT:
		//FRONT FACE
		m_Indices.push_back(19);
		m_Indices.push_back(17);
		m_Indices.push_back(16);
		m_Indices.push_back(18);
		m_Indices.push_back(17);
		m_Indices.push_back(19);
		break;
	case BlockFace::LEFT:
		//LEFT FACE
		m_Indices.push_back(11);
		m_Indices.push_back(9);
		m_Indices.push_back(8);
		m_Indices.push_back(10);
		m_Indices.push_back(9);
		m_Indices.push_back(11);
		break;
	case BlockFace::RIGHT:
		//RIGHT FACE
		m_Indices.push_back(14);
		m_Indices.push_back(12);
		m_Indices.push_back(13);
		m_Indices.push_back(15);
		m_Indices.push_back(12);
		m_Indices.push_back(14);
		break;
	case BlockFace::TOP:
		//TOP FACE
		m_Indices.push_back(3);
		m_Indices.push_back(1);
		m_Indices.push_back(0);
		m_Indices.push_back(2);
		m_Indices.push_back(1);
		m_Indices.push_back(3);
		break;
	case BlockFace::BOTTOM:
		//BOTTOM FACE
		m_Indices.push_back(6);
		m_Indices.push_back(4);
		m_Indices.push_back(5);
		m_Indices.push_back(7);
		m_Indices.push_back(4);
		m_Indices.push_back(6);
		break;
	default:
		break;
	}
}


void DrawableGameObject::Update(float t)
{
	// Cube:  Rotate around origin
	XMMATRIX mSpin = XMMatrixRotationY(t);

	XMMATRIX mTranslate = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	XMMATRIX world = mTranslate;
	XMStoreFloat4x4(&m_World, world);
}

void DrawableGameObject::Draw(ID3D11DeviceContext *pContext)
{
	MaterialPropertiesConstantBuffer redPlasticMaterial;
	redPlasticMaterial.Material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	redPlasticMaterial.Material.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	redPlasticMaterial.Material.SpecularPower = 32.0f;
	redPlasticMaterial.Material.UseTexture = false;
	redPlasticMaterial.Material.UseBumpMap = false;
	pContext->UpdateSubresource(m_pMaterialConstantBuffer, 0, nullptr, &redPlasticMaterial, 0, 0);

	pContext->VSSetShader(m_pVertexShader, nullptr, 0);
	pContext->PSSetShader(m_pPixelShader, nullptr, 0);

	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pContext->PSSetConstantBuffers(1, 1, &m_pMaterialConstantBuffer);
	pContext->PSSetShaderResources(0, 2, g_pTextureArr);
	pContext->PSSetSamplers(0, 1, &m_pSamplerLinear);

	pContext->DrawIndexed(NUM_VERTICES, 0, 0);
}