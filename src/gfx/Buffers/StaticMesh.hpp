#ifndef SRC_GFX_BUFFERS_STATIC_MESH_HPP
#define SRC_GFX_BUFFERS_STATIC_MESH_HPP

#include "../Utility/GLFWInclude.hpp"
#include "Vertex.hpp"
#include "MeshData.hpp"

namespace GFX
{
	class StaticMesh
	{
	public:
		StaticMesh();
		~StaticMesh();
		
		void Load(const StaticMeshData& meshData);

		void BindMesh();

		GLuint GetVBO();
		GLuint GetVAO();
		GLuint GetIBO();

	private:
		GLuint m_VBO;
		GLuint m_VAO;
		GLuint m_IBO;
	};
}

#endif
