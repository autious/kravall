#ifndef SRC_GFX_BUFFERS_VERTEX_HPP
#define SRC_GFX_BUFFERS_VERTEX_HPP

#include <glm/glm.hpp>

namespace GFX
{
	struct StaticVertex
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec4 tangent;
		glm::vec2 uv;
	};
}

#endif