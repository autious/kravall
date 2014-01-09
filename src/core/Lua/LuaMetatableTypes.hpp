#ifndef SRC_CORE_LUA_LUAMETATABLETYPES_H
#define SRC_CORE_LUA_LUAMETATABLETYPES_H

#define COMPONENT_META_TYPE "metatype_core_component"
#define ENTITY_META_TYPE "metatype_core_entity"
#define BITMASK_META_TYPE "metatype_bitmask"

#include <cstdint>

#include <lua.h>

#include <ComponentFramework/SystemHandlerTemplate.hpp>

namespace Core
{
    Entity* LuaUNewEntity( lua_State * L );

    uint64_t* LuaUNewBitmask( lua_State * L );
}

#endif
