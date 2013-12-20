#ifndef SRC_CORE_LUA_BRIDGES_CONTENTLOADINGBIND_MATERIALLOADERBRIDGE_H
#define SRC_CORE_LUA_BRIDGES_CONTENTLOADINGBIND_MATERIALLOADERBRIDGE_H

#include <ContentManagement/ContentManager.hpp>
#include <lua.h>

namespace Core
{
    class MaterialLoaderBridge
    {
        public:
            static int Load( lua_State * L );
            static int Free( const AssetHash assetHash, lua_State * L );
            static const char *GetLoaderLuaName();
    };
}

#endif
