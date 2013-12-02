#include "BufferManager.hpp"

namespace GFX
{

	BufferManager::BufferManager()
	{
	}

	BufferManager::~BufferManager()
	{
		for (int i = 0; i < m_staticMeshes.size(); i++)
			delete(m_staticMeshes.at(i));
	}

	void BufferManager::BindMesh(GLuint id)
	{

		for (unsigned int i = 0; i < m_staticMeshes.size(); i++)
		{
			if (id == m_staticMeshes.at(i)->GetVBO())
			{
				m_staticMeshes.at(i)->BindMesh();
				return;
			}
		}

		//loop through list until id found
		//bind
		//return
	}

	void BufferManager::UnbindMesh()
	{
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void BufferManager::LoadStaticMesh(const StaticMeshData& meshData)
	{
		StaticMesh* mesh = new StaticMesh();
		mesh->Load(meshData);
		m_staticMeshes.push_back(mesh);
	}

}