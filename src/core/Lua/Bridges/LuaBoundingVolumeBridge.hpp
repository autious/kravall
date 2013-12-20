#ifndef SRC_CORE_LUA_BRIDGES_BOUNDINGVOLUMEBRIDGE_HPP
#define SRC_CORE_LUA_BRIDGES_BOUNDINGVOLUMEBRIDGE_HPP

#include <lua.h>
#define BOUNDING_VOLUME_COMPONENT_COLLISIONMODEL_TYPE_META "bounding_volume_component_collisionmodel_object_type_meta"
#define BOUNDING_VOLUME_COMPONENT_TYPE_TYPE_META "bounding_volume_component_type_object_type_meta"


namespace Core
{
    class LuaBoundingVolumeComponentBridge
    {
    public:
         static void OpenLibs( lua_State * L );
    };
}

#endif
