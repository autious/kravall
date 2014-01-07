#ifndef GFXLIGHTS_INCLUDE_HPP
#define GFXLIGHTS_INCLUDE_HPP

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace GFX
{
	// Struct used for light data transfer to shader
	struct LightData
	{
		glm::vec3 position;
		float radius_length;
		glm::vec3 color;
		float intensity;
		glm::vec3 pad;
		float spot_angle;
		glm::vec4 orientation;
	};
}

#endif
