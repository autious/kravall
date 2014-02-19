#include "TargetingComponentBinding.hpp"

#include <World.hpp>

Core::ComponentGetters Core::TargetingComponentBinding::GetGetters()
{
	ComponentGetters getters;

	getters["numberOfAttackers"] = [](Core::Entity entity, lua_State *L)
	{
		TargetingComponent *mvc = WGETC<TargetingComponent>(entity);

		lua_pushnumber(L, mvc->numberOfAttackers);

		return 1;
	};

	getters["attackers"] = [](Core::Entity entity, lua_State * L)
	{
		TargetingComponent *mvc = WGETC<TargetingComponent>(entity);

		lua_newtable(L);

		for (int i = 0; i < MAX_ATTACKERS; i++)
		{
			lua_pushinteger(L, i + 1);
			lua_pushnumber(L, mvc->attackers[i]);
			lua_settable(L, -3);
		}

		return 1;
	};

	getters["target"] = [](Core::Entity entity, lua_State *L)
	{
		TargetingComponent *mvc = WGETC<TargetingComponent>(entity);

		lua_pushnumber(L, mvc->target);

		return 1;
	};

	getters["isAttacking"] = [](Core::Entity entity, lua_State *L)
	{
		TargetingComponent *mvc = WGETC<TargetingComponent>(entity);

		lua_pushnumber(L, mvc->isAttacking);

		return 1;
	};

	getters["hasAttacked"] = [](Core::Entity entity, lua_State *L)
	{
		TargetingComponent *mvc = WGETC<TargetingComponent>(entity);

		lua_pushnumber(L, mvc->hasAttacked);

		return 1;
	};

	getters["weapon"] = [](Core::Entity entity, lua_State *L)
	{
		TargetingComponent *mvc = WGETC<TargetingComponent>(entity);

		lua_pushnumber(L, mvc->weapon);

		return 1;
	};

	return getters;

}

Core::ComponentSetters Core::TargetingComponentBinding::GetSetters()
{
	ComponentSetters setters;

	setters["numberOfAttackers"] = [](Core::Entity entity, lua_State * L, int valueindex)
	{
		TargetingComponent *mvc = WGETC<TargetingComponent>(entity);

		if (lua_isnumber(L, valueindex))
		{
			mvc->numberOfAttackers = static_cast<float>(lua_tonumber(L, valueindex));
		}
		else
		{
			luaL_error(L, "Unable to set numberOfAttackers for ent %d, value is not number", entity);
		}
	};

	setters["attackers"] = [](Core::Entity entity, lua_State * L, int valueindex)
	{
		TargetingComponent *mvc = WGETC<TargetingComponent>(entity);

		if (lua_istable(L, valueindex))
		{
			for (int i = 0; i < MAX_ATTACKERS; i++)
			{
				lua_pushinteger(L, i + 1);
				lua_gettable(L, valueindex);

				if (lua_isnumber(L, -1))
				{
					mvc->attackers[i] = static_cast<float>(lua_tonumber(L, -1));
				}
				else
				{
					luaL_error(L, "Unable to set attacker for ent %d, value is not number", entity);
				}

				lua_pop(L, 1);
			}
		}
	};

	setters["target"] = [](Core::Entity entity, lua_State * L, int valueindex)
	{
		TargetingComponent *mvc = WGETC<TargetingComponent>(entity);

		if (lua_isnumber(L, valueindex))
		{
			mvc->target = static_cast<float>(lua_tonumber(L, valueindex));
		}
		else
		{
			luaL_error(L, "Unable to set target for ent %d, value is not number", entity);
		}
	};

	setters["isAttacking"] = [](Core::Entity entity, lua_State * L, int valueindex)
	{
		TargetingComponent *mvc = WGETC<TargetingComponent>(entity);

		if (lua_isnumber(L, valueindex))
		{
			mvc->isAttacking = static_cast<bool>(lua_toboolean(L, valueindex));
		}
		else
		{
			luaL_error(L, "Unable to set isAttacking for ent %d, value is not number", entity);
		}
	};

	setters["hasAttacked"] = [](Core::Entity entity, lua_State * L, int valueindex)
	{
		TargetingComponent *mvc = WGETC<TargetingComponent>(entity);

		if (lua_isnumber(L, valueindex))
		{
			mvc->hasAttacked = static_cast<bool>(lua_toboolean(L, valueindex));
		}
		else
		{
			luaL_error(L, "Unable to set hasAttacked for ent %d, value is not number", entity);
		}
	};

	setters["weapon"] = [](Core::Entity entity, lua_State * L, int valueindex)
	{
		TargetingComponent *mvc = WGETC<TargetingComponent>(entity);

		if (lua_isnumber(L, valueindex))
		{
			mvc->weapon = static_cast<int>(lua_tonumber(L, valueindex));
		}
		else
		{
			luaL_error(L, "Unable to set weapon for ent %d, value is not number", entity);
		}
	};

	return setters;
}


Core::ComponentType Core::TargetingComponentBinding::GetComponentType()
{
	return EntityHandler::GetComponentType<TargetingComponent>();
}

const char* Core::TargetingComponentBinding::GetComponentLuaName()
{
	return "TargetingComponent";
}
