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
	
    static int LuaQueueAnimation( lua_State *L )
    {
		LuaEntity* ent = luau_checkentity(L, 1);
		const char* name = luaL_checkstring(L, 2);
		bool loop = lua_toboolean(L, 3);
		
		Core::AnimationManager::QueueAnimation(ent->entity, name, loop);

        return 0;
    }

    static int LuaPauseAnimation( lua_State *L )
    {
		LuaEntity* ent = luau_checkentity(L, 1);
		
		Core::AnimationManager::PauseAnimation(ent->entity);

        return 0;
    }
	
    static int LuaResumeAnimation( lua_State *L )
    {
		LuaEntity* ent = luau_checkentity(L, 1);
		
		Core::AnimationManager::ResumeAnimation(ent->entity);

        return 0;
    }

    static int LuaSetAnimationSpeed( lua_State *L )
    {
		LuaEntity* ent = luau_checkentity(L, 1);
		float speed = luaL_checknumber(L, 2);
		
		Core::AnimationManager::SetAnimationSpeed(ent->entity, speed);

        return 0;
    }

    static int LuaSetAnimationTime( lua_State *L )
    {
		LuaEntity* ent = luau_checkentity(L, 1);
		float time = luaL_checknumber(L, 2);
		
		Core::AnimationManager::SetAnimationTime(ent->entity, time);

        return 0;
    }

    static int LuaIsAnimationPlaying( lua_State *L )
    {
		LuaEntity* ent = luau_checkentity(L, 1);
		
		lua_pushboolean( L, Core::AnimationManager::IsAnimationPlaying(ent->entity));

        return 1;
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
                luau_setfunction( L, "queue", LuaQueueAnimation );
                luau_setfunction( L, "pause", LuaPauseAnimation );
                luau_setfunction( L, "resume", LuaResumeAnimation );
                luau_setfunction( L, "setSpeed", LuaSetAnimationSpeed );
                luau_setfunction( L, "setTime", LuaSetAnimationTime );
                luau_setfunction( L, "isPlaying", LuaIsAnimationPlaying );
            lua_setfield(L, -2, "animations" );
        lua_pop( L, 1 );

		assert(sanity==lua_gettop(L));
    }
}
