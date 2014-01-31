#ifndef SRC_CORE_SYSTEMS_TARGETING_SYSTEM_HPP
#define SRC_CORE_SYSTEMS_TARGETING_SYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>
#include <Components/WorldPositionComponent.hpp>
#include <Components/UnitTypeComponent.hpp>

namespace Core
{
	/*!
	System for handling the movement of moving objects. Moving objects are defined by a WorldPositionComponent and
	a MovementComponent.
	*/
	class TargetingSystem : public BaseSystem
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
		TargetingSystem();

		/*!
			The name of the system for printing.
			\return The system's name.
		*/
		virtual const char* GetHumanName() { return "TargetingSystem"; }

	private:
		Entity m_lastClickedEntity;
		Entity m_currentTarget;
		bool m_wasFPressed;

		void HandlePoliceTargeting(Entity police);
		void HandleRioterTargeting(Entity rioter);
		Entity FindClosestTarget(WorldPositionComponent* origin, UnitType targetType);

		void TemporaryFunction();
	};
}

#endif