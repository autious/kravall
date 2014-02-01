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
        }
        else
        {
            lua_pushnil(L);
        }

        return 1;
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
            }
            else
            {
                lua_pushnil(L);
            }
            return 1;
        }
            
        return luaL_error(L, "getHitEntity(mouseX, mouseY, Aspect), requires 3 arguments");
    }

    static int LuaGetGroundHit(lua_State * L)
    {
        Core::PickingSystem* pickingSystem = Core::world.m_systemHandler.GetSystem<Core::PickingSystem>();

        if(lua_gettop(L) == 2)
        {
            int mouseX = luaL_checkint(L, 1);
            int mouseY = luaL_checkint(L, 2);

            glm::vec3 pos = pickingSystem->GetGroundHit(mouseX, mouseY);        
            lua_pushnumber(L, pos.x);
            lua_pushnumber(L, pos.y);
            lua_pushnumber(L, pos.z);

            return 3;
        }
            
        return luaL_error(L, "getGroundHit(mouseX, mouseY), requires 2 arguments");
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
                    luau_setfunction(L, "getGroundHit", LuaGetGroundHit);

                lua_setfield(L, -2, "picking" );
            lua_pop( L, 2 );
    }
}
