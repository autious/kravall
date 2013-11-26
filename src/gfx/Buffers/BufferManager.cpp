#include "BufferManager.hpp"

namespace GFX
{

	BufferManager::BufferManager()
	{
	}

	BufferManager::~BufferManager()
	{

	}

	void BufferManager::BindMesh(GLuint id)
	{

	}

	void BufferManager::LoadStaticMesh(const StaticMeshData& meshData)
	{
		StaticMesh* mesh = new StaticMesh();
		mesh->Load(meshData);
		m_staticMeshes.push_back(mesh);
	}


}