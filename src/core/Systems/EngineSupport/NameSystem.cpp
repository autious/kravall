#include "NameSystem.hpp"

#include <SystemDef.hpp>
#include <World.hpp>

namespace Core
{
    NameSystem::NameSystem()
        : BaseSystem( EntityHandler::GenerateAspect<NameComponent>(), 0ULL )
    {
    }

    void NameSystem::Update( float delta )
    {
    }

    /* This won't work as the name of a component might change!
    virtual void ChangedEntity( Entity id, Aspect old_asp, Aspect new_asp )
    {
        BaseSystem::ChangedEntity( id, old_asp, new_asp );

        NameComponent *nc = WGETC<NameComponent>(id);

        Aspect ncAspect = EntityHandler::GenerateAspect<NameComponent>();

        //Remove entity, should guaranteed exist.
        // Entity HAD a name component,doesn't anymore.
        if( (old_asp & ncAspect) == ncAspect && (new_asp & ncAspect) == 0ULL )
        {
            namemap.erase( namemap.find( std::stringn(c->name) ) );
        }
        //Add entity, should guaranteed not have existed before
        // Entity DIDN'T have a name component, but does now.
        else if( (old_asp & ncAspect) == 0ULL && (new_asp & ncAspect) == ncAspect )
        {
            namemap[nc->name]
        }
    }
    */

    std::vector<Entity> NameSystem::GetEntitiesByName( const char * name )
    {
        std::vector<Entity> ents;

	    for (std::vector<Entity>::iterator it = m_entities.begin();
            it != m_entities.end();
            it++)
        {
            NameComponent *nc = WGETC<NameComponent>(*it);

            std::string truncV = NameComponent::GetTrunc( name );
            if( strcmp( nc->name, truncV.c_str() ) == 0 ) 
                ents.push_back( *it );
        }

        return ents;
    }
}
