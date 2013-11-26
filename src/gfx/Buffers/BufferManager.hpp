#ifndef SRC_GFX_BUFFERS_BUFFER_MANAGER_HPP
#define SRC_GFX_BUFFERS_BUFFER_MANAGER_HPP

#include "..\Utility\GLFWInclude.hpp"
#include "Vertex.hpp"

#include "StaticMesh.hpp"

#include <vector>

namespace GFX
{
	class BufferManager
	{
	public:
		BufferManager();
		~BufferManager();

		void LoadStaticMesh(const StaticMeshData& meshData);
		void BindMesh(GLuint id);
		void UnbindMesh();

	private:
		std::vector<StaticMesh*> m_staticMeshes;
	};
}

#endif