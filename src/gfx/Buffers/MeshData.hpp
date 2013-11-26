#ifndef SRC_GFX_MESH_DATA_HPP
#define SRC_GFX_MESH_DATA_HPP

#include "Vertex.hpp"
#include <vector>

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
		std::vector<StaticVertex> vertices;
		std::vector<int> indices;
	};
}

#endif