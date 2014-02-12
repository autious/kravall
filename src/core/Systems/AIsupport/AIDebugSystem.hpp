#ifndef SRC_CORE_SYSTEMS_AIDEBUGGSYSTEM_HPP
#define SRC_CORE_SYSTEMS_AIDEBUGGSYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>


namespace Core
{
	/*!
		Debug system to host testcode for the AI framework.
	*/
    class AIDebugSystem : public BaseSystem
    {

    public:
        virtual void Update( float delta ) override;

        AIDebugSystem();

        virtual const char* GetHumanName() { return "AIDebugSystem"; }





    };
}

#endif
