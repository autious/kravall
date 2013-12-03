#ifndef SRC_GFX_BUFFERS_MESH_MANAGER_HPP
#define SRC_GFX_BUFFERS_MESH_MANAGER_HPP

#include <gfx/Vertex.hpp>
#include <GL/glew.h>
namespace GFX
{
	namespace MeshManager
	{
		void BindMesh(const int& ibo, const int& vao);
		void BindMesh(const int& vao);
		void UnbindMesh();
		void LoadStaticMesh(GLuint& IBO, GLuint& VAO, int& sizeVerts, int& sizeIndices, StaticVertex* verts, int* indices);
	}
}

#endif