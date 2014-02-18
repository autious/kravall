#include "MeshManager.hpp"
#include <gfx/GFXDefines.hpp>

#include <iostream>
namespace GFX
{
	MeshManager::MeshManager()
	{
		m_idCounter = 0;
	}

	MeshManager::~MeshManager()
	{
		for (auto it = m_meshes.begin(); it != m_meshes.end(); it++)
		{
			glDeleteVertexArrays(1, &it->VAO);
			glDeleteBuffers(1, &it->IBO);
		}
		m_meshes.clear();
	}

	void MeshManager::BindMesh(const int& ibo, const int& vao)
	{
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	}

	void MeshManager::BindMesh(const int& vao)
	{
		glBindVertexArray(vao);
	}

	void MeshManager::UnbindMesh()
	{
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void MeshManager::DeleteMesh(unsigned long long id)
	{
		unsigned int index = FindMesh(id);

		if (index != std::numeric_limits<decltype(index)>::max())
		{
			glDeleteVertexArrays(1, &m_meshes[index].VAO);
			glDeleteBuffers(1, &m_meshes[index].IBO);
			m_meshes.erase(m_meshes.begin() + index);
		}
	
	}

	int MeshManager::BindSkeletonToMesh(const unsigned int& meshID, const int& skeletonID)
	{
		unsigned int index = FindMesh(meshID);

		if (index != std::numeric_limits<decltype(index)>::max())
		{
			m_meshes[index].skeletonID = skeletonID;
			return GFX_SUCCESS;
		}
		return GFX_FAIL;
	}

	int MeshManager::GetSkeletonID(const unsigned int& meshID)
	{
		unsigned int index = FindMesh(meshID);

		if (index != std::numeric_limits<decltype(index)>::max())
		{
			return m_meshes[index].skeletonID;
		}
		return -1;
	}

    void MeshManager::LoadMesh(unsigned int& meshID, const int& sizeVerts, const int& sizeIndices, GFX::Vertex* verts, int* indices)
	{
		Mesh mesh;
		GLuint VBO;
		GLuint VAO;
		GLuint IBO;

		//Generate VBO
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeVerts * sizeof (GFX::Vertex), verts, GL_STATIC_DRAW);

		//Generate IBO
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndices * sizeof(int), indices, GL_STATIC_DRAW);

		//Generate VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GFX::Vertex), (void*)0);
		glVertexAttribDivisor(0, 0);

		//Normal
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GFX::Vertex), (void*)(4 * sizeof(float)));
		glVertexAttribDivisor(1, 0);

		//Tangent
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(GFX::Vertex), (void*)(8 * sizeof(float)));
		glVertexAttribDivisor(2, 0);

		//bone indices
		glEnableVertexAttribArray(3);
		glVertexAttribIPointer(3, 4, GL_INT, sizeof(GFX::Vertex), (void*)(12 * sizeof(int)));
		glVertexAttribDivisor(3, 0);

		//bone weights
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(GFX::Vertex), (void*)(16 * sizeof(float)));
		glVertexAttribDivisor(4, 0);

		//UV
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(GFX::Vertex), (void*)(20 * sizeof(float)));
		glVertexAttribDivisor(5, 0);


		mesh.id = static_cast<unsigned int>(m_idCounter);
		m_idCounter++;
		mesh.VAO = VAO;
		mesh.IBO = IBO;
		mesh.indexCount = sizeIndices;
		mesh.skeletonID = -1;
		m_meshes.push_back(mesh);

		meshID = mesh.id;
	}
	
}
