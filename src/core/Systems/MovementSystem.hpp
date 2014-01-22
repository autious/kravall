#ifndef SRC_CORE_SYSTEMS_MOVEMENT_SYSTEM_HPP
#define SRC_CORE_SYSTEMS_MOVEMENT_SYSTEM_HPP

#include <SystemDef.hpp>

#define POLICE_GOAL_ARRIVAL_THRESHOLD 1.0f

namespace Core
{
	/*!
		System for handling the movement of moving objects. Moving objects are defined by a WorldPositionComponent and
		a MovementComponent.
	*/
	class MovementSystem : public BaseSystem
	{

	public:
		/*! 
			System update function. All logic for the system is placed here.
			\param delta The delta time for the frame.
		*/
		virtual void Update(float delta) override;

		/*! 
			Default constructor creating a system. The system works on entities containing a WorldPositioncomponent and
			a MovementComponent.
		*/
		MovementSystem() : BaseSystem(EntityHandler::GenerateAspect<WorldPositionComponent, MovementComponent>(), 0ULL)
		{}

		/*! 
			The name of the system for printing.
			\return The system's name.
		*/
		virtual const char* GetHumanName() { return "MovementSystem"; }

	private:
		void InterpolateDirections(MovementComponent* mc, float delta);
	};
}

#endif