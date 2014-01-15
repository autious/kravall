#ifndef SRC_GFX_BUFFERS_MESH_MANAGER_HPP
#define SRC_GFX_BUFFERS_MESH_MANAGER_HPP

#include <vector>
#include <limits>
#include <type_traits>
#include <gfx/Vertex.hpp>
#include "MeshData.hpp"
#include <GL/glew.h>
namespace GFX
{
	class MeshManager
	{
	public:
		MeshManager();
		~MeshManager();

		void BindMesh(const int& ibo, const int& vao);
		void BindMesh(const int& vao);
		void UnbindMesh();
		void DeleteMesh(unsigned long long id);

		void LoadStaticMesh(unsigned int& meshID, const int& sizeVerts, const int& sizeIndices, StaticVertex* verts, int* indices);
		void LoadAnimatedMesh(unsigned int& meshID, const int& sizeVerts, const int& sizeIndices, AnimatedVertex* verts, int* indices);

		inline Mesh GetMesh(const unsigned long long int& meshID) 
		{ 
			unsigned int index = FindMesh(meshID);

			if (index != std::numeric_limits<decltype(index)>::max())
				return m_meshes[index]; 

			Mesh m;
			m.id = std::numeric_limits<decltype(m.id)>::max();
			m.indexCount = 0;
			m.VAO = std::numeric_limits<decltype(m.VAO)>::max();

			return m;
		}

	private:
		std::vector<Mesh> m_meshes;
		unsigned long long int m_idCounter;

		inline const unsigned int FindMesh(unsigned long long id)
		{
			for (unsigned int i = 0; i < m_meshes.size(); i++)
				if (m_meshes[i].id == static_cast<unsigned int>(id))
					return i;

			return std::numeric_limits<std::result_of<decltype(&MeshManager::FindMesh)(MeshManager, unsigned long long)>::type>::max();
		}
	};
}

#endif
