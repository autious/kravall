#ifndef SRC_GFX_MATERIAL_MATERIAL_MANAGER_HPP
#define SRC_GFX_MATERIAL_MATERIAL_MANAGER_HPP

#include <vector>
#include <limits>
#include "MaterialData.hpp"
#include <GL/glew.h>
namespace GFX
{
	class MaterialManager
	{
	public:
		MaterialManager();
		~MaterialManager();

		void CreateMaterial(unsigned long long int& id);
		void DeleteMaterial(const unsigned long long int& id);
		int AddTexture(const unsigned long long int& materialID, const unsigned long long int& textureID);
		void RemoveTexture(const unsigned long long int& materialID, const unsigned long long int& textureID);

		int SetShader(const unsigned long long int& materialID, const unsigned int& shaderID);

		inline Material GetMaterial(const unsigned long long int& materialID) 
		{ 
			unsigned int index = FindMaterial(materialID);

			if (index != std::numeric_limits<unsigned int>::max())
				return m_materials[index]; 

			Material m;

			return m;
		}
		
	private:
		std::vector<Material> m_materials;
		unsigned long long m_idCounter;

		inline const unsigned int FindMaterial(unsigned long long id)
		{
			for (unsigned int i = 0; i < m_materials.size(); i++)
				if (m_materials[i].id == static_cast<unsigned int>(id))
					return i;

			return std::numeric_limits<unsigned int>::max();
		}
	};
}

#endif
