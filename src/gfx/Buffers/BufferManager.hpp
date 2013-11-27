#ifndef SRC_GFX_BUFFERS_BUFFER_MANAGER_HPP
#define SRC_GFX_BUFFERS_BUFFER_MANAGER_HPP

#include "Vertex.hpp"
#include "StaticMesh.hpp"

#include <vector>
#include <GL/glew.h>

namespace GFX
{
	class BufferManager
	{
	public:
		BufferManager();
		~BufferManager();

		/*!
		Load a static mesh into a VBO, VAO and IBO, save it in the vector for static meshes
		\param meshData Vertex and index data for a static mesh
		*/
		void LoadStaticMesh(const StaticMeshData& meshData);

		/*!
		Bind the VAO and IBO of a mesh which matches id
		\param id The id of the mesh to be bound
		*/
		void BindMesh(GLuint id);

		/*!
		Unbind any mesh from the GPU
		*/
		void UnbindMesh();

	private:
		std::vector<StaticMesh*> m_staticMeshes;
	};
}

#endif
