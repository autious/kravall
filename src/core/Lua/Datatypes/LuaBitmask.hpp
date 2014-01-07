#ifndef SRC_CORE_LUA_DATATYPES_LUABITMASK_H
#define SRC_CORE_LUA_DATATYPES_LUABITMASK_H

#include <lua.h>

namespace Core
{
    class LuaBitmask
    {
        public:
            static void OpenLibs( lua_State * L );
    };
}

#endif
