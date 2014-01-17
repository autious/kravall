#ifndef SRC_CORE_SYSTEMS_AREALUAUPDATESYSTEM_H
#define SRC_CORE_SYSTEMS_AREALUAUPDATESYSTEM_H

#include <ComponentFramework/BaseSystem.hpp>

namespace Core
{
    class AreaLuaUpdateSystem : public BaseSystem
    {
    public:
        virtual void Update( float delta ) override;

        AreaLuaUpdateSystem();

        virtual const char* GetHumanName() { return "AreaLuaUpdateSystem"; }
    private:
        void DrawAreas();

    };
}

#endif
