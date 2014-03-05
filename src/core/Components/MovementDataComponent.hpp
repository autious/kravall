#ifndef SRC_CORE_COMPONENTS_MOVEMENT_DATA_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_MOVEMENT_DATA_COMPONENT_HPP

#include <float.h> // For FLT_MAX
#include <cassert>
#include <cmath>
#include <limits>
#include <glm/glm.hpp>

#define MOVEDTHISFRAME_THRESHOLD 0.1f

namespace Core
{

	/*!
		Component for additional movement related data.
	*/
	struct MovementDataComponent
	{

		/*! The position from the previos frame. Used to calculate perceived speed. */
		float prevPos[3];

		/*! The delta time for the previous frame, such that the movement speed for the unit can be accounted for correctly */
		float prevDt;
		
		/*! The distance unit per second this entity had moved since the last frame, also accounting for movement from collisions etc. */
		float frameSpeed;

		/*! If the entity has moved more than a certain distance in the last frame this value will be true. See macro MOVEDTHISFRAME_THRESHOLD */
		bool movedThisFrame;

		/*! Used to save pf influence vector from asynchronous calculations. Might not be updated every frame. */
		float oldPFDir[3];


		/*! Default constructor. Initialising all members to 0. */
		MovementDataComponent()
		{
			movedThisFrame = false;

			prevPos[0] = std::numeric_limits<float>::max();
			prevPos[1] = 0.0f;
			prevPos[2] = 0.0f;

			frameSpeed = 0.0f;
			prevDt = 1.0f;
		}

		inline static const char* GetName()
		{
			return "MovementDataComponent";
		}
	};
}
#endif
