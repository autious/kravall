#include "FormationComponentBinding.hpp"

#include <Components/FormationComponent.hpp>
#include <ComponentFramework/SystemTypes.hpp>

#include <Lua/LuaUtility.hpp>

#include <World.hpp>

namespace Core
{
    ComponentGetters FormationComponentBinding::GetGetters()
    {
        Core::ComponentGetters getters;

        getters["relativePosition"] = [](Core::Entity entity, lua_State* L)
        {
            Core::FormationComponent* frmc = WGETC<Core::FormationComponent>(entity);
            if(frmc)
            {
                lua_pushnumber(L, frmc->relativePosition[0]);
                lua_pushnumber(L, frmc->relativePosition[1]);
                return 2;
            }
            else
            {
                return luaL_error(L, "could not retrieve FormationComponent of entity: %d to get relativePosition. Component is null", entity);
            }
        };

        return getters;
    }

    ComponentSetters FormationComponentBinding::GetSetters()
    {
        Core::ComponentSetters setters;

        setters["relativePosition"] = [](Core::Entity entity, lua_State* L, int valueindex)
        {
            Core::FormationComponent* frmc = WGETC<Core::FormationComponent>(entity);

            if(frmc)
            {
                if( lua_istable( L, valueindex ) )
                {           
                    for( int i = 0; i < 2; i++ )
                    {
                        lua_pushinteger( L, i+1 );
                        lua_gettable(L, valueindex);

                        if(lua_isnumber(L, -1))
                        {
                            frmc->relativePosition[i] = static_cast<float>(lua_tonumber(L, -1));
                        }
                        else
                        {
                            luaL_error(L, "unable to set relativePosition of entity: %d ,argument %d is not a number", entity, i+1);
                        }

                        lua_pop(L, 1);
                    }
                }
                else
                {
                    luaL_error(L, "unable to set squadTargetForward of entity: %d ,assignment is not a table", entity);
                }
            }
            else
            {
                luaL_error(L, "could not retrieve FormationComponent of entity: %d to set relativePosition. Component is null", entity);
            }
        };

        return setters;
    }

    Core::ComponentType FormationComponentBinding::GetComponentType()
    {
        return EntityHandler::GetComponentType<Core::FormationComponent>();
    }

    const char* FormationComponentBinding::GetComponentLuaName()
    {
        return "FormationComponent";
    }
}
