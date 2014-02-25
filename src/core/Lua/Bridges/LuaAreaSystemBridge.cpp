#include "LuaAreaSystemBridge.hpp"

#include <Systems/AIsupport/AreaSystem.hpp>

#include <World.hpp>
#include <Lua/LuaMetatableTypes.hpp>
#include <Lua/LuaUtility.hpp>

extern "C"
{
    static int LuaGetAreaRioters( lua_State * L )
    {
        Core::AreaSystem *as = Core::world.m_systemHandler.GetSystem<Core::AreaSystem>();
        LuaEntity * ent = luau_checkentity( L, 1 );

        std::vector<Core::Entity> ents = as->GetAreaRioters( ent->entity );

        lua_newtable( L );
        int pos = 1;
        for(  std::vector<Core::Entity>::iterator it = ents.begin(); it != ents.end(); it++ )
        {
            LuaEntity *le = Core::LuaUNewLightEntity( L );
            le->entity = *it;
            lua_rawseti( L, -2, pos++ );
        }

        return 1;
    }

    static int LuaGetAreaRioterCount( lua_State * L )
    {
        Core::AreaSystem *as = Core::world.m_systemHandler.GetSystem<Core::AreaSystem>();
        LuaEntity * ent = luau_checkentity( L, 1 );
        
        lua_pushinteger( L, as->GetAreaRioterCount( ent->entity ) );

        return 1;
    }

    static int LuaIsPointInside( lua_State * L )
    {
        Core::AreaSystem *as = Core::world.m_systemHandler.GetSystem<Core::AreaSystem>();
        LuaEntity * ent = luau_checkentity( L, 1 );
        glm::vec3 * vec3 = luau_checkglmvec3( L, 2);  

        return as->IsAreaOnPoint( ent->entity, *vec3 );
    }
}

namespace Core
{
    LuaAreaSystemBridge::LuaAreaSystemBridge( lua_State * L )
    {
        lua_getglobal( L, "core" );
            lua_getfield(L, -1, "system" );
                lua_newtable( L );
                    luau_setfunction( L, "getAreaRioters", LuaGetAreaRioters );
                    luau_setfunction( L, "getAreaRioterCount", LuaGetAreaRioterCount );
                    luau_setfunction( L, "isPointInside", LuaIsPointInside );
                lua_setfield(L, -2, "area" );
            lua_pop( L, 2 );
    }
}
