#include "MaterialManager.hpp"
#include <GFXDefines.hpp>

namespace GFX
{
	MaterialManager::MaterialManager()
	{
		m_idCounter = 0;
	}

	MaterialManager::~MaterialManager()
	{

	}

	void MaterialManager::CreateMaterial(unsigned long long int& id)
	{
		Material material;
		material.id = static_cast<unsigned int>(m_idCounter);
		m_idCounter++;
		material.shaderProgramID = std::numeric_limits<unsigned int>::max();
		m_materials.push_back(material);
		id = material.id;
	}

	void MaterialManager::DeleteMaterial(const unsigned long long int& id)
	{
		unsigned int index = FindMaterial(id);
		if (index != std::numeric_limits<unsigned int>::max())
		{
			m_materials.erase(m_materials.begin() + index);
		}
	}

	int MaterialManager::AddTexture(const unsigned long long int& materialID, const unsigned long long int& textureID)
	{
		unsigned int index = FindMaterial(materialID);
		if (index != std::numeric_limits<unsigned int>::max())
		{
			m_materials[index].textures.push_back(textureID);
			return GFX_SUCCESS;
		}
		else
		{
			return GFX_INVALID_MATERIAL;
		}
	}

	void MaterialManager::RemoveTexture(const unsigned long long int& materialID, const unsigned long long int& textureID)
	{
		unsigned int index = FindMaterial(materialID);
		if (index != std::numeric_limits<unsigned int>::max())
		{
			for (unsigned int i = 0; i < m_materials[index].textures.size(); i++)
			{
				if (m_materials[index].textures[i] == textureID)
				{
					m_materials[index].textures.erase(m_materials[index].textures.begin() + i);
				}
			}
		}
	}

	int MaterialManager::SetShader(const unsigned long long int& materialID, const unsigned int& shaderID)
	{
		unsigned int index = FindMaterial(materialID);
		if (index != std::numeric_limits<unsigned int>::max())
		{
			m_materials[index].shaderProgramID = shaderID;
			return GFX_SUCCESS;
		}
		else
		{
			return GFX_INVALID_MATERIAL;
		}
	}
}
