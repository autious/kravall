#ifndef SRC_CORE_COMPONENTS_ROTATION_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_ROTATION_COMPONENT_HPP

#include <glm/glm.hpp>
#include <glm/ext.hpp>
namespace Core
{
	struct RotationComponent
	{
		float rotation[4];

		RotationComponent()
		{
			rotation[0] = 0.0f;
			rotation[1] = 0.0f;
			rotation[2] = 0.0f;
			rotation[3] = 1.0f;
		}

		RotationComponent(float x, float y, float z, float w)
		{
			rotation[0] = x;
			rotation[1] = y;
			rotation[2] = z;
			rotation[3] = w;
		}

		static glm::quat GetQuat(float* rotation)
		{
			glm::quat q;
			q.x = rotation[0];
			q.y = rotation[1];
			q.z = rotation[2];
			q.w = rotation[3];
			
			return q;
		}

	};
}

#endif
