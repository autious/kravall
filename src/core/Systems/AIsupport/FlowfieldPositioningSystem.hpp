#ifndef SRC_CORE_SYSTEMS_FLOWFIELDPOSITIONINGSYSTEM_HPP
#define SRC_CORE_SYSTEMS_FLOWFIELDPOSITIONINGSYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>


namespace Core
{
	/*!
		System responsible for assigning flowfield influence to agents and
		check their position vs. the navigation mesh.
	*/
    class FlowfieldPositioningSystem : public BaseSystem
    {

    public:
        virtual void Update( float delta ) override;

        FlowfieldPositioningSystem();

        virtual const char* GetHumanName() { return "FlowfieldPositioningSystem"; }
    };
}


#endif
