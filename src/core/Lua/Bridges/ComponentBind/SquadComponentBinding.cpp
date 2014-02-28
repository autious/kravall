#include "SquadComponentBinding.hpp"

#include <Components/SquadComponent.hpp>
#include <ComponentFramework/SystemTypes.hpp>

#include <Lua/Bridges/LuaAttributeBridge.hpp>
#include <Lua/LuaMetatableTypes.hpp>
#include <Components/AttributeComponent.hpp>

#include <Lua/LuaUtility.hpp>

#include <World.hpp>

namespace Core
{
    ComponentGetters SquadComponentBinding::GetGetters()
    {
        ComponentGetters getters;

        getters["squadStance"] = [](Core::Entity entity, lua_State* L)
        {
            SquadComponent* sqdc = WGETC<SquadComponent>(entity);

            Core::PoliceStance* stance = LuaUNewPoliceStance(L);
            *stance = sqdc->squadStance;

            return 1;
        };

        getters["squadFormation"] = [](Core::Entity entity, lua_State* L)
        {
            SquadComponent* sqdc = WGETC<SquadComponent>(entity);

            Core::SquadFormation* formation = Core::LuaUNewSquadFormation(L);
            *formation = sqdc->squadFormation;

            return 1;
        };

        getters["squadID"] = [](Core::Entity entity, lua_State* L)
        {
            SquadComponent* sqdc = WGETC<SquadComponent>(entity);
            lua_pushnumber(L, sqdc->squadID);

            return 1;
        };

        getters["squadGoal"] = [](Core::Entity entity, lua_State* L)
        {
            SquadComponent* sqdc = WGETC<SquadComponent>(entity);

            lua_newtable(L);
                lua_pushnumber(L, sqdc->squadGoal[0]);
                lua_rawseti(L, -2, 1);
                lua_pushnumber(L, sqdc->squadGoal[1]);
                lua_rawseti(L, -2, 2);            
                lua_pushnumber(L, sqdc->squadGoal[2]);
                lua_rawseti(L, -2, 3);            

            return 1;
        };

        getters["squadForward"] = [](Core::Entity entity, lua_State* L)
        {
            SquadComponent* sqdc = WGETC<SquadComponent>(entity);

            lua_newtable(L);
                lua_pushnumber(L, sqdc->squadForward[0]);
                lua_rawseti(L, -2, 1);
                lua_pushnumber(L, sqdc->squadForward[1]);
                lua_rawseti(L, -2, 2);            
            return 1;
        };

        getters["squadTargetForward"] = [](Core::Entity entity, lua_State* L)
        {
            SquadComponent* sqdc = WGETC<SquadComponent>(entity);
            
            lua_newtable(L);
                lua_pushnumber(L, sqdc->squadTargetForward[0]);
                lua_rawseti(L, -2, 1);
                lua_pushnumber(L, sqdc->squadTargetForward[1]);
                lua_rawseti(L, -2, 2);            
            return 1;
        };

        getters["squadLeader"] = [](Core::Entity entity, lua_State* L)
        {
            SquadComponent* sqdc = WGETC<SquadComponent>(entity);
            LuaEntity* ent = LuaUNewLightEntity(L);
            ent->entity = sqdc->squadLeader;

            return 1;
        };

        getters["squadMoveInFormation"] = [](Core::Entity entity, lua_State* L)
        {
            SquadComponent* sqdc = WGETC<SquadComponent>(entity);

            lua_pushboolean(L, sqdc->squadMoveInFormation);

            return 1;
        };

        getters["waitForStraggler"] = [](Core::Entity entity, lua_State* L)
        {
            SquadComponent* sqdc = WGETC<SquadComponent>(entity);

            lua_pushboolean(L, sqdc->waitForStraggler);

            return 1;
        };

        getters["squadHealth"] = [](Core::Entity entity, lua_State* L)
        {
            SquadComponent* sqdc = WGETC<SquadComponent>(entity);

            lua_pushnumber(L, sqdc->squadHealth);

            return 1;

        };

        getters["squadStamina"] = [](Core::Entity entity, lua_State* L)
        {

            SquadComponent* sqdc = WGETC<SquadComponent>(entity);

            lua_pushnumber(L, sqdc->squadStamina);

            return 1;
        };

        getters["squadMorale"] = [](Core::Entity entity, lua_State* L)
        {
            SquadComponent* sqdc = WGETC<SquadComponent>(entity);

            lua_pushnumber(L, sqdc->squadMorale);

            return 1;
        };

        return getters;
    }
    
    ComponentSetters SquadComponentBinding::GetSetters()
    {
        ComponentSetters setters;

        setters["squadStance"] = [](Core::Entity entity, lua_State* L, int valueindex)
        {
            SquadComponent* sqdc = WGETC<SquadComponent>(entity);
            sqdc->squadStance = *luau_checkpolicestance(L, valueindex);
        };

        setters["squadFormation"] = [](Core::Entity entity, lua_State* L, int valueindex)
        {
            SquadComponent* sqdc = WGETC<SquadComponent>(entity);
            sqdc->squadFormation = *luau_checksquadformation(L, valueindex);
        };

        setters["squadID"] = [](Core::Entity entity, lua_State* L, int valueindex)
        {
            if(lua_isnumber(L, valueindex))
            {
                SquadComponent* sqdc = WGETC<SquadComponent>(entity);
                sqdc->squadID = static_cast<int>(lua_tonumber(L, valueindex));
				LOG_WARNING << sqdc->squadID << std::endl;
            }
            else
            {
                luaL_error(L, "unable to set squadID of entity %d, value is not a number", entity);
            }
        };

		setters["squadGoal"] = [](Core::Entity entity, lua_State* L, int valueindex)
        {
            SquadComponent* sqdc = WGETC<SquadComponent>(entity);

            if( lua_istable( L, valueindex ) )
            {           
                for( int i = 0; i < 3; i++ )
                {
                    lua_pushinteger( L, i+1 );
                    lua_gettable(L, valueindex);

                    if(lua_isnumber(L, -1))
                    {
                        sqdc->squadGoal[i] = static_cast<float>(lua_tonumber(L, -1));
                    }
                    else
                    {
                        luaL_error(L, "unable to set squadGoal of entity: %d ,argument %d is not a number", entity, i+1);
                    }

                    lua_pop(L, 1);
                }
            }
            else
            {
                luaL_error(L, "unable to set squadGoal of entity: %d ,assignment is not a table", entity);
            }
        };

        setters["squadTargetForward"] = [](Core::Entity entity, lua_State* L, int valueindex)
        {
            SquadComponent* sqdc = WGETC<SquadComponent>(entity);

            if( lua_istable( L, valueindex ) )
            {           
                for( int i = 0; i < 2; i++ )
                {
                    lua_pushinteger( L, i+1 );
                    lua_gettable(L, valueindex);

                    if(lua_isnumber(L, -1))
                    {
                        sqdc->squadTargetForward[i] = static_cast<float>(lua_tonumber(L, -1));
                    }
                    else
                    {
                        luaL_error(L, "unable to set squadTargetForward of entity: %d ,argument %d is not a number", entity, i+1);
                    }

                    lua_pop(L, 1);
                }
            }
            else
            {
                luaL_error(L, "unable to set squadTargetForward of entity: %d ,assignment is not a table", entity);
            }
        };

        setters["squadMoveInFormation"] = [](Core::Entity entity, lua_State* L, int valueindex)
        {
            if(lua_isboolean(L, valueindex))
            {
                SquadComponent* sqdc = WGETC<SquadComponent>(entity);
                sqdc->squadMoveInFormation = static_cast<bool>(lua_toboolean(L, valueindex));
            }
            else
            {
                luaL_error(L, "unable to set squadMoveInFormation of entity %d, value is not a boolean", entity);
            }
        };

        return setters;
    }

    ComponentType SquadComponentBinding::GetComponentType()
    {
        return EntityHandler::GetComponentType<SquadComponent>();
    }

    const char* SquadComponentBinding::GetComponentLuaName()
    {
        return "SquadComponent";
    }
}



