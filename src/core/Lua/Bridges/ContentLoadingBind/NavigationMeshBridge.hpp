#ifndef SRC_CORE_LUA_BRIDGES_CONTENTLOADINGBIND_NAVIGATIONMESHBRIDGE_HPP
#define SRC_CORE_LUA_BRIDGES_CONTENTLOADINGBIND_NAVIGATIONMESHBRIDGE_HPP

#include <lua.h>

namespace Core
{
    class NavigationMeshBridge
    {
        public:
            static int Load( lua_State * L );
            static int Free( lua_State * L ); // does nothing...
            static const char *GetLoaderLuaName();
    };
}

#endif
