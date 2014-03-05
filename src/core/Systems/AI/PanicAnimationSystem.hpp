#ifndef SRC_CORE_SYSTEMS_PANIC_ANIMATION_SYSTEM_HPP
#define SRC_CORE_SYSTEMS_PANIC_ANIMATION_SYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>

namespace Core
{
	/*!
		System for handling reaction-animations and movementspeed for units.
	*/
	class PanicAnimationSystem : public BaseSystem
	{

	public:
		/*!
			System update function. All logic for the system is placed here.
			\param delta The delta time for the frame.
		*/
		virtual void Update(float delta) override;

		/*!
			Constructor, does nothing besides defining target components.
		*/ 
		PanicAnimationSystem();

		/*!
			The name of the system for printing.
			\return The system's name.
		*/
		virtual const char* GetHumanName() { return "PanicAnimationSystem"; }

	private:
		



	};
}

#endif