#ifndef SRC_CORE_LUA_BRIDGES_LUALINEARHEAPBRIDGE_HPP
#define SRC_CORE_LUA_BRIDGES_LUALINEARHEAPBRIDGE_HPP

#include <lua.h>
//#define ATTRIBUTE_POLICE_COMPONENT_TYPE_META "attribute_police_component_type_meta"
//#define ATTRIBUTE_RIOTER_COMPONENT_TYPE_META "attribute_rioter_component_type_meta"

namespace Core
{
    class LuaLinearHeapBridge
    {
    public:
        LuaLinearHeapBridge( lua_State * L );
    };
}

#endif
