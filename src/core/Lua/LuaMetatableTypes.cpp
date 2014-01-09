#include "LuaMetatableTypes.hpp"

#include <lauxlib.h>
#include <lualib.h>

namespace Core
{
    Entity* LuaUNewEntity( lua_State * L )
    {
        Entity * ent = (Entity*)lua_newuserdata( L, sizeof( Entity ) ); 
        luaL_getmetatable( L, ENTITY_META_TYPE );
        lua_setmetatable( L, -2 );

        return ent;
    }

    uint64_t* LuaUNewBitmask( lua_State * L )
    {
        uint64_t * data = (uint64_t*)lua_newuserdata( L, sizeof( uint64_t ));
        luaL_getmetatable( L, BITMASK_META_TYPE );
        lua_setmetatable( L, -2 );

        return data;
    }
}
