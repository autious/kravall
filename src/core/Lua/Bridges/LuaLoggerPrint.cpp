#include "LuaLoggerPrint.hpp"

#include <sstream>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <logger/Logger.hpp>

extern "C" 
{
    static int LuaPrintDebug( lua_State * L )
    {
       int nargs = lua_gettop(L);

        std::stringstream ss; 

        for (int i=1; i <= nargs; i++) {
            if (lua_isstring(L, i)) 
            {
                ss << lua_tostring( L, i );
            }
            else if( lua_isnumber( L, i ) )
            {
                ss << lua_tonumber( L, i );
            }
            else if( lua_isboolean( L, i ) )
            {
                ss << lua_toboolean( L, i );
            }
            else if( lua_istable( L, i ) )
            {
                ss << "table";
            }
            else if( lua_isnil( L, i ) )
            {
                ss << "nil"; 
            }
            else
            {
                ss << "[TYPE NOT HANDLED]";
            }
        }

        LogSystem::LogData( LogSystem::LogType::logType_warning, "lua" ) <<  ss.str() << std::endl;

        return 0; 
    }

    static int LuaPrint( lua_State * L )
    {
        return LuaPrintDebug( L );
    }
}

void Core::LuaLoggerPrint::OpenLibs( lua_State * state )
{
    lua_pushcfunction( state, LuaPrintDebug );
    lua_setglobal( state, "log" );

    lua_pushcfunction( state, LuaPrint );
    lua_setglobal( state, "print" );
}