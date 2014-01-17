#ifndef SRC_CORE_SYSTEMS_NAMESYSTEM_H
#define SRC_CORE_SYSTEMS_NAMESYSTEM_H

#include <ComponentFramework/BaseSystem.hpp>

namespace Core
{
    class NameSystem : public BaseSystem
    {
    public:
        virtual void Update( float delta ) override;

        NameSystem();

        virtual const char* GetHumanName() { return "NameSystem"; }
    private:

    };
}

#endif
