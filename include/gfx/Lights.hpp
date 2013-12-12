#ifndef GFXLIGHTS_INCLUDE_HPP
#define GFXLIGHTS_INCLUDE_HPP

#include <glm/glm.hpp>

namespace GFX
{
	// Point light
	struct PointLight
	{
		glm::vec3 position;
		float radius;
		glm::vec3 color;
		float intensity;
		bool hasShadow;
	};

	// Spotlight
	struct SpotLight
	{
		glm::vec3 position;
		float length;
		float angle;
		glm::vec3 color;
		float intensity;
		bool hasShadow;
	};

	// Directional light
	struct DirectionalLight
	{
		glm::vec3 direction;
		glm::vec3 color;
		float intensity;
		bool hasShadow;
	};
}

#endif