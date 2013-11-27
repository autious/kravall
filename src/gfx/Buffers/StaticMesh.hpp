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
		
		/*!
		Load a static mesh into a VBO, VAO and IBO
		\param meshData Vertex and index data for a static mesh
		*/
		void Load(const StaticMeshData& meshData);

		/*!
		Bind the VAO and IBO of a mesh which matches id
		\param id The id of the mesh to be bound
		*/
		void BindMesh();

		/*!
		Returns the VBO id
		\return VBO id
		*/
		GLuint GetVBO();

		/*!
		Returns the VAO id
		\return VAO id
		*/
		GLuint GetVAO();

		/*!
		Returns the IBO id
		\return IBO id
		*/
		GLuint GetIBO();

	private:
		GLuint m_VBO;
		GLuint m_VAO;
		GLuint m_IBO;
	};
}

#endif
