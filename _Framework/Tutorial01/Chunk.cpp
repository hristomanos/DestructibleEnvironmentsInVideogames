#include "Chunk.h"


Chunk::Chunk()
{
	//Create new blocks
	m_pBlocks = new DrawableGameObject** [CHUNK_SIZE]; 

	for (int i = 0; i < CHUNK_SIZE; i++)
	{
		m_pBlocks[i] = new DrawableGameObject*[CHUNK_SIZE];

		for (int j = 0; j < CHUNK_SIZE; j++)
		{
			m_pBlocks[i][j] = new DrawableGameObject[CHUNK_SIZE];
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

void Chunk::UI()
{
	if (ImGui::CollapsingHeader("Chunk"))
	{
		ImGui::PushID("Chunk");
		ImGui::PopID();
	}
}

void Chunk::CreateMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	//Create vertex buffer
	//iterate through each block and init all the active ones.

	m_pBlocks[0][0][0].SetActive(false);
	m_pBlocks[3][0][0].SetActive(false);
	m_pBlocks[3][2][0].SetActive(false);
	m_pBlocks[2][2][0].SetActive(false);

	bool bDefault = true;
	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				if (x > 0 && y > 0 && z > 0 && x != CHUNK_SIZE - 1 && y != CHUNK_SIZE - 1 && z != CHUNK_SIZE - 1
					&& m_pBlocks[x - 1][y][z].isActive() == true && m_pBlocks[x + 1][y][z].isActive() == true
					&& m_pBlocks[x][y - 1][z].isActive() == true && m_pBlocks[x][y + 1][z].isActive() == true
					&& m_pBlocks[x][y][z -1].isActive() == true && m_pBlocks[x][y][z + 1].isActive() == true)
				{
					m_pBlocks[x][y][z].SetEmptyBlock(true);
				}

				//Ignore if inactive
				if (m_pBlocks[x][y][z].isActive() == false || m_pBlocks[x][y][z].isEmptyBlock() == true )
				{
					continue;
				}


				//Render triangles on the side of inactive voxels
				//If an adjacent block is deactivated, render that face
				if (x > 0 && m_pBlocks[x - 1][y][z].isActive() == false)
				{
					m_pBlocks[x][y][z].UpdateIndices(BlockFace::LEFT);
				}

				if (x < CHUNK_SIZE - 1 && m_pBlocks[x + 1][y][z].isActive() == false)
				{
					m_pBlocks[x][y][z].UpdateIndices(BlockFace::RIGHT);
				}

				if (y > 0 && m_pBlocks[x][y - 1][z].isActive() == false)
				{
					m_pBlocks[x][y][z].UpdateIndices(BlockFace::BOTTOM);
				}

				if (y < CHUNK_SIZE - 1 && m_pBlocks[x][y + 1][z].isActive() == false)
				{
					m_pBlocks[x][y][z].UpdateIndices(BlockFace::TOP);
				}

				if (z > 0 && m_pBlocks[x][y][z - 1].isActive() == false)
				{
					m_pBlocks[x][y][z].UpdateIndices(BlockFace::FRONT);
				}

				if (z < CHUNK_SIZE - 1 && m_pBlocks[x][y][z + 1].isActive() == false)
				{
					m_pBlocks[x][y][z].UpdateIndices(BlockFace::BACK);
				}


				//Draw a face on the oposite side for each active block
				//Check Left side
				if (x == CHUNK_SIZE - 1 && m_pBlocks[x - 1][y][z].isActive() == true || x == CHUNK_SIZE - 1 && m_pBlocks[x - 1][y][z].isEmptyBlock() == true)
				{
					m_pBlocks[x][y][z].UpdateIndices(BlockFace::RIGHT);
				}

				//Check Right side
				if (x == 0 && m_pBlocks[x + 1][y][z].isActive() == true || x == 0 && m_pBlocks[x + 1][y][z].isEmptyBlock() == true)
				{
					m_pBlocks[x][y][z].UpdateIndices(BlockFace::LEFT);
				}



				//Check top face
				if (y == 0 && m_pBlocks[x][y + 1][z].isActive() == true || y == 0 && m_pBlocks[x][y + 1][z].isEmptyBlock() == true)
				{
					m_pBlocks[x][y][z].UpdateIndices(BlockFace::BOTTOM);
				}

				//check bottom face
				if (y  == CHUNK_SIZE - 1 && m_pBlocks[x][y - 1][z].isActive() == true || y == CHUNK_SIZE - 1 && m_pBlocks[x][y - 1][z].isEmptyBlock() == true)
				{
					m_pBlocks[x][y][z].UpdateIndices(BlockFace::TOP);
				}

				//Check front face
				if (z == CHUNK_SIZE - 1 && m_pBlocks[x][y][z - 1].isActive() == true || z == CHUNK_SIZE - 1 && m_pBlocks[x][y][z - 1].isEmptyBlock() == true) //Front face
				{
					m_pBlocks[x][y][z].UpdateIndices(BlockFace::BACK);
				}


				//Check back face
				if (z == 0 && m_pBlocks[x][y][z + 1].isActive() == true || z == 0 && m_pBlocks[x][y][z + 1].isEmptyBlock() == true)
				{
					m_pBlocks[x][y][z].UpdateIndices(BlockFace::FRONT);
				}
				

				//Init voxel
				m_pBlocks[x][y][z].Init(pDevice,pContext);
				m_pBlocks[x][y][z].InitMesh(pDevice, pContext);
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
				if (m_pBlocks[x][y][z].isActive() == false || m_pBlocks[x][y][z].isEmptyBlock() == true)
				{
					continue;
				}

				//Render voxel
				XMMATRIX mGO = XMLoadFloat4x4(m_pBlocks[x][y][z].GetTransform());
				constantBuffer.mWorld = XMMatrixTranspose(mGO);
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





