#include "LuaGroupsBridge.hpp"

#include <Lua/LuaUtility.hpp>
#include <Lua/LuaMetatableTypes.hpp>

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

        Core::NavigationMesh* navMesh = Core::GetNavigationMesh();
        if(navMesh)
        {
            if(!navMesh->CheckPointInsideNavigationMesh(medianPosition))
            {
                     
            }
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

    static int LuaCreateGroup(lua_State * L)
    {
        Core::NavigationMesh* navMesh = Core::GetNavigationMesh();
        if(navMesh)
        {
            lua_pushnumber(L, navMesh->CreateGroup());
        }
        else
        {
            luaL_error(L, "Navigation mesh does not exist");
            lua_pushnumber(L, -1);
        }
        return 1;
    }

    static int LuaGetMembersInGroup(lua_State * L)
    {
        Core::GroupDataSystem* groupSystem = Core::world.m_systemHandler.GetSystem<Core::GroupDataSystem>();

        std::vector<Core::Entity> entities = groupSystem->GetMembersInGroup(luaL_checkinteger(L, 1));

        lua_newtable(L);
        int pos = 1;
        for(std::vector<Core::Entity>::iterator it = entities.begin(); it != entities.end(); ++it)
        {
            LuaEntity *le = Core::LuaUNewLightEntity(L);
            le->entity = *it;
            lua_rawseti(L, -2, pos++);
        }

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

    lua_getfield(L, -1, "system");
	

    lua_pushstring(L, "groups");

    lua_newtable(L);
        lua_pushstring(L, "setGroupGoal");
        lua_pushcfunction(L, LuaSetGroupGoal);
    lua_settable(L, -3);

        lua_pushstring(L, "getGroupMedianPosition");
        lua_pushcfunction(L, LuaGetGroupNavigationMeshMedianPosition);
    lua_settable(L, -3);

        lua_pushstring(L, "getGroupMemberCount");
        lua_pushcfunction(L, LuaGetGroupMemberCount);
    lua_settable(L, -3);

        lua_pushstring(L, "getNumberOfGroups");
        lua_pushcfunction(L, LuaGetNumberOfGroups);
    lua_settable(L, -3);

        lua_pushstring(L, "createGroup");
        lua_pushcfunction(L, LuaCreateGroup);
    lua_settable(L, -3);

        lua_pushstring(L, "getMembersInGroup");
        lua_pushcfunction(L, LuaGetMembersInGroup);
    lua_settable(L, -3);

        lua_newtable(L);
            lua_pushstring(L, "__newindex");
            lua_pushcfunction(L, LuaNewIndex);
        lua_settable(L, - 3);

    lua_setmetatable(L, -2);

    lua_settable(L, -3);

	lua_pop(L, 2); 

	assert(stackSize == lua_gettop(L));
}
