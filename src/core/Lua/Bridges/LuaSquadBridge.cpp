#include "LuaSquadBridge.hpp"

#include <Systems/SquadSystem.hpp>

#include <Lua/LuaUtility.hpp>
#include <Lua/LuaMetatableTypes.hpp>

#include <World.hpp>

extern "C"
{

    static int LuaEq(lua_State* L)
    {
        Core::SquadFormation* lhs = luau_checksquadformation(L, 1);
        Core::SquadFormation* rhs = luau_checksquadformation(L, 2);

        lua_pushboolean(L, *rhs == *lhs);
        return 1;
    }

    static int LuaSetSquadGoal(lua_State* L)
    {
        Core::SquadSystem* squadSystem = Core::world.m_systemHandler.GetSystem<Core::SquadSystem>();
        
        if(lua_gettop(L) == 4)
        {
            if(lua_istable(L, 1))
            {   
                int nSquads = 0;
                int* squads = nullptr;

                lua_pushnil(L);
                while(lua_next(L, 1))
                {
                    lua_pop(L, 1);
                    ++nSquads;
                }
                        
                squads = Core::world.m_frameHeap.NewPODArray<int>(nSquads);

               
                lua_pushnil(L);
                for(int i=0; lua_next(L, 1); ++i)
                {
                    squads[i] = luaL_checknumber(L, -1);
                    lua_pop(L, 1);
                }                

                squadSystem->SetSquadGoal(squads, nSquads, 
                        glm::vec3(static_cast<float>(luaL_checknumber(L, 2)), 
                                    static_cast<float>(luaL_checknumber(L, 3)),
                                    static_cast<float>(luaL_checknumber(L, 4))));
                return 0;
            }
            else
            {
                return luaL_error(L, "argument 1 of setSquadGoal is not a table");
            }
        }
        else
        {
            return luaL_error(L, "setSquadGoal([squadIDs], x, y, z)  expects 4 paramters");
        }
    }

    static int LuaSetSquadFormation(lua_State* L)
    {
        Core::SquadSystem* squadSystem = Core::world.m_systemHandler.GetSystem<Core::SquadSystem>();

        if(lua_gettop(L) == 8)        
        {
            if(lua_istable(L, 1))
            {   
                int nSquads = 0;
                int* squads = nullptr;

                lua_pushnil(L);
                while(lua_next(L, 1))
                {
                    lua_pop(L, 1);
                    ++nSquads;
                }
                        
                squads = Core::world.m_frameHeap.NewPODArray<int>(nSquads);

                lua_pushnil(L);
                for(int i=0; lua_next(L, 1); ++i)
                {
                    squads[i] = luaL_checknumber(L, -1);
                    lua_pop(L, 1);
                }

                squadSystem->SetSquadFormation(squads, nSquads, *luau_checksquadformation(L, 2), 
                        glm::vec3(static_cast<float>(luaL_checknumber(L, 3)), 
                                    static_cast<float>(luaL_checknumber(L, 4)),
                                    static_cast<float>(luaL_checknumber(L, 5))),
                        glm::vec3(static_cast<float>(luaL_checknumber(L, 6)), 
                                    static_cast<float>(luaL_checknumber(L, 7)),
                                    static_cast<float>(luaL_checknumber(L, 8))));
                return 0;
            }
            else
            {
                return luaL_error(L, "argument 1 of setSquadFormation is not a table");
            }
        }
        else
        {
            return luaL_error(L, "setSquadFormation([squadIDs], <SquadFormation>, startX, startY, startZ, endX, endY, endZ)  expects 8 paramters");
        }
    }

    static int LuaPreviewSquadFormation(lua_State* L)
    {
        Core::SquadSystem* squadSystem = Core::world.m_systemHandler.GetSystem<Core::SquadSystem>();

        if(lua_gettop(L) == 8)
        {
            if(lua_istable(L, 1))
            {   
                int nSquads = 0;
                int* squads = nullptr;

                lua_pushnil(L);
                while(lua_next(L, 1))
                {
                    lua_pop(L, 1);
                    ++nSquads;
                }
                        
                squads = Core::world.m_frameHeap.NewPODArray<int>(nSquads);

                lua_pushnil(L);
                for(int i=0; lua_next(L, 1); ++i)
                {
                    squads[i] = luaL_checknumber(L, -1);
                    lua_pop(L, 1);
                }

                squadSystem->PreviewSquadFormation(squads, nSquads, *luau_checksquadformation(L, 2), 
                        glm::vec3(static_cast<float>(luaL_checknumber(L, 3)), 
                                    static_cast<float>(luaL_checknumber(L, 4)),
                                    static_cast<float>(luaL_checknumber(L, 5))),
                        glm::vec3(static_cast<float>(luaL_checknumber(L, 6)), 
                                    static_cast<float>(luaL_checknumber(L, 7)),
                                    static_cast<float>(luaL_checknumber(L, 8))));
                return 0;
            }
            else
            {

                return luaL_error(L, "argument 1 of previewSquadFormation is not a table");
            }
        }
        else
        {
            return luaL_error(L, "previewSquadFormation([squadIDs], <SquadFormation>, startX, startY, startZ, endX, endY, endZ)  expects 8 paramters");
        }
    }

    static int LuaSquadStance(lua_State* L)
    {

        Core::SquadSystem* squadSystem = Core::world.m_systemHandler.GetSystem<Core::SquadSystem>();

        if(lua_gettop(L) == 2)
        {
            if(lua_istable(L, 1))
            {   
                int nSquads = 0;
                int* squads = nullptr;

                lua_pushnil(L);
                while(lua_next(L, 1))
                {
                    lua_pop(L, 1);
                    ++nSquads;
                }
                        
                squads = Core::world.m_frameHeap.NewPODArray<int>(nSquads);

                lua_pushnil(L);
                for(int i=0; lua_next(L, 1); ++i)
                {
                    squads[i] = luaL_checknumber(L, -1);
                    lua_pop(L, 1);
                }

                squadSystem->SetSquadStance(squads, nSquads, *luau_checkpolicestance(L, 2)); 
                return 0;
            }
            else
            {

                return luaL_error(L, "argument 1 of previewSquadFormation is not a table");
            }
        }
        else
        {
            return luaL_error(L, "setSquadStance([squadIDs], <SquadFormation>, startX, startY, startZ, endX, endY, endZ)  expects 8 paramters");
        }
    }
}

namespace Core
{
    LuaSquadBridge::LuaSquadBridge(lua_State* L)
    {

        int sanity = lua_gettop(L);

        luaL_newmetatable(L, SQUAD_FORMATION_META_TYPE);
            luau_setfunction(L, "__eq", LuaEq);
        lua_pop(L, 1);

        lua_getglobal(L, "core");
            lua_getfield(L, -1, "system");
                lua_newtable(L);
                    luau_setfunction(L, "setSquadGoal", LuaSetSquadGoal);
                    luau_setfunction(L, "setSquadFormation", LuaSetSquadFormation);
                    luau_setfunction(L, "previewSquadFormation", LuaPreviewSquadFormation);
                        lua_newtable(L);
                        Core::SquadFormation* formation = LuaUNewSquadFormation(L);
                        *formation = Core::SquadFormation::NO_FORMATION;
                        lua_setfield(L, -2, "NoFormation");

                        formation = LuaUNewSquadFormation(L);
                        *formation = Core::SquadFormation::LINE_FORMATION;
                        lua_setfield(L, -2, "LineFormation");

                        formation = LuaUNewSquadFormation(L);
                        *formation = Core::SquadFormation::CIRCLE_FORMATION;
                        lua_setfield(L, -2, "CircleFormation");

                    lua_setfield(L, -2, "formations");
                lua_setfield(L, -2, "squad");
        lua_pop(L, 2);

        assert(sanity == lua_gettop(L));
    }

}
