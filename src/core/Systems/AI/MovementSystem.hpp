#ifndef SRC_CORE_SYSTEMS_MOVEMENT_SYSTEM_HPP
#define SRC_CORE_SYSTEMS_MOVEMENT_SYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>

namespace Core
{

	struct MovementComponent;

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
		MovementSystem();

		/*! 
			The name of the system for printing.
			\return The system's name.
		*/
		virtual const char* GetHumanName() { return "MovementSystem"; }

	private:
		static const float TURN_FACTOR;

		void InterpolateDirections(MovementComponent* mc, float delta);
	};
}

#endif