#include "StaticMesh.hpp"

namespace GFX
{
	StaticMesh::StaticMesh()
	{
		m_VAO = 0;
		m_VBO = 0;
	}

	StaticMesh::~StaticMesh()
	{
		glDeleteBuffers(1, &m_VBO);
	}

	GLuint StaticMesh::GetVAO()
	{
		return m_VAO;
	}

	GLuint StaticMesh::GetVBO()
	{
		return m_VBO;
	}

	GLuint StaticMesh::GetIBO()
	{
		return m_IBO;
	}

	void StaticMesh::Load(const StaticMeshData& meshData)
	{
		//Generate VBO
		glGenBuffers(1, &m_VBO);
		
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, meshData.vertices.size() * sizeof (StaticVertex), meshData.vertices.data(), GL_STATIC_DRAW);

		//Generate VAO
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		//Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(StaticVertex), (void*)0);

		//Normal
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(StaticVertex), (void*)(4 * sizeof(float)));

		//Tangent
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(StaticVertex), (void*)(8 * sizeof(float)));

		//UV
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(StaticVertex), (void*)(12 * sizeof(float)));

		//Generate IBO
		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.indices.size() * sizeof(int), meshData.indices.data(), GL_STATIC_DRAW);
	}

	void StaticMesh::BindMesh()
	{
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	}
}