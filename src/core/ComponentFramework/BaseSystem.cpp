#include "BaseSystem.hpp"
#include <cassert>

#include <iostream>

Core::BaseSystem::BaseSystem( Aspect inclusive, Aspect exclusive )
{
    m_inclusive = inclusive;
    m_exclusive = exclusive;
}

void Core::BaseSystem::ChangedEntity( Entity id, Aspect old_asp, Aspect new_asp )
{
    //Remove if old matches
    if( AspectMatch( old_asp ) )
    {
        bool found = false;
        for( std::vector<Entity>::iterator it = m_entities.begin();
             it != m_entities.end() && found == false;
             it++ )
        {
            if( *it == id )
            {
                found = true;
                m_entities.erase( it );
            }
        }

        assert( m_inclusive == 0 || found );
    }

    //Add if new matches
    if( AspectMatch( new_asp ) )
    {
        m_entities.push_back( id );
    }
}

bool Core::BaseSystem::AspectMatch( Aspect asp )
{
    return ((m_inclusive & asp) == m_inclusive) && ((m_exclusive & asp) == 0 );
}
