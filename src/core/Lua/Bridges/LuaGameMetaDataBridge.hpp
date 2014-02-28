#ifndef SRC_CORE_LUA_BRIDGES_LUAGAMEMETADATABRIDGE_HPP
#define SRC_CORE_LUA_BRIDGES_LUAGAMEMETADATABRIDGE_HPP

#include <lua.h>


namespace Core
{
    class LuaGameMetaDataBridge
    {
    public:
         LuaGameMetaDataBridge( lua_State * L );
    };
}

#endif
