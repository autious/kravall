#ifndef SRC_CORE_SYSTEMS_AREALUAUPDATESYSTEM_H
#define SRC_CORE_SYSTEMS_AREALUAUPDATESYSTEM_H

#include <map>
#include <vector>

#include <ComponentFramework/BaseSystem.hpp>

namespace Core
{
    class AreaSystem : public BaseSystem
    {
    public:
        virtual void Update( float delta ) override;

        AreaSystem();

        virtual void ChangedEntity( Entity id, Aspect old_asp, Aspect new_asp );
        virtual const char* GetHumanName() { return "AreaSystem"; }

        std::vector<Entity> GetAreaEntities( Entity id );
    private:
        void DrawAreas();

        std::map<Entity,std::vector<Entity>> rioterContainList;
    };
}

#endif
