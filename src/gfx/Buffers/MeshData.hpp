#ifndef SRC_GFX_MESH_DATA_HPP
#define SRC_GFX_MESH_DATA_HPP

#include "Vertex.hpp"
#include <vector>
#include <GL/glew.h>

namespace GFX
{
	enum MeshType
	{
		MESHTYPE_STATIC,
		MESHTYPE_DYNAMIC,
		MESHTYPE_COUNT
	};

	struct StaticMeshData
	{
		std::vector<GFX::Vertex> vertices;
		std::vector<int> indices;
	};

	struct Mesh
	{
		unsigned int id;
		GLuint VAO;
		GLuint IBO;
		unsigned indexCount;
	};
}

#endif
