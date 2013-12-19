#ifndef SRC_CORE_COMPONENTS_WORLD_POSITION_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_WORLD_POSITION_COMPONENT_HPP

#include <glm/glm.hpp>

namespace Core
{
	/*!
		Component holding an object's position in the world.
	*/
	struct WorldPositionComponent
	{
		/*! The object's position in the world. Represented by a floating point vector of size 3. */
		float position[3];

		/*!
			Constructor setting the position to the values specified in the positionVector parameter.
			\param positionVector The position of the object in a three dimensional vector.
		*/
		WorldPositionComponent(const glm::vec3& positionVector)
		{
			position[0] = positionVector.x;
			position[1] = positionVector.y;
			position[2] = positionVector.z;
		}

		/*!
			Constructor setting the position to the values specified in the x, y and z parameters.
			\param x The x-component of the position.
			\param y The y-component of the position.
			\param z The z-component of the position.
		*/
		WorldPositionComponent(const float& x, const float& y, const float& z)
		{
			position[0] = x;
			position[1] = y;
			position[2] = z;
		}

		/*! Default constructor. Initialising position to origo (0.0f, 0.0f, 0.0f). */
		WorldPositionComponent()
		{
			position[0] = 0.0f;
			position[1] = 0.0f;
			position[2] = 0.0f;
		}

		/*!
			Translates a WorldPositionComponent into a four dimensional vector where x, y and z corresponds to the world
			position and w is 1.0f.
			\param wpc The WorldPositionComponent to translate.
			\return A vec4 containing the WorldPositionComponent's position in x, y and z and 1.0f in w.
		*/
		static glm::vec4 GetVec4(const WorldPositionComponent& wpc)
		{
			return glm::vec4(wpc.position[0], wpc.position[1], wpc.position[2], 1.0f);
		}

		/*!
			Translates a WorldPositionComponent into a three dimensional vector where x, y and z corresponds to the world
			position.
			\param wpc The WorldPositionComponent to translate.
			\return A vec3 containing the WorldPositionComponent's position in x, y and z.
		*/
		static glm::vec3 GetVec3(const WorldPositionComponent& wpc)
		{
			return glm::vec3(wpc.position[0], wpc.position[1], wpc.position[2]);
		}

		/*!
			Sets the position of a WorldPositionComponent to the new position stored in the newPos vector.
			\param newPos The new position for the WorldPositionComponent.
			\param wpc The WorldPositionComponent to change.
		*/
		static void SetPosition(glm::vec3 newPos, WorldPositionComponent& wpc)
		{
			wpc.position[0] = newPos.x;
			wpc.position[1] = newPos.y;
			wpc.position[2] = newPos.z;
		}

		/*!
			Sets the position of a WorldPositionComponent to the new position stored in the x, y and z parameters.
			\param x The x component of the new position for the WorldPositionComponent.
			\param y The y component of the new position for the WorldPositionComponent.
			\param z The z component of the new position for the WorldPositionComponent.
			\param wpc The WorldPositionComponent to change.
		*/
		static void SetPosition(const float& x, const float& y, const float& z, WorldPositionComponent& wpc)
		{
			wpc.position[0] = x;
			wpc.position[1] = y;
			wpc.position[2] = z;
		}
	};
}

#endif