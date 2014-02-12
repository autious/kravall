#ifndef SRC_CORE_COMPONENTS_MOVEMENT_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_MOVEMENT_COMPONENT_HPP

#include <float.h> // For FLT_MAX
#include <cassert>
#include <cmath>
#include <limits>
#include <glm/glm.hpp>


namespace Core
{
	/*!
		State used to set the respective movement properties.
	*/
	enum MovementState
	{
		Movement_Idle,
		Movement_Walking,
		Movement_Sprinting,
		MOVEMENTSTATE_COUNT,
	};

	/*!
		Used to determine who has the stonger case when several systems want to change the goal in the same frame.
	*/
	enum MovementGoalPriority : short
	{
		NoGoalPriority,
		FormationGoalPriority,
		AttackGoalPriority,
	};

	/*!
		Used to determine who has the stronger case when several systems want to change desiredSpeed in the same frame.
	*/
	enum DesiredSpeedSetPriority : short
	{
		NoDesiredSpeedPriority,
		PoliceGoalSytemDesiredSpeedPriority,
		SquadMoveInFormationDesiredSpeedPriority,
		CombatAnimationDesiredSpeedPriority,
	};

	/*!
		Component holding a moving object's speed, maximum speed and direction of movement.
	*/
	struct MovementComponent
	{
		/*! The object's current speed. */
		float speed;

		/*! The object's desired speed. The speed will attempt to reach this speed. */
		float desiredSpeed;

		/*! 
			Should NEVER be set directly - use the static function SetDirection instead. An array specifying the 
			object's direction of movement where index 0 = x, index 1 = y and index 2 = z. 
		*/
		float direction[3];

		/*! An array specifying the object's new direction of movement where index 0 = x, index 1 = y and index 2 = z. */
		float newDirection[3];

		/*! Priority value for the current set goal. If new value has higher priority the current value may be repalced. */
		MovementGoalPriority currentGoalPriority;

		/*! Priority value for the current desiredSpeed. If new value has higher priority the current value may be repalced. */
		DesiredSpeedSetPriority currentDesiredSpeedPriority;

		/*!
			An array specifying the object's goal, ceasing movement when the goal is reached. The index 0 corresponds
			to the x component, 1 to y and 2 to z. An x-value of FLT_MAX disables the goal.
		*/
		float goal[3];

		/*! The Navigation mesh node index in which the goal resides. */
		int NavMeshGoalNodeIndex;

		/*! State used when resetting movement data, eg. when changing states. */
		MovementState state;

		/*! Default constructor. Initialising all members to 0. */
		MovementComponent() : speed(0.0f), desiredSpeed(0.0f), currentGoalPriority( MovementGoalPriority::NoGoalPriority ), 
			currentDesiredSpeedPriority( DesiredSpeedSetPriority::NoDesiredSpeedPriority )
		{
			direction[0] = 0.0f;
			direction[1] = 0.0f;
			direction[2] = 0.0f;

			newDirection[0] = 0.0f;
			newDirection[1] = 0.0f;
			newDirection[2] = 0.0f;

			goal[0] = std::numeric_limits<float>::max();
			goal[1] = 0.0f;
			goal[2] = 0.0f;

			NavMeshGoalNodeIndex = -1;

			state = MovementState::Movement_Walking;
		}

		/*!
			Constructor setting all members to starting values.
			\param dirX The x-component of the movement direction vector.
			\param dirY The y-component of the movement direction vector.
			\param dirZ The z-component of the movement direction vector.
			\param startSpeed The initial speed of the object.
		*/
		MovementComponent(const float& dirX, const float& dirY, const float& dirZ, const float& startSpeed)
			: speed(startSpeed), desiredSpeed(startSpeed)
		{
			direction[0] = dirX;
			direction[1] = dirY;
			direction[2] = dirZ;

			newDirection[0] = 0.0f;
			newDirection[1] = 0.0f;
			newDirection[2] = 0.0f;

			goal[0] = FLT_MAX;
			goal[1] = 0.0f;
			goal[2] = 0.0f;

			state = MovementState::Movement_Walking;
		}

		inline static const char* GetName()
		{
			return "MovementComponent";
		}

		static void SetDirection(MovementComponent* mc, const float& newX, const float& newY, const float& newZ)
		{
			// Make sure vector is normalised.
			assert((newX == 0.0f && newY == 0.0f && newZ == 0.0f) || ((std::sqrt(newX * newX + newY * newY + newZ * newZ) - 1.0) < 0.01));

			mc->newDirection[0] = newX;
			mc->newDirection[1] = newY;
			mc->newDirection[2] = newZ;
		}

		inline void SetGoal( glm::vec3 newGoal, int node, MovementGoalPriority prio )
		{
			if( currentGoalPriority <= prio )
			{
				currentGoalPriority = prio; 
				goal[0] = newGoal.x;
				goal[1] = newGoal.y;
				goal[2] = newGoal.z;
				NavMeshGoalNodeIndex = node;
			}
		}

		inline void SetGoal( float newGoal[3], int node, MovementGoalPriority prio )
		{
			if( currentGoalPriority <= prio )
			{
				currentGoalPriority = prio;
				goal[0] = newGoal[0];
				goal[1] = newGoal[1];
				goal[2] = newGoal[2];
				NavMeshGoalNodeIndex = node;
			}
		}

		inline void SetDesiredSpeed( float speed, DesiredSpeedSetPriority prio )
		{
			if( currentDesiredSpeedPriority <= prio )
			{
				desiredSpeed = speed;
				currentDesiredSpeedPriority = prio;
			}
		}

	};
}
#endif
