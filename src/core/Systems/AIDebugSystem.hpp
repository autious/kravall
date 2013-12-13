#ifndef SRC_CORE_SYSTEMS_AIDEBUGGSYSTEM_HPP
#define SRC_CORE_SYSTEMS_AIDEBUGGSYSTEM_HPP

#include <SystemDef.hpp>


namespace Core
{
	/*!
		Debug system to host testcode for the AI framework.
	*/
    class AIDebugSystem : public BaseSystem
    {

    public:
        virtual void Update( float delta ) override;

        AIDebugSystem() : BaseSystem( EntityHandler::GenerateAspect< WorldPositionComponent, BoundingVolumeComponent >(), 0ULL )
        {}

        virtual const char* GetHumanName() { return "AIDebugSystem"; }





    };
}

#endif
