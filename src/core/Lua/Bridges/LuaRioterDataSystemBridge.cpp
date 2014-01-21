#include "LuaRioterDataSystemBridge.hpp"

#include <Lua/LuaUtility.hpp>
#include <Lua/LuaMetatableTypes.hpp>
#include <World.hpp>

extern "C"
{
    static int LuaGetRiotersInGroupCount( lua_State * L )
    {
        Core::RioterDataSystem *as = Core::world.m_systemHandler.GetSystemT<Core::RioterDataSystem>();
        lua_pushinteger( L, as->GetRioterInGroupCount( luaL_checkinteger( L, 1 ) ) );
    
        return 1;
    }

    static int LuaGetRiotersInGroup( lua_State * L)
    {
        Core::RioterDataSystem *as = Core::world.m_systemHandler.GetSystemT<Core::RioterDataSystem>();

        std::vector<Core::Entity> ents = as->GetRiotersInGroup( luaL_checkinteger(L, 1 ) );

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
    LuaRioterDataSystemBridge::LuaRioterDataSystemBridge( lua_State *L ) 
    {
        lua_getglobal( L, "core" );
            lua_getfield( L, -1, "system" );
                lua_newtable( L );
                    luau_setfunction( L, "getRiotersInGroupCount", LuaGetRiotersInGroupCount );
                    luau_setfunction( L, "getRiotersInGroup", LuaGetRiotersInGroup );
    
                lua_setfield( L, -2, "rioterData" );
        lua_pop( L, 2 );
    }
}
