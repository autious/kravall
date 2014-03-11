#ifndef SRC_CORE_COMPONENTS_MOVEMENT_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_MOVEMENT_COMPONENT_HPP

#include <float.h> // For FLT_MAX
#include <cassert>
#include <cmath>
#include <limits>
#include <glm/glm.hpp>
#include <memory>
#include <cstring>

#define MOVEDTHISFRAME_THRESHOLD 0.1f

namespace Core
{
	/*!
		State used to set the respective movement properties.
	*/
	enum MovementState
	{
		Movement_idle,
		Movement_Walking,
        Movement_Jogging,
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
		TargetingGoalPriority,
		StopGoalPriority,
	};

	/*!
		Used to determine who has the stronger case when several systems want to change movementState in the same frame.
	*/
	enum MovementStatePriority : short
	{
		MovementState_NoPriority,
		MovementState_MovementSystemPriority,
		MovementState_RioterGoalSystemPriority,
		MovementState_PoliceGoalSytemPriority,
		MovementState_SquadMoveInFormationPriority,
		MovementState_FleeAnimationPriority,
		MovementState_CombatAnimationPriority,		
	};

	/*!
		Component holding a moving object's speed, maximum speed, direction of movement and goal related data.
	*/
	struct MovementComponent
	{
		/*! The object's current speed. */
		float speed;

		/*! The object's desired speed for each movement state. The speed will attempt to reach this speed. */
		float desiredSpeed[ MovementState::MOVEMENTSTATE_COUNT ];

		/*! 
			Should NEVER be set directly - use the static function SetDirection instead. An array specifying the 
			object's direction of movement where index 0 = x, index 1 = y and index 2 = z. 
		*/
		float direction[3];

		/*! An array specifying the object's new direction of movement where index 0 = x, index 1 = y and index 2 = z. */
		float newDirection[3];

		/*! Priority value for the current set goal. If new value has higher priority the current value may be repalced. */
		MovementGoalPriority currentGoalPriority;

		/*! Priority value for the current movement state. If new state has higher priority the current value may be repalced. */
		MovementStatePriority currentMovementStatePriority;

		/*! The preferred normal movement state of the unit. */
		Core::MovementState preferredState;

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
		MovementComponent() : speed(0.0f), currentGoalPriority( MovementGoalPriority::NoGoalPriority ),
			currentMovementStatePriority( MovementStatePriority::MovementState_NoPriority )
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
			preferredState = MovementState::Movement_Walking;

			std::memset( desiredSpeed, 0, sizeof(float) * MovementState::MOVEMENTSTATE_COUNT );
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

		inline void SetMovementState( MovementState newState, MovementStatePriority prio )
		{
			if( currentMovementStatePriority <= prio )
			{
				state = newState;
				currentMovementStatePriority = prio;
			}
		}

	};
}
#endif
