#include "MeshManager.hpp"

namespace GFX
{
	MeshManager::MeshManager()
	{
		m_idCounter = 0;
	}

	MeshManager::~MeshManager()
	{

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
			m_meshes.erase(m_meshes.begin() + index);
		}
	
	}

	void MeshManager::LoadStaticMesh(unsigned int& meshID, const int& sizeVerts, const int& sizeIndices, StaticVertex* verts, int* indices)
	{
		Mesh mesh;
		GLuint VBO;
		GLuint VAO;
		GLuint IBO;

		//Generate VBO
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeVerts * sizeof (StaticVertex), verts, GL_STATIC_DRAW);

		//Generate IBO
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndices * sizeof(int), indices, GL_STATIC_DRAW);

		//Generate VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(StaticVertex), (void*)0);

		//Normal
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(StaticVertex), (void*)(4 * sizeof(float)));

		//Tangent
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(StaticVertex), (void*)(8 * sizeof(float)));
			
		//Binormal
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(StaticVertex), (void*)(12 * sizeof(float)));

		//UV
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(StaticVertex), (void*)(16 * sizeof(float)));



		mesh.id = static_cast<unsigned int>(m_idCounter);
		m_idCounter++;
		mesh.VAO = VAO;
		mesh.IBO = IBO;
		mesh.indexCount = sizeIndices;
		m_meshes.push_back(mesh);

		meshID = mesh.id;
	}

	
}