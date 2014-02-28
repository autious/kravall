#ifndef SRC_CORE_LUA_BRIDGES_CONTENTLOADINGBIND_GNOMELOADERBRIDGE_H
#define SRC_CORE_LUA_BRIDGES_CONTENTLOADINGBIND_GNOMELOADERBRIDGE_H

#include <ContentManagement/ContentManager.hpp>
#include <lua.h>

namespace Core
{
    class GnomeLoaderBridge
    {
        public:
            static int Load( lua_State * L );
            static int Free( const AssetHash assetHash, lua_State * L );
            static const char *GetLoaderLuaName();
    };
}

#endif
