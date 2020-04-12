#pragma once
#include "DrawableGameObject.h"

#include "Camera.h"
#include "Graphics.h"

class Chunk
{
public:
	Chunk();
	~Chunk();

	void Update(double deltaTime);

	void Draw();

	static const int CHUNK_SIZE = 2;//16

	void CreateMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);


	//Test
	void RenderMesh(ConstantBuffer constantBuffer, Camera* camera, Graphics* graphics);

	void UpdateMesh(float deltaTime);
private:
	DrawableGameObject*** m_pBlocks;
};

