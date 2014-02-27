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

		Reload();
	}

	void LUTManager::Reload()
	{
		if (m_LUTTextures.size() > 0)
		{
			for (int i = 0; i < m_LUTTextures.size(); i++)
				m_textureManager->DeleteTexture(m_LUTTextures[i]);

			m_LUTTextures.clear();
			m_LUTNames.clear();
		}

		GLuint id;
		m_textureManager->Load3DTexture(id, 32, 32, 32, "assets/texture/Lut/identityLUT.png");
		m_LUTTextures.push_back(id);
		m_LUTNames.push_back("identity");

		m_textureManager->Load3DTexture(id, 32, 32, 32, "assets/texture/Lut/sepia.png");
		m_LUTTextures.push_back(id);
		m_LUTNames.push_back("sepia");

		m_textureManager->Load3DTexture(id, 32, 32, 32, "assets/texture/Lut/blackandwhite.png");
		m_LUTTextures.push_back(id);
		m_LUTNames.push_back("blackandwhite");

		m_textureManager->Load3DTexture(id, 32, 32, 32, "assets/texture/Lut/test.png");
		m_LUTTextures.push_back(id);
		m_LUTNames.push_back("test");

		m_textureManager->Load3DTexture(id, 32, 32, 32, "assets/texture/Lut/test3.png");
		m_LUTTextures.push_back(id);
		m_LUTNames.push_back("test3");
	}

	GLuint LUTManager::GetLUTHandle(std::string name)
	{
		for (int i = 0; i < m_LUTNames.size(); i++)
		{
			if (m_LUTNames[i] == name)
				return m_LUTTextures[i];
		}

		return -1;
	}
}