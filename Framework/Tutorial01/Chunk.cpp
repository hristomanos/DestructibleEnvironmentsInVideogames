#include "Chunk.h"


//It is not vital that the voxel data is stored in a 3D array, but it is useful if we want to easily index the block data.
//For example, a cube located at 10,5,7 can be looked into the array by m_pBlocks[10][5][7]


Chunk::Chunk()
{
	//Create new blocks
	m_pBlocks = new DrawableGameObject** [CHUNK_SIZE]; //First pointer, make that a pointer to an array of cubes

	for (int i = 0; i < CHUNK_SIZE; i++)
	{
		m_pBlocks[i] = new DrawableGameObject*[CHUNK_SIZE]; //Second pointer, make that a pointer to another array of cubes

		for (int j = 0; j < CHUNK_SIZE; j++)
		{
			m_pBlocks[i][j] = new DrawableGameObject[CHUNK_SIZE]; //Third pointer, make that pointer to another array of cubes
		}

	}
}


Chunk::~Chunk()
{
	//Delete the blocks

	for (int i = 0; i < CHUNK_SIZE; i++)
	{
		for (int j = 0; j < CHUNK_SIZE; j++)
		{
			delete[] m_pBlocks[i][j];
		}

		delete[] m_pBlocks[i];
	}

	delete[] m_pBlocks;
}

void Chunk::CreateMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	//Create vertex buffer

	//iterate through each block and init all the active ones.
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				//Ignore if inactive
				if (m_pBlocks[x][y][z].isActive() == false)
				{
					continue;
				}

				//Init voxel
				m_pBlocks[x][y][z].Init(pDevice,pContext);
				m_pBlocks[x][y][z].SetPosition(XMFLOAT3(x * 2, y * 2, z * 2));

			}
		}
	}
}


void Chunk::RenderMesh(ConstantBuffer constantBuffer, Camera* camera, Graphics* graphics)
{
	//iterate through each block and init all the active ones.
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				//Ignore if inactive
				if (m_pBlocks[x][y][z].isActive() == false)
				{
					continue;
				}

				//Render voxel
				XMMATRIX *mGO = m_pBlocks[x][y][z].GetTransform();
				constantBuffer.mWorld = XMMatrixTranspose(*mGO);
				constantBuffer.mView = XMMatrixTranspose(camera->GetViewMatrix());
				constantBuffer.mProjection = XMMatrixTranspose(camera->GetProjectionMatrix());
				graphics->GetImmediateContext()->UpdateSubresource(graphics->GetConstantBuffer(), 0, nullptr, &constantBuffer, 0, 0);

				m_pBlocks[x][y][z].Draw(graphics->GetImmediateContext());

			}
		}
	}
}

void Chunk::UpdateMesh(float deltaTime)
{
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				m_pBlocks[x][y][z].Update(deltaTime);
			}
		}
	}
}
