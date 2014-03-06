#include "LuaTimerBridge.hpp"

#include <Lua/LuaMetatableTypes.hpp>
#include <Lua/LuaUtility.hpp>
#include <Timer.hpp>
#include <cstdio>
#include <sstream>
#include <iomanip>
#include <algorithm>

extern "C"
{
    static int LuaCreateTimer( lua_State * L )
    {
        luau_assert( L, lua_gettop(L) == 0 );
        LuaTimer * timer = LuaUNewTimer( L, new Core::HighresTimer() );        

        return 1;
    }

    static int LuaTimerStart( lua_State * L )
    {
        luau_assert( L, lua_gettop(L) == 1 );
       
        LuaTimer * timer = luau_checktimer( L, 1 );   

        timer->timer->Start();
        return 0;
    }

    static int LuaTimerStop( lua_State * L )
    {
        luau_assert( L, lua_gettop(L) == 1 );
       
        LuaTimer * timer = luau_checktimer( L, 1 );   

        timer->timer->Stop();
        return 0;
    }

    static int LuaTimerGetDeltaMicroseconds( lua_State * L )
    {
        luau_assert( L, lua_gettop(L) == 1 );
       
        LuaTimer * timer = luau_checktimer( L, 1 );   

        lua_pushnumber( L, std::chrono::duration_cast<std::chrono::microseconds>(timer->timer->GetDelta()).count() );
        return 1;
    }

    static int LuaTimerToString( lua_State * L )
    {
        luau_assert( L, lua_gettop(L) == 1 );
       
        LuaTimer * timer = luau_checktimer( L, 1 );   
       
        std::stringstream ss;

        ss << std::fixed << std::setw( 7 ) << std::setprecision(4) << std::setfill( '0' ) << std::chrono::duration_cast<std::chrono::microseconds>(timer->timer->GetDelta()).count() / 1000.0f << "ms";

        lua_pushstring( L, ss.str().c_str() );

        return 1;
    }

    static int LuaTimerGC( lua_State * L )
    {
        luau_assert( L, lua_gettop(L) == 1 );
       
        LuaTimer * timer = luau_checktimer( L, 1 );   

        if( timer->timer != nullptr )
        {
            delete timer->timer;
        }
        timer->timer = nullptr;

        return 0;
    }
}

namespace Core
{
    LuaTimerBridge::LuaTimerBridge( lua_State * L )
    {
        int sanity = lua_gettop( L );

        lua_getglobal( L, "core" );
            lua_newtable( L );
                int timerTable = lua_gettop( L );
                luau_setfunction( L, "createTimer", LuaCreateTimer );
                luau_setfunction( L, "start", LuaTimerStart );
                luau_setfunction( L, "stop", LuaTimerStop );
                luau_setfunction( L, "getDeltaMicroseconds", LuaTimerGetDeltaMicroseconds );
                    luaL_newmetatable( L, TIMER_META_TYPE );
                    luau_setfunction( L, "__tostring", LuaTimerToString );
                    luau_setfunction( L, "__gc", LuaTimerGC );
                    lua_pushvalue( L, timerTable );
                    lua_setfield( L, -2, "__index" ); 
                    lua_pop(L,1);   //Pop metatable
            lua_setfield( L, -2, "timer" );
         lua_pop( L, 1);

        assert( sanity == lua_gettop( L ) ); 
    }
}

