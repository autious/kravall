#include "LuaSquadBridge.hpp"

#include <Systems/SquadSystem.hpp>

#include <Lua/LuaUtility.hpp>

#include <World.hpp>

extern "C"
{
    static int LuaSetSquadGoal(lua_State* L)
    {
        Core::SquadSystem* squadSystem = Core::world.m_systemHandler.GetSystem<Core::SquadSystem>();
        
        if(lua_gettop(L) == 4)
        {
            squadSystem->SetSquadGoal(static_cast<int>(luaL_checknumber(L, 1)), 
                    glm::vec3(static_cast<float>(luaL_checknumber(L, 2)), 
                                static_cast<float>(luaL_checknumber(L, 3)),
                                static_cast<float>(luaL_checknumber(L, 4))));
            return 0;
        }
        else
        {
            return luaL_error(L, "setSquadGoal(squadID, x, y, z)  expects 4 paramters");
        }
    }
}

namespace Core
{
    LuaSquadBridge::LuaSquadBridge(lua_State* L)
    {
        lua_getglobal(L, "core");
            lua_getfield(L, -1, "system");
                lua_newtable(L);
                    luau_setfunction(L, "setSquadGoal", LuaSetSquadGoal);
                lua_setfield(L, -2, "squad");
            lua_pop(L, 2);
    }

}
