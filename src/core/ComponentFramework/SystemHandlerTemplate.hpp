#ifndef SRC_CORE_COMPONENTFRAMEWORK_SYSTEMHANDLER_H
#define SRC_CORE_COMPONENTFRAMEWORK_SYSTEMHANDLER_H

#include "BaseSystem.hpp"
#include "PVector.hpp"
#include <array>

namespace Core
{
    /*!
        This shit be cray
    */
    template<typename... Args>
    class SystemHandlerTemplate
    {
    public: 
        static const int SYSTEM_COUNT = sizeof...(Args);

        SystemHandlerTemplate( )
        {
            m_systems = {{(new Args())...}};
        }

        ~SystemHandlerTemplate()
        {
        } 

        int GetSystemCount()
        {
            return SYSTEM_COUNT;
        }
    
        void CallChangedEntity( Entity id, Aspect old_asp, Aspect new_asp )
        {
            for( int i = 0; i < SYSTEM_COUNT; i++ )
            {
                m_systems[i].ChangedEntity( id, old_asp, new_asp );
            }
        };

    private:
        std::array<BaseSystem*,SYSTEM_COUNT> m_systems;
    };
}
#endif
