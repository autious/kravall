#ifndef SRC_CORE_COMPONENTFRAMEWORK_SYSTEMHANDLER_H
#define SRC_CORE_COMPONENTFRAMEWORK_SYSTEMHANDLER_H

#include "BaseSystem.hpp"
#include "PVector.hpp"

#include <array>
#include <utility>
#include <chrono>
#include <vector>

#define GNAME( name ) #name

namespace Core
{
    /*!
        SystemHandler, stores systems, calls them and handles callbacks from EntityHandler to Systems.
        Can be created and called every frame to apply the registered systems transformation on 
        registered entities
    */
    template<typename... Args>
    class SystemHandlerTemplate
    {
    public: 
        static const int SYSTEM_COUNT = sizeof...(Args);

        /*!
            Standard constructur, creates systemhandler
        */
        SystemHandlerTemplate( )
        {
            m_systems = {{(new Args())...}};
        }

        ~SystemHandlerTemplate()
        {
        } 

        /*!
            Returns how many systems were compiled into the systemhandler
        */
        int GetSystemCount()
        {
            return SYSTEM_COUNT;
        }

        /*!
            Main update loop, called every frame to update all systems
        */
        void Update( float delta )
        {
            
            for( int i = 0; i < SYSTEM_COUNT; i++ )
            {
                auto start = std::chrono::high_resolution_clock::now();
                 
                m_systems[i]->Update( delta );

                auto end = std::chrono::high_resolution_clock::now();

                std::chrono::microseconds diff = std::chrono::duration_cast<std::chrono::microseconds>( end - start );

                m_frameTimes[i] = diff; 
            }          
        }
    
        /*!
            Intended to be called by EntityHandler when entities are created, modified or removed.
        */
        void CallChangedEntity( Entity id, Aspect old_asp, Aspect new_asp )
        {
            for( int i = 0; i < SYSTEM_COUNT; i++ )
            {
                m_systems[i]->ChangedEntity( id, old_asp, new_asp );
            }
        };

        /*!
            This function primarily exist for testing purposes,
            don't use it without thinking about it first.
        */
        BaseSystem *GetSystem( int id )
        {
            return m_systems[id];
        }

        std::vector<std::pair<const char*,std::chrono::microseconds>> GetFrameTime()
        {
            std::vector<std::pair<const char*,std::chrono::microseconds>> ar;

            for( int i = 0; i < SYSTEM_COUNT; i++ )
            {
                ar.push_back( std::pair<const char*, std::chrono::microseconds>( m_systems[i]->GetHumanName(), m_frameTimes[i] ) );
            }

            return ar;
        }

    private:
        std::array<BaseSystem*,SYSTEM_COUNT> m_systems;
        std::array<std::chrono::microseconds,SYSTEM_COUNT> m_frameTimes;
    };
}
#endif
