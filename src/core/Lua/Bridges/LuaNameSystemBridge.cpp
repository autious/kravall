#include "LuaNameSystemBridge.hpp"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <World.hpp>

#include <Lua/LuaMetatableTypes.hpp>
#include <Lua/LuaUtility.hpp>

extern "C" 
{
    static int LuaGetEntitiesByName( lua_State *L )
    {
        Core::NameSystem *ns = Core::world.m_systemHandler.GetSystemT<Core::NameSystem>();

        std::vector<Core::Entity> ents = ns->GetEntitiesByName( luaL_checkstring( L, 1 ) );

        lua_newtable( L );
        int pos = 1;
        for(  std::vector<Core::Entity>::iterator it = ents.begin(); it != ents.end(); it++ )
        {
            LuaEntity *le = Core::LuaUNewLightEntity( L );
            le->entity = *it;
            lua_rawseti( L, -2, pos++ );
        }

        return 1;
    }

}

namespace Core
{
    LuaNameSystemBridge::LuaNameSystemBridge( lua_State * L )
    {
        lua_getglobal( L, "core" );
            lua_getfield(L, -1, "system" );
                lua_newtable( L );
                    luau_setfunction( L, "getEntitiesByName", LuaGetEntitiesByName );
                lua_setfield(L, -2, "name" );
            lua_pop( L, 2 );
    }
}
