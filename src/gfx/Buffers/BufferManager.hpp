#ifndef SRC_GFX_BUFFERS_BUFFER_MANAGER_HPP
#define SRC_GFX_BUFFERS_BUFFER_MANAGER_HPP

#include "Vertex.hpp"
#include "StaticMesh.hpp"

#include <vector>
#include <gl/glew.h>

namespace GFX
{
	class BufferManager
	{
	public:
		BufferManager();
		~BufferManager();

		void LoadStaticMesh(const StaticMeshData& meshData);
		void BindMesh(GLuint id);
	private:
		std::vector<StaticMesh*> m_staticMeshes;
	};
}

#endif
