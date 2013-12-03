#ifndef SRC_CORE_LUA_MAINLUA_H
#define SRC_CORE_LUA_MAINLUA_H

#include <lua.h>
#include <luaux.h>

namespace Core
{
    class LuaState
    {
        private:
            lua_State *m_state;

            LuaState( LuaState& s ) {}
        public:
            LuaState();
            ~LuaState();

            Execute( const char * filename );
    };
}

#endif
