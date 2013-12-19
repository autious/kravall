#ifndef SRC_CORE_LUA_BRIDGES_CONTENTLOADING_CONTENTHANDLE_H
#define SRC_CORE_LUA_BRIDGES_CONTENTLOADING_CONTENTHANDLE_H

#include <ContentManagement/ContentManager.hpp>

#define CONTENT_LOADER_TYPE_META "content_loader_type_meta"
#define CONTENT_HANDLE_META "content_handle_meta"

namespace Core
{
    typedef unsigned int ContentLoaderBridgeType;

    struct ContentHandle
    {
        ContentLoaderBridgeType loaderId;
        AssetHash assetHash;
    };
}


#endif
