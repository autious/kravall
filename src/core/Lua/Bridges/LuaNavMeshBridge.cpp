#include "LuaNavMeshBridge.hpp"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <GameUtility/NavigationMesh.hpp>
#include <logger/Logger.hpp>
#include <cassert>

#include <World.hpp>

void CalculateGroupMedianPosition(int groupId, float &x, float &y, float &z)
{

}

extern "C"
{
	static int LuaSetGroupGoal(lua_State * L)
	{
		// Push the result of the CalculateFlowFieldForGroup to the lua stack. The function takes the four parameters entered
		// when calling the function.
		lua_pushboolean(L,
			Core::GetNavigationMesh()->CalculateFlowfieldForGroup(glm::vec2(static_cast<float>(luaL_checknumber(L, 2)),
																			static_cast<float>(luaL_checknumber(L, 3))),
																    static_cast<int>(luaL_checkinteger(L, 1)))
			? 1 : 0);

		return 1;
	}

    static int LuaGetGroupNavigationMeshPosition(lua_State * L)
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        
        //CalculateGroupMedianPosition(luaL_checknumber(L, 1), &x, &y, &z);

        if(!Core::GetNavigationMesh()->CheckPointInsideNavigationMesh(glm::vec2(x, z)))
        {

        }

        lua_pushnumber(L, x);
        lua_pushnumber(L, y);
        
        return 2;
    }

	static int LuaNewIndex(lua_State * L)
	{
		return luaL_error(L, "Read only table");
	}
}

Core::LuaNavMeshBridge::LuaNavMeshBridge(lua_State * L)
{
	int stackSize = lua_gettop(L);
	lua_getglobal(L, "core");

	int coreTableIndex = lua_gettop(L);

	if (lua_isnil(L, -1) == false)
	{
		lua_pushstring(L, "nav_mesh");

		lua_newtable(L);
			lua_pushstring(L, "set_group_goal");
			lua_pushcfunction(L, LuaSetGroupGoal);
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
