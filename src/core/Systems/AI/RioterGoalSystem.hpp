#ifndef SRC_CORE_SYSTEMS_RIOTERGOALSYSTEM_HPP
#define SRC_CORE_SYSTEMS_RIOTERGOALSYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>


namespace Core
{
	/*!
		Will handle rioters who has specific goals.
	*/
    class RioterGoalSystem : public BaseSystem
    {

    public:
        virtual void Update( float delta ) override;

        RioterGoalSystem();

        virtual const char* GetHumanName() { return "RioterGoalSystem"; }
    };
}

#endif
