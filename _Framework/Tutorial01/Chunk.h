#pragma once
#include "DrawableGameObject.h"

#include "Camera.h"
#include "Graphics.h"

inline static float random(int range)
{
	return (rand() % (range * 2) - range);
}



class Chunk
{
public:
	Chunk();
	~Chunk();

	void Update(double deltaTime);

	void Draw();

	void UI();

	static const int CHUNK_SIZE = 4;

	void CreateMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	void RenderMesh(ConstantBuffer constantBuffer, Camera* camera, Graphics* graphics);

	void UpdateMesh(float deltaTime);
private:
	DrawableGameObject*** m_pBlocks;
};

