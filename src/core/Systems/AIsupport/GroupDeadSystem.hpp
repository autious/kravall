#ifndef SRC_CORE_SYSTEMS_AISUPPORT_GROUPDEADSYSTEM_H
#define SRC_CORE_SYSTEMS_AISUPPORT_GROUPDEADSYSTEM_H

#include <ComponentFramework/BaseSystem.hpp>

#include <glm/glm.hpp>

#include <vector>

namespace Core
{
    /*!
        Primary system for keeping track on how many dead members a group has
    */
    class GroupDeadSystem : public BaseSystem
    {
    public:
        GroupDeadSystem();

        virtual void Update( float delta ) override;

        virtual const char* GetHumanName() { return "GroupDeadSystem"; }

        int GetDeadUnitCount( );
    };
}

#endif
