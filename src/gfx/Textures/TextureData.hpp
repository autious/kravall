#ifndef SRC_GFX_TEXTURES_TEXTURE_DATA_HPP
#define SRC_GFX_TEXTURES_TEXTURE_DATA_HPP

#include <GL/glew.h>

namespace GFX
{
	struct TextureData
	{
		unsigned int id;
		GLuint textureHandle;
	};
}

#endif