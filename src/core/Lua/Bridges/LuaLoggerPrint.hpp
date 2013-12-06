#ifndef SRC_CORE_LUA_BRIDGES_LUALOGGERPRINT_H
#define SRC_CORE_LUA_BRIDGES_LUALOGGERPRINT_H

#include <lua.h>

namespace Core
{
    class LuaLoggerPrint
    {
        public:
            static void OpenLibs( lua_State * state );
    };
}

#endif
