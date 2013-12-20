#ifndef SRC_CORE_LUA_BRIDGES_LUAGFXBRIDGE_H
#define SRC_CORE_LUA_BRIDGES_LUAGFXBRIDGE_H

#include <lua.h>
#define GFX_OBJECT_TYPE_META "gfx_object_type_meta"

namespace Core
{
    typedef unsigned int GFXObjectType;
    class LuaGFXBridge
    {
    public:
         static void OpenLibs( lua_State * L );
    };
}

#endif
