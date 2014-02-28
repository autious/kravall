#ifndef SRC_GFX_TEXTURES_LUT_MANAGER_HPP
#define SRC_GFX_TEXTURES_LUT_MANAGER_HPP

#include <iostream>
#include <vector>
#include "Textures/TextureManager.hpp"

namespace GFX
{
	class LUTManager
	{
	public:
		LUTManager();
		~LUTManager();

		void Initialize(TextureManager* textureManager);

		void Reload();
		GLuint GetLUTHandle(std::string name);
	private:

		std::vector<std::string> m_LUTNames;
		std::vector<GLuint> m_LUTTextures;
		TextureManager* m_textureManager;

	};
}

#endif