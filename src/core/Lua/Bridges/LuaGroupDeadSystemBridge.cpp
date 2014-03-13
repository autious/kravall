#include "LuaGroupDeadSystemBridge.hpp"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <Lua/LuaUtility.hpp>
#include <Lua/LuaMetatableTypes.hpp>
#include <Systems/AIsupport/GroupDeadSystem.hpp>
#include <World.hpp>

#include <logger/Logger.hpp>

extern "C"
{
    static int LuaGetDeadUnitCount( lua_State *L )
    {
        Core::GroupDeadSystem* groupDeadSystem = Core::world.m_systemHandler.GetSystem<Core::GroupDeadSystem>();
        lua_pushinteger(L, groupDeadSystem->GetDeadUnitCount( ) );
        return 1;
    }
    /*
    static int LuaGetDeadMembers( lua_State *L )
    {
        int groupID = luaL_checkinteger( L, 1 );
        Core::GroupDeadSystem* groupDeadSystem = Core::world.m_systemHandler.GetSystem<Core::GroupDeadSystem>();

        std::vector<Core::Entity> members = groupDeadSystem->GetDeadMembers( groupID );
         
        lua_newtable( L );
        for( int i = 0; i < members.size(); i++ )
        {
            Core::LuaUNewLightEntity( L, members[i] ); 
            lua_rawseti( L, -2, i+1 ); 
        }
        return 1;
    }

    static int LuaGetDeadMemberCount( lua_State *L )
    {
        int groupID = luaL_checkinteger( L, 1 );
        Core::GroupDeadSystem* groupDeadSystem = Core::world.m_systemHandler.GetSystem<Core::GroupDeadSystem>();
        lua_pushinteger(L, groupDeadSystem->GetDeadMemberCount( groupID ) );
    
        return 1;
    }
    */
}

namespace Core
{
    LuaGroupDeadSystemBridge::LuaGroupDeadSystemBridge( lua_State * L )
    {
        lua_getglobal( L, "core" );
            lua_getfield( L,-1, "system" );
                lua_newtable(L);
                    luau_setfunction( L, "getDeadUnitCount", LuaGetDeadUnitCount );
                lua_setfield( L, -2, "groupDead");
        lua_pop( L,2 );
    }
}
