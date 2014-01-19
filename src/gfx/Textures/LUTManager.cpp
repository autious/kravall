#include "LUTManager.hpp"

namespace GFX
{
	LUTManager::LUTManager()
	{

	}

	LUTManager::~LUTManager()
	{

	}

	void LUTManager::Initialize(TextureManager* textureManager)
	{
		m_textureManager = textureManager;
	}

	void LUTManager::Reload()
	{
		//for (int i = 0; i < m_LUT.count; i++)
		//{
		//	//m_LUT.at(i)
		//}
	}

	void LUTManager::SetActive(std::string name)
	{

	}
}