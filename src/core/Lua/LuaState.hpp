#ifndef SRC_CORE_LUA_MAINLUA_H
#define SRC_CORE_LUA_MAINLUA_H

#include <lua.h>

namespace Core
{
    class ConfigurationHandler;
    class LuaState
    {
        private:
            friend class ConfigurationHandler;
            lua_State *m_state;
        public:
            LuaState();
            ~LuaState();

            void Execute( const char *filename ); 
            void DoBlock( const char *block );
    };
}
#endif
