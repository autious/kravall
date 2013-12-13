#ifndef SRC_CORE_SYSTEMS_EXAMPLESYSTEM_HPP
#define SRC_CORE_SYSTEMS_EXAMPLESYSTEM_HPP

#include <SystemDef.hpp>


namespace Core
{
    class ExampleSystem : public BaseSystem
    {

    public:
        virtual void Update( float delta ) override;

        ExampleSystem() : BaseSystem( EntityHandler::GenerateAspect<ExampleComponent1,ExampleComponent2>(), 0ULL )
        {}

        virtual const char* GetHumanName() { return "ExampleSystem"; }
    };
}

#endif
