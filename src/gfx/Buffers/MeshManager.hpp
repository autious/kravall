#ifndef SRC_GFX_BUFFERS_MESH_MANAGER_HPP
#define SRC_GFX_BUFFERS_MESH_MANAGER_HPP

#include <vector>
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

		inline Mesh GetMesh(const unsigned long long int& meshID) 
		{ 
			int index = FindMesh(meshID);

			if (index != UINT_MAX)
				return m_meshes[index]; 

			Mesh m;
			m.id = UINT_MAX;
			m.indexCount = 0;
			m.VAO = UINT_MAX;

			return m;
		}

	private:
		std::vector<Mesh> m_meshes;
		unsigned long long int m_idCounter;

		inline const unsigned int FindMesh(unsigned long long id)
		{
			for (unsigned int i = 0; i < m_meshes.size(); i++)
				if (m_meshes[i].id = static_cast<unsigned int>(id))
					return i;

			return UINT_MAX;
		}
	};
}

#endif