#ifndef SRC_CORE_SYSTEMS_PRIORITY_RESET_HPP
#define SRC_CORE_SYSTEMS_PRIORITY_RESET_HPP

#include <ComponentFramework/BaseSystem.hpp>

namespace Core
{
	/*!
		This system is responsible for resetting the priority-values of all componenets. 
		This system is meant to be run either first or last in every frame.
	*/
	class PriorityResetSystem : public BaseSystem
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
		PriorityResetSystem();

		/*!
			The name of the system for printing.
			\return The system's name.
		*/
		virtual const char* GetHumanName() { return "PriorityResetSystem"; }

	private:
		



	};
}

#endif