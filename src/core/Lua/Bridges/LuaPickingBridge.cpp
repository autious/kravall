#include "LuaPickingBridge.hpp"

#include <World.hpp>
#include <Lua/LuaMetatableTypes.hpp>
#include <Lua/LuaUtility.hpp>

#include <ComponentFramework/SystemTypes.hpp>

#include <limits>

extern "C"
{
    static int LuaGetLastHitEntity(lua_State * L)
    {
        Core::PickingSystem* pickingSystem = Core::world.m_systemHandler.GetSystem<Core::PickingSystem>();
        
        Core::Entity ent = pickingSystem->GetLastHitEntity();
        if(ent != INVALID_ENTITY)
        {
            LuaEntity* luaEnt = Core::LuaUNewLightEntity(L);    
            luaEnt->entity = ent;  

            lua_pushboolean(L, true);        
        }
        else
        {
            lua_pushnil(L);

            lua_pushboolean(L, false);
        }

        return 2;
    }

    static int LuaGetHitEntity(lua_State * L)
    {
        Core::PickingSystem* pickingSystem = Core::world.m_systemHandler.GetSystem<Core::PickingSystem>();

        if(lua_gettop(L) == 3)
        {
            int mouseX = luaL_checkint(L, 1);
            int mouseY = luaL_checkint(L, 2);
            LuaAspect* luaAsp = luau_checkaspect(L , 3);

            Core::Entity ent = pickingSystem->GetHitEntity(mouseX, mouseY, luaAsp->aspect);
            if(ent != INVALID_ENTITY)
            {
                LuaEntity* luaEnt = Core::LuaUNewLightEntity(L);
                luaEnt->entity = ent;

                lua_pushboolean(L, true);
            }
            else
            {
                lua_pushnil(L);

                lua_pushboolean(L, false);
            }
            return 2;
        }
            
        return luaL_error(L, "GetHitEntity(mouseX, mouseY, Aspect), requires 3 arguments");
    }
}

namespace Core
{
    LuaPickingBridge::LuaPickingBridge(lua_State* L)    
    {

        lua_getglobal(L, "core");
            lua_getfield(L, -1, "system");
                lua_newtable(L);
                    luau_setfunction(L, "getLastHitEntity", LuaGetLastHitEntity);
                    luau_setfunction(L, "getHitEntity", LuaGetHitEntity);

                lua_setfield(L, -2, "picking" );
            lua_pop( L, 2 );
    }
}
