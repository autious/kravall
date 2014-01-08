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
            else if( lua_isuserdata( L, i ) )
            {
                if( lua_getmetatable( L, i ) )
                {
                    lua_getfield( L, -1, "__tostring" ); 

                    if( lua_isfunction( L, -1 ) )
                    {
                        lua_pushvalue( L, i );
                        lua_call( L, 1, 1 ); //Removes the function and argument from stack

                        if( lua_isstring( L, -1 ) )
                            ss << lua_tostring( L, -1 );

                        lua_pop( L,1 ); //Pop the return value.
                    }

                    lua_pop( L,1 ); // Pop the __tostring value
                    lua_pop( L,1 ); // Pop the meta table value
                }
            }
            else if( lua_isnil( L, i ) )
            {
                ss << "nil"; 
            }
            else
            {
                ss << "[" << lua_typename(L,i) << " NOT HANDLED]";
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

Core::LuaLoggerPrint::LuaLoggerPrint( lua_State * state )
{
    lua_pushcfunction( state, LuaPrintDebug );
    lua_setglobal( state, "log" );

    lua_pushcfunction( state, LuaPrint );
    lua_setglobal( state, "print" );
}
