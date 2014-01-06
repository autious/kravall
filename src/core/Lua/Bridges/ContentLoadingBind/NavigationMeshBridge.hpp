#ifndef SRC_CORE_LUA_BRIDGES_CONTENTLOADINGBIND_NAVIGATIONMESHBRIDGE_HPP
#define SRC_CORE_LUA_BRIDGES_CONTENTLOADINGBIND_NAVIGATIONMESHBRIDGE_HPP

#include <lua.h>
#include <ContentManagement/ContentManager.hpp>

namespace Core
{
    class NavigationMeshBridge
    {
        public:
            static int Load( lua_State * L );
            static int Free( Core::AssetHash hash, lua_State * L ); 
            static const char *GetLoaderLuaName();
    };
}

#endif
