#ifndef INCLUDE_GFX_VERTEX_HPP
#define INCLUDE_GFX_VERTEX_HPP

#include <glm/glm.hpp>

namespace GFX
{
	struct StaticVertex
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec4 tangent;
		glm::vec4 binormal;
		glm::vec2 uv;
	};
}

#endif