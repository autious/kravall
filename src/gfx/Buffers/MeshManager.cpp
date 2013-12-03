#include "MeshManager.hpp"

namespace GFX
{
	namespace MeshManager
	{
		void BindMesh(const int& ibo, const int& vao)
		{
			glBindVertexArray(vao);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		}

		void BindMesh(const int& vao)
		{
			glBindVertexArray(vao);
		}

		void UnbindMesh()
		{
			glBindVertexArray(0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		void LoadStaticMesh(GLuint& IBO, GLuint& VAO, int& sizeVerts, int& sizeIndices, StaticVertex* verts, int* indices)
		{
			GLuint VBO;

			//Generate VBO
			glGenBuffers(1, &VBO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeVerts * sizeof (StaticVertex), verts, GL_STATIC_DRAW);

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

			//Generate IBO
			glGenBuffers(1, &IBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndices * sizeof(int), indices, GL_STATIC_DRAW);
		}
	}
}