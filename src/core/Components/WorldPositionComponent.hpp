#ifndef SRC_CORE_COMPONENTS_WORLD_POSITION_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_WORLD_POSITION_COMPONENT_HPP

#include <glm/glm.hpp>

namespace Core
{
	struct WorldPositionComponent
	{
		float position[3];

		WorldPositionComponent(float x, float y, float z)
		{
			position[0] = x;
			position[1] = y;
			position[2] = z;
		}

		WorldPositionComponent()
		{
			position[0] = 0;
			position[1] = 0;
			position[2] = 0;
		}

		static glm::vec4 GetVec4(float* position)
		{
			return glm::vec4(position[0], position[1], position[2], 1.0f);
		}

		static glm::vec3 GetVec3(float* position)
		{
			return glm::vec3(position[0], position[1], position[2]);
		}

		static void SetPosition(glm::vec3 newPos, float*& targetPos)
		{
			targetPos[0] = newPos.x;
			targetPos[1] = newPos.y;
			targetPos[2] = newPos.z;
		}

		static void SetPosition(const float& x, const float& y, const float& z, float*& targetPos)
		{
			targetPos[0] = x;
			targetPos[1] = y;
			targetPos[2] = z;
		}
	};
}

#endif