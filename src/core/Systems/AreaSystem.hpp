#ifndef SRC_CORE_SYSTEMS_AREALUAUPDATESYSTEM_H
#define SRC_CORE_SYSTEMS_AREALUAUPDATESYSTEM_H

#include <ComponentFramework/BaseSystem.hpp>

namespace Core
{
    class AreaSystem : public BaseSystem
    {
    public:
        virtual void Update( float delta ) override;

        AreaSystem();

        virtual const char* GetHumanName() { return "AreaSystem"; }
    private:
        void DrawAreas();

    };
}

#endif
