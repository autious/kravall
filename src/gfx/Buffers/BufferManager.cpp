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
		bool found = false;

		//loop through list until id found
		//bind
		//return
	}

	void BufferManager::LoadStaticMesh(const StaticMeshData& meshData)
	{
		StaticMesh* mesh = new StaticMesh();
		mesh->Load(meshData);
		m_staticMeshes.push_back(mesh);
	}


}