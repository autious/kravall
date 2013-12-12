#ifndef SRC_CORE_SYSTEMS_PICKINGSYSTEM_HPP
#define SRC_CORE_SYSTEMS_PICKINGSYSTEM_HPP

#include <SystemDef.hpp>


namespace Core
{
    class PickingSystem : public BaseSystem
    {

    public:
        virtual void Update( float delta ) override;

        PickingSystem() : BaseSystem( EntityHandler::GenerateAspect<ExampleComponent1,ExampleComponent2>(), 0ULL )
        {}

        virtual const char* GetHumanName() { return "ExampleSystem"; }
    };
}

#endif
