#ifndef INCLUDE_GFX_VERTEX_HPP
#define INCLUDE_GFX_VERTEX_HPP

#include <glm/glm.hpp>

namespace GFX
{
	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec4 tangent;
		glm::ivec4 boneIndices;
		glm::vec4 boneWeights;
		glm::vec2 uv;
	};
}

#endif
