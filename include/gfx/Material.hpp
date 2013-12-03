#ifndef INCLUDE_GFX_MATERIAL_HPP
#define INCLUDE_GFX_MATERIAL_HPP

#include <glm/glm.hpp>
#include <GL/glew.h>

namespace GFX
{
	struct Material
	{
		GLuint diffuse;
		GLuint specular;
		GLuint normal;
		GLuint glow;
	};
}

#endif