#ifndef SRC_CORE_SYSTEMS_FLOWFIELDSYSTEM_HPP
#define SRC_CORE_SYSTEMS_FLOWFIELDSYSTEM_HPP

#include <SystemDef.hpp>


namespace Core
{
	/*!
		System responsible for assigning respective direction from flowfield to agents.
	*/
    class FlowfieldSystem : public BaseSystem
    {

    public:
        virtual void Update( float delta ) override;

        FlowfieldSystem();

        virtual const char* GetHumanName() { return "FlowfieldSystem"; }
    };
}


#endif
