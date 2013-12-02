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

    protected:
        std::vector<Entity> m_entities;
    private:
        Aspect m_inclusive, m_exclusive;

        bool AspectMatch( Aspect asp );
    };
}

#endif
