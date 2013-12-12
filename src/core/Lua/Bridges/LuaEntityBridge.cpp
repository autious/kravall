#include "LuaEntityBridge.hpp"

#include <Lua/Bridges/ComponentBind/WorldPositionComponentBinding.hpp>

namespace Core
{
    typedef LuaEntityBridgeTemplate<WorldPositionComponentBinding> EntityBridge;
}

static Core::EntityBridge entityBridge;

extern "C" 
{

    static int LuaEntitySet( lua_State * L )
    {
        return entityBridge.CallSet( L );    
    }

    /*
    static int LuaEntityGet( lua_State * L )
    {
        
    }
    */
}

void Core::LuaEntityBridge::OpenLibs( lua_State * L )
{
    
}
