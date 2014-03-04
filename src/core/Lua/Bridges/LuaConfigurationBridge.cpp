#include "LuaConfigurationBridge.hpp"

#include <lua.h>

#include <cfgpath.h>

#include <logger/Logger.hpp>
#include <Lua/LuaUtility.hpp>

extern "C"
{
    static int LuaGetConfigurationFile( lua_State * L )
    {
        char cfgdir[MAX_PATH];     

        get_user_config_file( cfgdir, sizeof(cfgdir), "ekswaised-kravall-game" );

        if( cfgdir[0] == 0 ) 
        {
            LOG_ERROR << "Unable to find home directory" << std::endl;
            return 0;
        }

        lua_pushstring( L, cfgdir );

        return 1;
    }
}

namespace Core
{
    LuaConfigurationBridge::LuaConfigurationBridge( lua_State * L )
    {
       int sanity = lua_gettop( L );

        lua_getglobal( L, "core" );         
            lua_newtable( L );
                luau_setfunction( L, "getConfigurationPath", LuaGetConfigurationFile );
            lua_setfield( L, -2, "configuration" );
        lua_pop(L,1);

        assert( sanity == lua_gettop( L ) );
    }
}
