#ifndef SRC_CORE_COMPONENTFRAMEWORK_BASESYSTEM_H
#define SRC_CORE_COMPONENTFRAMEWORK_BASESYSTEM_H
#include "SystemTypes.hpp"

#include <vector>

namespace Core
{
     
    class BaseSystem
    {
    public:
        /*!
            Base constructor, takes inclusive and exclusive aspects
            used by the ChangedEntity function to determine if an entity 
            should be saved in the internal entity list.
        */
        BaseSystem( Aspect inclusive, Aspect exclusive );

        virtual ~BaseSystem() {}
        
        /*!
            Update call
        */
        virtual void Update( float delta ) = 0;

        /*!
            Changed entity, called every time entities are created, removed or if their
            Aspect has changed
        */

        void ChangedEntity( Entity id, Aspect old_asp, Aspect new_asp );

        bool AspectMatch( Aspect asp );
    protected:
        /*!
            Systems personal entities list.
        */
        std::vector<Entity> m_entities;

    private:
        Aspect m_inclusive, m_exclusive;

    };
}

#endif
