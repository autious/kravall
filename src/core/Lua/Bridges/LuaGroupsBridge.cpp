#include "LuaGroupsBridge.hpp"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <GameUtility/NavigationMesh.hpp>
#include <logger/Logger.hpp>
#include <cassert>

#include <World.hpp>

extern "C"
{
	static int LuaSetGroupGoal(lua_State * L)
	{
		// Push the result of the CalculateFlowFieldForGroup to the lua stack. The function takes the four parameters entered
		// when calling the function.
		lua_pushboolean(L,
			Core::GetNavigationMesh()->CalculateFlowfieldForGroup(glm::vec3(static_cast<float>(luaL_checknumber(L, 2)),
                                                                            static_cast<float>(luaL_checknumber(L, 3)),
																			static_cast<float>(luaL_checknumber(L, 4))),
																    static_cast<int>(luaL_checkinteger(L, 1)))
			? 1 : 0);

		return 1;
	}

    static int LuaGetGroupNavigationMeshMedianPosition(lua_State * L)
    {
        glm::vec3 medianPosition = Core::world.m_systemHandler.GetSystem<Core::GroupDataSystem>()->GetMedianPosition(luaL_checknumber(L, 1));

        if(!Core::GetNavigationMesh()->CheckPointInsideNavigationMesh(medianPosition))
        {
                     
        }

        lua_pushnumber(L, medianPosition.x);
        lua_pushnumber(L, medianPosition.y);
        lua_pushnumber(L, medianPosition.z);
        
        return 3;
    }

    static int LuaGetGroupMemberCount(lua_State * L)
    {
        lua_pushnumber(L, Core::world.m_systemHandler.GetSystem<Core::GroupDataSystem>()->GetMemberCount(luaL_checknumber(L, 1)));

        return 1;
    }

    static int LuaGetNumberOfGroups(lua_State * L)
    {
        lua_pushnumber(L, Core::world.m_systemHandler.GetSystem<Core::GroupDataSystem>()->GetNumberOfGroups());

        return 1;
    }

	static int LuaNewIndex(lua_State * L)
	{
		return luaL_error(L, "Read only table");
	}
}

Core::LuaGroupsBridge::LuaGroupsBridge(lua_State * L)
{
	int stackSize = lua_gettop(L);
	lua_getglobal(L, "core");

	int coreTableIndex = lua_gettop(L);

	if (lua_isnil(L, -1) == false)
	{
		lua_pushstring(L, "groups");

		lua_newtable(L);
			lua_pushstring(L, "set_group_goal");
			lua_pushcfunction(L, LuaSetGroupGoal);
		lua_settable(L, -3);

            lua_pushstring(L, "get_group_median_position");
            lua_pushcfunction(L, LuaGetGroupNavigationMeshMedianPosition);
        lua_settable(L, -3);

            lua_pushstring(L, "get_group_member_count");
            lua_pushcfunction(L, LuaGetGroupMemberCount);
        lua_settable(L, -3);

            lua_pushstring(L, "get_number_of_groups");
            lua_pushcfunction(L, LuaGetNumberOfGroups);
        lua_settable(L, -3);

		    lua_newtable(L);
			    lua_pushstring(L, "__newindex");
			    lua_pushcfunction(L, LuaNewIndex);
			lua_settable(L, - 3);

		lua_setmetatable(L, -2);

		lua_settable(L, -3);
	}
	else
	{
		LOG_ERROR << "Missing core table" << std::endl;
	}

	lua_pop(L, 1); //pop core table

	assert(stackSize == lua_gettop(L));
}
