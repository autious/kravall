#ifndef SRC_GFX_TEXTURES_LUT_MANAGER_HPP
#define SRC_GFX_TEXTURES_LUT_MANAGER_HPP

#include <iostream>
#include <map>
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
		void SetActive(std::string name);
	private:

		std::map<std::string, GLuint> m_LUT;
		TextureManager* m_textureManager;

	};
}

#endif