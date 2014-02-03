#include "LuaAnimationBridge.hpp"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <World.hpp>
#include <Animation/AnimationManager.hpp>

#include <Lua/LuaMetatableTypes.hpp>
#include <Lua/LuaUtility.hpp>

extern "C" 
{
    static int LuaPlayAnimation( lua_State *L )
    {
		LuaEntity* ent = luau_checkentity(L, 1);
		const char* name = luaL_checkstring(L, 2);
		
		Core::AnimationManager::PlayAnimation(ent->entity, name);

        return 0;
    }

    static int LuaLoopAnimation( lua_State *L )
    {
		LuaEntity* ent = luau_checkentity(L, 1);
		const char* name = luaL_checkstring(L, 2);
		
		Core::AnimationManager::LoopAnimation(ent->entity, name);

        return 0;
    }

}

namespace Core
{
    LuaAnimationBridge::LuaAnimationBridge( lua_State * L )
    {
		int sanity = lua_gettop(L);
        lua_getglobal( L, "core" );
            lua_newtable( L );
                luau_setfunction( L, "play", LuaPlayAnimation );
                luau_setfunction( L, "loop", LuaLoopAnimation );
            lua_setfield(L, -2, "animations" );
        lua_pop( L, 1 );

		assert(sanity==lua_gettop(L));
    }
}

//{
//    LuaNameSystemBridge::LuaNameSystemBridge( lua_State * L )
//    {
//        lua_getglobal( L, "core" );
//            lua_getfield(L, -1, "system" );
//                lua_newtable( L );
//                    luau_setfunction( L, "getEntitiesByName", LuaGetEntitiesByName );
//                lua_setfield(L, -2, "name" );
//            lua_pop( L, 2 );
//    }
//}