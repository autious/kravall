#ifndef SRC_CORE_SYSTEMS_POLICEGOALSYSTEM_HPP
#define SRC_CORE_SYSTEMS_POLICEGOALSYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>


namespace Core
{
	/*!
		Will set the police to go to its target if target path is clear.
		Otherwise police should use flowfield to get to the target.
	*/
    class PoliceGoalSystem : public BaseSystem
    {

    public:
        virtual void Update( float delta ) override;

        PoliceGoalSystem();

        virtual const char* GetHumanName() { return "PoliceGoalSystem"; }
    };
}

#endif
