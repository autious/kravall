#ifndef SRC_CORE_LUA_BRIDGES_LUAMOVEMENTMETADATABRIDGE_HPP
#define SRC_CORE_LUA_BRIDGES_LUAMOVEMENTMETADATABRIDGE_HPP

#include <lua.h>
#define UNIT_MOVEMENT_META_DATA_TYPE_META "unit_movement_meta_data_type_meta"

namespace Core
{
    class LuaMovementMetaDataBridge
    {
    public:
         LuaMovementMetaDataBridge( lua_State * L );
    };
}

#endif
