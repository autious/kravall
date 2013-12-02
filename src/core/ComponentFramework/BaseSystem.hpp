#ifndef SRC_CORE_COMPONENTFRAMEWORK_BASESYSTEM_H
#define SRC_CORE_COMPONENTFRAMEWORK_BASESYSTEM_H
#include "SystemTypes.hpp"

#include <vector>

namespace Core
{
     
    class BaseSystem
    {
    public:
        BaseSystem( Aspect inclusive, Aspect exclusive );
        virtual ~BaseSystem() {}

        void ChangedEntity( Entity id, Aspect old_asp, Aspect new_asp );
    private:
        Aspect m_inclusive, m_exclusive;
        std::vector<Entity> m_entities;

        bool AspectMatch( Aspect asp );
    };
}

#endif
