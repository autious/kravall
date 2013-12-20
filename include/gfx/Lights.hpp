#ifndef GFXLIGHTS_INCLUDE_HPP
#define GFXLIGHTS_INCLUDE_HPP

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace GFX
{
    /*
	// Point light
	struct PointLight
	{
		glm::vec3 color;
		float intensity;
	};

	// Spotlight
	struct SpotLight
	{
		float angle;
		glm::vec3 color;
		float intensity;
	};

	// Directional light
	struct DirectionalLight
	{
		glm::vec3 color;
		float intensity;
	};

	// Ambient light
	struct AmbientLight
	{
		glm::vec3 color;
		float intensity;
	};
    */

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
