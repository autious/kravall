#ifndef SRC_GFX_BUFFERS_STATIC_MESH_HPP
#define SRC_GFX_BUFFERS_STATIC_MESH_HPP

#include "Vertex.hpp"
#include "MeshData.hpp"
#include <gl/glew.h>

namespace GFX
{
	class StaticMesh
	{
	public:
		StaticMesh();
		~StaticMesh();
		
		void Load(const StaticMeshData& meshData);

		GLuint GetVBO();
		GLuint GetVAO();

	private:
		GLuint m_VBO;
		GLuint m_VAO;
		GLuint m_IBO;
	};
}

#endif