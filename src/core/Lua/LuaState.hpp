#ifndef SRC_CORE_LUA_MAINLUA_H
#define SRC_CORE_LUA_MAINLUA_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

namespace Core
{
    class LuaState
    {
        private:
            lua_State *m_state;
        public:
            LuaState();
            ~LuaState();

            void Execute( const char *filename ); 
    };
}

#endif
