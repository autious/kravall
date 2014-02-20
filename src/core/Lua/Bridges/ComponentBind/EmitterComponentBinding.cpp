#include "EmitterComponentBinding.hpp"

#include <Components/EmitterComponent.hpp>
#include <World.hpp>

#include <Lua/LuaUtility.hpp>
#include <Lua/LuaMetatableTypes.hpp>

namespace Core
{
    ComponentGetters EmitterComponentBinding::GetGetters()
    {
        ComponentGetters getters;
        
        getters["rate"] = [](Core::Entity entity, lua_State* L)
        {
            EmitterComponent* emc = WGETC<EmitterComponent>(entity);
            if(emc)
            {
                lua_pushnumber(L, emc->rate);
                return 1;
            }
            return luaL_error(L, "could not retrieve EmitterComponent of entity %d to get 'rate'. Component is null", entity);
        };

        getters["offset"] = [](Core::Entity entity, lua_State* L)
        {
            EmitterComponent* emc = WGETC<EmitterComponent>(entity);
            if(emc)
            {
                lua_newtable(L);
                    lua_pushnumber(L, emc->offset[0]);
                    lua_rawseti(L, -2, 1);
                    lua_pushnumber(L, emc->offset[1]);
                    lua_rawseti(L, -2, 2);
                    lua_pushnumber(L, emc->offset[2]);
                    lua_rawseti(L, -2, 3);
                return 1;
            }
            return luaL_error(L, "could not retrieve EmitterComponent of entity %d to get 'offset'. Component is null", entity);
        };

        getters["life"] = [](Core::Entity entity, lua_State* L)
        {
            EmitterComponent* emc = WGETC<EmitterComponent>(entity);
            if(emc)
            {   
                lua_pushnumber(L, emc->life);
                return 1;
            }
            return luaL_error(L, "could not retrieve EmitterComponent of entity %d to get 'life'. Component is null", entity);
        };

        getters["lifeVariance"] = [](Core::Entity entity, lua_State* L)
        {
            EmitterComponent* emc = WGETC<EmitterComponent>(entity);
            if(emc)
            {
                lua_pushnumber(L, emc->lifeVariance);
                return 1;
            }
            return luaL_error(L, "could not retrieve EmitterComponent of entity %d to get 'lifeVariance'. Component is null", entity);
        };

        getters["lifeReduction"] = [](Core::Entity entity, lua_State* L)
        {
            EmitterComponent* emc = WGETC<EmitterComponent>(entity);
            if(emc)
            {
                lua_pushnumber(L, emc->lifeReduction);
                return 1;
            }
            return luaL_error(L, "could not retrieve EmitterComponent of entity %d to get 'lifeReduction'. Component is null", entity);
        };

        getters["lifeReductionVariance"] = [](Core::Entity entity, lua_State* L)
        {
            EmitterComponent* emc = WGETC<EmitterComponent>(entity);
            if(emc)
            {
                lua_pushnumber(L, emc->lifeReductionVariance);
                return 1;
            }
            return luaL_error(L, "could not retrieve EmitterComponent of entity %d to get 'lifeReductionVariance'. Component is null", entity);
        };

        getters["velocity"] = [](Core::Entity entity, lua_State* L)
        {
            EmitterComponent* emc = WGETC<EmitterComponent>(entity);
            if(emc)
            {
                lua_newtable(L);
                    lua_pushnumber(L, emc->velocity[0]);
                    lua_rawseti(L, -2, 1);
                    lua_pushnumber(L, emc->velocity[1]);
                    lua_rawseti(L, -2, 2);
                    lua_pushnumber(L, emc->velocity[2]);
                    lua_rawseti(L, -2, 3);
                return 1;
            }
            return luaL_error(L, "could not retrieve EmitterComponent of entity %d to get 'velocity'. Component is null", entity);
        };

        getters["velocityVariance"] = [](Core::Entity entity, lua_State* L)
        {
            EmitterComponent* emc = WGETC<EmitterComponent>(entity);
            if(emc)
            {
                lua_newtable(L);
                    lua_pushnumber(L, emc->velocityVariance[0]);
                    lua_rawseti(L, -2, 1);
                    lua_pushnumber(L, emc->velocityVariance[1]);
                    lua_rawseti(L, -2, 2);
                    lua_pushnumber(L, emc->velocityVariance[2]);
                    lua_rawseti(L, -2, 3);
                return 1;
            }
            return luaL_error(L, "could not retrieve EmitterComponent of entity %d to get 'velocityVariance'. Component is null", entity);
        };

        getters["acceleration"] = [](Core::Entity entity, lua_State* L)
        {
            EmitterComponent* emc = WGETC<EmitterComponent>(entity);
            if(emc)
            {
                lua_newtable(L);
                    lua_pushnumber(L, emc->acceleration[0]);
                    lua_rawseti(L, -2, 1);
                    lua_pushnumber(L, emc->acceleration[1]);
                    lua_rawseti(L, -2, 2);
                    lua_pushnumber(L, emc->acceleration[2]);
                    lua_rawseti(L, -2, 3);
                return 1;
            }
            return luaL_error(L, "could not retrieve EmitterComponent of entity %d to get 'acceleration'. Component is null", entity);
        };

        getters["type"] = [](Core::Entity entity, lua_State* L)
        {
            EmitterComponent* emc = WGETC<EmitterComponent>(entity);
            if(emc)
            {
                Core::EmitterType* type = LuaUNewEmitterType(L);
                (*type) = emc->type;
                return 1;
            }
            return luaL_error(L, "could not retrieve EmitterComponent of entity %d to get 'type'. Component is null", entity);
        };

        getters["handle"] = [](Core::Entity entity, lua_State* L)
        {
            EmitterComponent* emc = WGETC<EmitterComponent>(entity);
            if(emc)
            {
                LuaParticleDefinition* luaParticleDef = LuaUNewParticleDefinition(L);
                luaParticleDef->handle = emc->handle;
                return 1;
            }
            return luaL_error(L, "could not retrieve EmitterComponent of entity %d to get ''. Component is null", entity);
        };

        getters["coneDirection"] = [](Core::Entity entity, lua_State* L)
        {
            EmitterComponent* emc = WGETC<EmitterComponent>(entity);
            if(emc)
            {
                lua_newtable(L);
                    lua_pushnumber(L, emc->cone.coneDirection[0]);
                    lua_rawseti(L, -2, 1);
                    lua_pushnumber(L, emc->cone.coneDirection[1]);
                    lua_rawseti(L, -2, 2);
                    lua_pushnumber(L, emc->cone.coneDirection[2]);
                    lua_rawseti(L, -2, 3);                        
                return 1;
            }
            return luaL_error(L, "could not retrieve EmitterComponent of entity %d to get 'coneDirection'. Component is null", entity);
        };

        getters["coneAngle"] = [](Core::Entity entity, lua_State* L)
        {
            EmitterComponent* emc = WGETC<EmitterComponent>(entity);
            if(emc)
            {
                lua_pushnumber(L, emc->cone.coneAngle);
                return 1;
            }
            return luaL_error(L, "could not retrieve EmitterComponent of entity %d to get 'coneAngle'. Component is null", entity);
        };

        getters["coneAngleVariance"] = [](Core::Entity entity, lua_State* L)
        {
            EmitterComponent* emc = WGETC<EmitterComponent>(entity);
            if(emc)
            {
                lua_pushnumber(L, emc->cone.coneAngleVariance);
                return 1;
            }
            return luaL_error(L, "could not retrieve EmitterComponent of entity %d to get 'coneAngleVariance'. Component is null", entity);
        };

        getters["surfaceDirectionOne"] = [](Core::Entity entity, lua_State* L)
        {
            EmitterComponent* emc = WGETC<EmitterComponent>(entity);
            if(emc)
            {
                lua_newtable(L);
                    lua_pushnumber(L, emc->surface.surfaceDirectionOne[0]);
                    lua_rawseti(L, -2, 1);
                    lua_pushnumber(L, emc->surface.surfaceDirectionOne[1]);
                    lua_rawseti(L, -2, 2);
                    lua_pushnumber(L, emc->surface.surfaceDirectionOne[2]);
                    lua_rawseti(L, -2, 3);                        
                return 1;
            }
            return luaL_error(L, "could not retrieve EmitterComponent of entity %d to get 'surfaceDirectionOne'. Component is null", entity);
        };

        getters["surfaceDirectionTwo"] = [](Core::Entity entity, lua_State* L)
        {
            EmitterComponent* emc = WGETC<EmitterComponent>(entity);
            if(emc)
            {
                lua_newtable(L);
                    lua_pushnumber(L, emc->surface.surfaceDirectionTwo[0]);
                    lua_rawseti(L, -2, 1);
                    lua_pushnumber(L, emc->surface.surfaceDirectionTwo[1]);
                    lua_rawseti(L, -2, 2);
                    lua_pushnumber(L, emc->surface.surfaceDirectionTwo[2]);
                    lua_rawseti(L, -2, 3);                        
                return 1;
            }
            return luaL_error(L, "could not retrieve EmitterComponent of entity %d to get 'surfaceDirectionTwo'. Component is null", entity);
        };

        return getters;
    }

    ComponentSetters EmitterComponentBinding::GetSetters()
    {
        Core::ComponentSetters setters;

        setters["rate"] = [](Core::Entity entity, lua_State* L, int valueindex)
        {
            Core::EmitterComponent* emc = WGETC<Core::EmitterComponent>(entity);

            if(emc)
            {
                if( lua_isnumber( L, valueindex ) )
                {           
                    emc->rate = static_cast<int>(lua_tonumber(L, valueindex));
                }
                else
                {
                    luaL_error(L, "unable to set 'rate' of entity: %d ,assignment is not a number", entity);
                }
            }
            else
            {
                luaL_error(L, "could not retrieve EmitterComponent of entity: %d to set 'rate'. Component is null", entity);
            }
        };

        setters["offset"] = [](Core::Entity entity, lua_State* L, int valueindex)
        {
            Core::EmitterComponent* emc = WGETC<Core::EmitterComponent>(entity);

            if(emc)
            {
                if( lua_istable( L, valueindex ) )
                {           
                    for( int i = 0; i < 3; i++ )
                    {
                        lua_pushinteger( L, i+1 );
                        lua_gettable(L, valueindex);

                        if(lua_isnumber(L, -1))
                        {
                            emc->offset[i] = static_cast<float>(lua_tonumber(L, -1));
                        }
                        else
                        {
                            luaL_error(L, "unable to set 'offset' of entity: %d ,argument %d is not a number", entity, i+1);
                        }

                        lua_pop(L, 1);
                    }
                }
                else
                {
                    luaL_error(L, "unable to set 'offset' of entity: %d ,assignment is not a table", entity);
                }
            }
            else
            {
                luaL_error(L, "could not retrieve EmitterComponent of entity: %d to set 'offset'. Component is null", entity);
            }
        };

        setters["life"] = [](Core::Entity entity, lua_State* L, int valueindex)
        {
            Core::EmitterComponent* emc = WGETC<Core::EmitterComponent>(entity);

            if(emc)
            {
                if( lua_isnumber( L, valueindex ) )
                {           
                    emc->life = static_cast<float>(lua_tonumber(L, valueindex));
                }
                else
                {
                    luaL_error(L, "unable to set 'life' of entity: %d ,assignment is not a number", entity);
                }
            }
            else
            {
                luaL_error(L, "could not retrieve EmitterComponent of entity: %d to set 'life'. Component is null", entity);
            }
        };
        setters["lifeVariance"] = [](Core::Entity entity, lua_State* L, int valueindex)
        {
            Core::EmitterComponent* emc = WGETC<Core::EmitterComponent>(entity);

            if(emc)
            {
                if( lua_isnumber( L, valueindex ) )
                {           
                    emc->lifeVariance = static_cast<float>(lua_tonumber(L, valueindex));
                }
                else
                {
                    luaL_error(L, "unable to set 'lifeVariance' of entity: %d ,assignment is not a number", entity);
                }
            }
            else
            {
                luaL_error(L, "could not retrieve EmitterComponent of entity: %d to set 'lifeVariance'. Component is null", entity);
            }
        };

        setters["lifeReduction"] = [](Core::Entity entity, lua_State* L, int valueindex)
        {
            Core::EmitterComponent* emc = WGETC<Core::EmitterComponent>(entity);

            if(emc)
            {
                if( lua_isnumber( L, valueindex ) )
                {           
                    emc->lifeReduction = static_cast<float>(lua_tonumber(L, valueindex));
                }
                else
                {
                    luaL_error(L, "unable to set 'lifeReduction' of entity: %d ,assignment is not a number", entity);
                }
            }
            else
            {
                luaL_error(L, "could not retrieve EmitterComponent of entity: %d to set 'lifeReduction'. Component is null", entity);
            }
        };

        setters["lifeReductionVariance"] = [](Core::Entity entity, lua_State* L, int valueindex)
        {
            Core::EmitterComponent* emc = WGETC<Core::EmitterComponent>(entity);

            if(emc)
            {
                if( lua_isnumber( L, valueindex ) )
                {           
                    emc->lifeReductionVariance = static_cast<float>(lua_tonumber(L, valueindex));
                }
                else
                {
                    luaL_error(L, "unable to set 'lifeReductionVariance' of entity: %d ,assignment is not a number", entity);
                }
            }
            else
            {
                luaL_error(L, "could not retrieve EmitterComponent of entity: %d to set 'lifeReductionVariance'. Component is null", entity);
            }
        };

        setters["velocity"] = [](Core::Entity entity, lua_State* L, int valueindex)
        {
            Core::EmitterComponent* emc = WGETC<Core::EmitterComponent>(entity);

            if(emc)
            {
                if( lua_istable( L, valueindex ) )
                {           
                    for( int i = 0; i < 3; i++ )
                    {
                        lua_pushinteger( L, i+1 );
                        lua_gettable(L, valueindex);

                        if(lua_isnumber(L, -1))
                        {
                            emc->velocity[i] = static_cast<float>(lua_tonumber(L, -1));
                        }
                        else
                        {
                            luaL_error(L, "unable to set 'velocity' of entity: %d ,argument %d is not a number", entity, i+1);
                        }

                        lua_pop(L, 1);
                    }
                }
                else
                {
                    luaL_error(L, "unable to set 'velocity' of entity: %d ,assignment is not a table", entity);
                }
            }
            else
            {
                luaL_error(L, "could not retrieve EmitterComponent of entity: %d to set 'velocity'. Component is null", entity);
            }
        };

        setters["velocityVariance"] = [](Core::Entity entity, lua_State* L, int valueindex)
        {
            Core::EmitterComponent* emc = WGETC<Core::EmitterComponent>(entity);

            if(emc)
            {
                if( lua_istable( L, valueindex ) )
                {           
                    for( int i = 0; i < 3; i++ )
                    {
                        lua_pushinteger( L, i+1 );
                        lua_gettable(L, valueindex);

                        if(lua_isnumber(L, -1))
                        {
                            emc->velocityVariance[i] = static_cast<float>(lua_tonumber(L, -1));
                        }
                        else
                        {
                            luaL_error(L, "unable to set 'velocityVariance' of entity: %d ,argument %d is not a number", entity, i+1);
                        }

                        lua_pop(L, 1);
                    }
                }
                else
                {
                    luaL_error(L, "unable to set 'velocityVariance' of entity: %d ,assignment is not a table", entity);
                }
            }
            else
            {
                luaL_error(L, "could not retrieve EmitterComponent of entity: %d to set 'velocityVariance'. Component is null", entity);
            }
        };

        setters["acceleration"] = [](Core::Entity entity, lua_State* L, int valueindex)
        {
            Core::EmitterComponent* emc = WGETC<Core::EmitterComponent>(entity);

            if(emc)
            {
                if( lua_istable( L, valueindex ) )
                {           
                    for( int i = 0; i < 3; i++ )
                    {
                        lua_pushinteger( L, i+1 );
                        lua_gettable(L, valueindex);

                        if(lua_isnumber(L, -1))
                        {
                            emc->acceleration[i] = static_cast<float>(lua_tonumber(L, -1));
                        }
                        else
                        {
                            luaL_error(L, "unable to set 'acceleration' of entity: %d ,argument %d is not a number", entity, i+1);
                        }

                        lua_pop(L, 1);
                    }
                }
                else
                {
                    luaL_error(L, "unable to set 'acceleration' of entity: %d ,assignment is not a table", entity);
                }
            }
            else
            {
                luaL_error(L, "could not retrieve EmitterComponent of entity: %d to set 'acceleration'. Component is null", entity);
            }
        };

        setters["type"] = [](Core::Entity entity, lua_State* L, int valueindex)
        {
            Core::EmitterComponent* emc = WGETC<Core::EmitterComponent>(entity);

            if(emc)
            {
                Core::EmitterType* type = luau_checkemittertype(L, valueindex);
                emc->type = (*type);
            }
            else
            {
                luaL_error(L, "could not retrieve EmitterComponent of entity: %d to set 'type'. Component is null", entity);
            }
        };

        setters["handle"] = [](Core::Entity entity, lua_State* L, int valueindex)
        {
            Core::EmitterComponent* emc = WGETC<Core::EmitterComponent>(entity);

            if(emc)
            {
                LuaParticleDefinition* luaParticleDef = luau_checkparticledefinition(L, valueindex);
                emc->handle = luaParticleDef->handle;
            }
            else
            {
                luaL_error(L, "could not retrieve EmitterComponent of entity: %d to set 'handle'. Component is null", entity);
            }
        };

        setters["coneDirection"] = [](Core::Entity entity, lua_State* L, int valueindex)
        {
            Core::EmitterComponent* emc = WGETC<Core::EmitterComponent>(entity);

            if(emc)
            {
                if( lua_istable( L, valueindex ) )
                {           
                    for( int i = 0; i < 3; i++ )
                    {
                        lua_pushinteger( L, i+1 );
                        lua_gettable(L, valueindex);

                        if(lua_isnumber(L, -1))
                        {
                            emc->cone.coneDirection[i] = static_cast<float>(lua_tonumber(L, -1));
                        }
                        else
                        {
                            luaL_error(L, "unable to set 'coneDirection' of entity: %d ,argument %d is not a number", entity, i+1);
                        }

                        lua_pop(L, 1);
                    }
                }
                else
                {
                    luaL_error(L, "unable to set 'coneDirection' of entity: %d ,assignment is not a table", entity);
                }
            }
            else
            {
                luaL_error(L, "could not retrieve EmitterComponent of entity: %d to set 'coneDirection'. Component is null", entity);
            }
        };

        setters["coneAngle"] = [](Core::Entity entity, lua_State* L, int valueindex)
        {
            Core::EmitterComponent* emc = WGETC<Core::EmitterComponent>(entity);

            if(emc)
            {
                if( lua_isnumber( L, valueindex ) )
                {           
                    emc->cone.coneAngle = static_cast<float>(lua_tonumber(L, -1));
                }
                else
                {
                    luaL_error(L, "unable to set 'coneAngle' of entity: %d ,assignment is not a number", entity);
                }
            }
            else
            {
                luaL_error(L, "could not retrieve EmitterComponent of entity: %d to set 'coneAngle'. Component is null", entity);
            }
        };

        setters["coneAngleVariance"] = [](Core::Entity entity, lua_State* L, int valueindex)
        {
            Core::EmitterComponent* emc = WGETC<Core::EmitterComponent>(entity);

            if(emc)
            {
                if( lua_isnumber( L, valueindex ) )
                {           
                    emc->cone.coneAngleVariance = static_cast<float>(lua_tonumber(L, -1));
                }
                else
                {
                    luaL_error(L, "unable to set 'coneAngleVariance' of entity: %d ,assignment is not a number", entity);
                }
            }
            else
            {
                luaL_error(L, "could not retrieve EmitterComponent of entity: %d to set 'coneAngleVariance'. Component is null", entity);
            }
        };

        setters["surfaceDirectionOne"] = [](Core::Entity entity, lua_State* L, int valueindex)
        {
            Core::EmitterComponent* emc = WGETC<Core::EmitterComponent>(entity);

            if(emc)
            {
                if( lua_istable( L, valueindex ) )
                {           
                    for( int i = 0; i < 3; i++ )
                    {
                        lua_pushinteger( L, i+1 );
                        lua_gettable(L, valueindex);

                        if(lua_isnumber(L, -1))
                        {
                            emc->surface.surfaceDirectionOne[i] = static_cast<float>(lua_tonumber(L, -1));
                        }
                        else
                        {
                            luaL_error(L, "unable to set 'surfaceDirectionOne' of entity: %d ,argument %d is not a number", entity, i+1);
                        }

                        lua_pop(L, 1);
                    }
                }
                else
                {
                    luaL_error(L, "unable to set 'surfaceDirectionOne' of entity: %d ,assignment is not a table", entity);
                }
            }
            else
            {
                luaL_error(L, "could not retrieve EmitterComponent of entity: %d to set 'surfaceDirectionOne'. Component is null", entity);
            }
        };

        setters["surfaceDirectionTwo"] = [](Core::Entity entity, lua_State* L, int valueindex)
        {
            Core::EmitterComponent* emc = WGETC<Core::EmitterComponent>(entity);

            if(emc)
            {
                if( lua_istable( L, valueindex ) )
                {           
                    for( int i = 0; i < 3; i++ )
                    {
                        lua_pushinteger( L, i+1 );
                        lua_gettable(L, valueindex);

                        if(lua_isnumber(L, -1))
                        {
                            emc->surface.surfaceDirectionTwo[i] = static_cast<float>(lua_tonumber(L, -1));
                        }
                        else
                        {
                            luaL_error(L, "unable to set 'surfaceDirectionTwo' of entity: %d ,argument %d is not a number", entity, i+1);
                        }

                        lua_pop(L, 1);
                    }
                }
                else
                {
                    luaL_error(L, "unable to set 'surfaceDirectionTwo' of entity: %d ,assignment is not a table", entity);
                }
            }
            else
            {
                luaL_error(L, "could not retrieve EmitterComponent of entity: %d to set 'surfaceDirectionTwo'. Component is null", entity);
            }
        };

        return setters;
    }

    ComponentType EmitterComponentBinding::GetComponentType()
    {
        return EntityHandler::GetComponentType<EmitterComponent>();
    }

    const char* EmitterComponentBinding::GetComponentLuaName()
    {
        return "EmitterComponent";
    }
}
