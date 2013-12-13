#ifndef SRC_CORE_LUA_BRIDGES_LUAENTITYBRIDGETYPES_H
#define SRC_CORE_LUA_BRIDGES_LUAENTITYBRIDGETYPES_H

#include <ComponentFramework/SystemTypes.hpp>


#include <functional>
#include <unordered_map>
#include <string>

#include <lua.h>

namespace Core
{
    /* Entity id, lua_State value is in and table return values index */
    typedef std::function<int(Entity,lua_State*)> ComponentGet;
    /* Entity id, lua_State value is in and value index */
    typedef std::function<void(Entity,lua_State*,int)> ComponentSet;
    
    typedef std::unordered_map<std::string,ComponentGet> ComponentGetters;
    typedef std::unordered_map<std::string,ComponentSet> ComponentSetters;
}

#endif
