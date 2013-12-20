#ifndef SRC_CORE_LUA_BRIDGES_LUAGFXBRIDGE_H
#define SRC_CORE_LUA_BRIDGES_LUAGFXBRIDGE_H

#include <lua.h>
#define GFX_OBJECT_TYPE_META "gfx_object_type_meta"
#define GFX_LIGHT_TYPE_META "gfx_light_type_meta"

namespace Core
{
    typedef unsigned int GFXObjectType;
    typedef unsigned int GFXLightType;
    class LuaGFXBridge
    {
    public:
         static void OpenLibs( lua_State * L );
    };
}

#endif
