#ifndef SRC_CORE_COMPONENTFRAMEWORK_SYSTEMHANDLER_H
#define SRC_CORE_COMPONENTFRAMEWORK_SYSTEMHANDLER_H

#include "BaseSystem.hpp"

namespace Core
{
    /*!
        This shit be cray
    */
    template<typename... Args>
    class SystemHandler
    {
    public: 
        SystemHandler( )
        {
            AddSystems<Args...>( sizeof...(Args) ); 
        }

        ~SystemHandler()
        {
            for( int i = 0; i < sizeof...(Args); i++ )
            {
                delete systemList[i];
            }
        } 

        int GetSystemCount();

        static const int SYSTEM_COUNT = sizeof...(Args);
    private:
        template<typename A, typename... Systems>
        void AddSystems(int size)
        {
            systemList[sizeof...(Args)-size] = new A();
            AddSystems<Systems...>(size-1);
        }

        template<typename A>
        void AddSystems(int size)
        {
            systemList[sizeof...(Args)-size] = new A();
        }

        BaseSystem *systemList[sizeof...(Args)];
    };
}
#endif
