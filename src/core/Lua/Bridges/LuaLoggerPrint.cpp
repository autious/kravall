#include "LuaLoggerPrint.hpp"

#include <sstream>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <logger/Logger.hpp>

#include <Lua/LuaMetatableTypes.hpp>
#include <Lua/LuaUtility.hpp>

extern "C" 
{
    static int LuaPrint( lua_State * L )
    {
        LuaLog * log = luau_checklog( L, 1 );

        int nargs = lua_gettop(L);

        std::stringstream ss; 

        for ( int i = 2; i <= nargs; i++) {
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

        LogSystem::LogData( log->logType, log->name ) <<  ss.str() << std::endl;

        return 0; 
    }
}

Core::LuaLoggerPrint::LuaLoggerPrint( lua_State * L )
{
    int sanity = lua_gettop( L);
    LuaLog *log = nullptr;

    luaL_newmetatable( L, LOG_META_TYPE );
        lua_newtable( L );
            luau_setfunction( L, "print", LuaPrint );
        lua_setfield( L, -2, "__index" );
        luau_setfunction( L, "__call", LuaPrint );
    lua_pop( L, 1 );

    lua_getglobal( L, "core" );
        lua_newtable( L );
            luau_setfunction( L, "print", LuaPrint );

            log = LuaUNewLog( L );
            log->logType = LogSystem::LogType::logType_debug;
            log->name = "ldebug";
            luaL_newmetatable( L, LOG_META_TYPE );        
            lua_setmetatable( L, -2 );
            lua_setfield( L, -2, "debug" );

            log = LuaUNewLog( L );
            log->logType = LogSystem::LogType::logType_fatal;
            log->name = "lfatal";
            luaL_newmetatable( L, LOG_META_TYPE );        
            lua_setmetatable( L, -2 );
            lua_setfield( L, -2, "fatal" );

            log = LuaUNewLog( L );
            log->logType = LogSystem::LogType::logType_error;
            log->name = "lerror";
            luaL_newmetatable( L, LOG_META_TYPE );        
            lua_setmetatable( L, -2 );
            lua_setfield( L, -2, "error" );

            log = LuaUNewLog( L );
            log->logType = LogSystem::LogType::logType_warning;
            log->name = "lwarning";
            luaL_newmetatable( L, LOG_META_TYPE );        
            lua_setmetatable( L, -2 );
            lua_setfield( L, -2, "warning" );

            log = LuaUNewLog( L );
            log->logType = LogSystem::LogType::logType_warning;
            log->name = "linfo";
            luaL_newmetatable( L, LOG_META_TYPE );        
            lua_setmetatable( L, -2 );
            lua_setfield( L, -2, "info" );

        lua_setfield( L, -2, "log" );
    lua_pop( L, 1 );
    
    assert( sanity == lua_gettop(L) );
}
