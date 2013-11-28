#ifndef SRC_CORE_COMPONENTFRAMEWORK_SYSTEMHANDLER_H
#define SRC_CORE_COMPONENTFRAMEWORK_SYSTEMHANDLER_H

#include "BaseSystem.hpp"
#include "PVector.hpp"

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
            AddSystems<Args...>( SYSTEM_COUNT ); 
        }

        ~SystemHandlerTemplate()
        {
            for( int i = 0; i < SYSTEM_COUNT; i++ )
            {
                delete systemList[i];
            }
        } 

        int GetSystemCount();

    private:
        template<typename A, typename... Systems>
        void AddSystems(int size)
        {
            systemList[SYSTEM_COUNT-size] = new A();
            AddSystems<Systems...>(size-1);
        }

        template<typename A>
        void AddSystems(int size)
        {
            systemList[SYSTEM_COUNT-size] = new A();
        }

        BaseSystem *systemList[SYSTEM_COUNT];
    };
}
#endif
