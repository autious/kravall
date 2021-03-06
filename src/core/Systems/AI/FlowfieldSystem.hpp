#ifndef SRC_CORE_SYSTEMS_FLOWFIELDSYSTEM_HPP
#define SRC_CORE_SYSTEMS_FLOWFIELDSYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>


namespace Core
{
	/*!
		System responsible for assigning respective direction from flowfield to agents.
	*/
    class FlowfieldSystem : public BaseSystem
    {
	private:
		void CalculatePathInDeadNode( Core::Entity rioter, int group );

    public:
        virtual void Update( float delta ) override;

        FlowfieldSystem();

        virtual const char* GetHumanName() { return "FlowfieldSystem"; }
    };
}


#endif
