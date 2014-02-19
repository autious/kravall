#include "LuaSquadBridge.hpp"

#include <Systems/AI/SquadSystem.hpp>

#include <Lua/LuaUtility.hpp>
#include <Lua/LuaMetatableTypes.hpp>

#include <World.hpp>

extern "C"
{
    static int LuaSquadFormationEq(lua_State* L)
    {
        Core::SquadFormation* lhs = luau_checksquadformation(L, 1);
        Core::SquadFormation* rhs = luau_checksquadformation(L, 2);

        lua_pushboolean(L, *rhs == *lhs);
        return 1;
    }

    static int LuaSquadFormationToString(lua_State* L)
    {
        Core::SquadFormation* sf = luau_checksquadformation(L, 1);

        switch(*sf)
        {
            case Core::SquadFormation::NO_FORMATION:
                lua_pushstring(L, "NoFormation" );
                break;
            case Core::SquadFormation::LINE_FORMATION:
                lua_pushstring(L, "LineFormation" );
                break;
            case Core::SquadFormation::CIRCLE_FORMATION:
                lua_pushstring(L, "CircleFormation" );
                break;
            case Core::SquadFormation::HALF_CIRCLE_FORMATION:
                lua_pushstring(L, "HalfCircleFormation" );
                break;
            default:
                lua_pushstring(L, "UnknownFormation" );
                break;
        }

        return 1;
    }

    static int LuaSquadAbilityEq( lua_State *L )
    {
        Core::SquadAbility* lhs = luau_checksquadability(L,1);
        Core::SquadAbility* rhs = luau_checksquadability(L,2);

        lua_pushboolean( L, *lhs == *rhs );
        return 1;
    }

    static int LuaSquadAbilityToString( lua_State *L )
    {
        Core::SquadAbility* sa = luau_checksquadability(L,1);

        switch( *sa )
        {
            case Core::SquadAbility::SPRINT:
                lua_pushstring( L, "Sprint" );
                break;
            case Core::SquadAbility::FLEE:
                lua_pushstring( L, "Flee" );
                break;
            case Core::SquadAbility::ROUT:
                lua_pushstring( L, "Rout" );
                break;
            case Core::SquadAbility::ATTACK:
                lua_pushstring( L, "Attack" );
                break;
            case Core::SquadAbility::ARREST_INDIVIDUAL:
                lua_pushstring( L, "ArrestIndividual" );
                break;
            case Core::SquadAbility::ARREST_GROUP:
                lua_pushstring( L, "ArrestGroup" );
                break;
            case Core::SquadAbility::TEAR_GAS:
                lua_pushstring( L, "TearGas" );
                break;
            case Core::SquadAbility::TAZE:
                lua_pushstring( L, "Taze" );
                break;
            case Core::SquadAbility::BLITZ:
                lua_pushstring( L, "Blitz" );
                break;
            default:
                lua_pushstring( L, "UnknownSquadAbility" );
                break;
        }

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
                    squads[i] = static_cast<int>(luaL_checknumber(L, -1));
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
            return luaL_error(L, "setSquadGoal([squadIDs], x, y, z)  expects 4 parameters");
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
                    squads[i] = static_cast<int>(luaL_checknumber(L, -1));
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
            return luaL_error(L, "setSquadFormation([squadIDs], <SquadFormation>, startX, startY, startZ, endX, endY, endZ)  expects 8 parameters");
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
                    squads[i] = static_cast<int>(luaL_checknumber(L, -1));
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
            return luaL_error(L, "previewSquadFormation([squadIDs], <SquadFormation>, startX, startY, startZ, endX, endY, endZ)  expects 8 parameters");
        }
    }

    static int LuaSetSquadStance(lua_State* L)
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
                    squads[i] = static_cast<int>(luaL_checknumber(L, -1));
                    lua_pop(L, 1);
                }

                squadSystem->SetSquadStance(squads, nSquads, *luau_checkpolicestance(L, 2)); 
                return 0;
            }
            else
            {

                return luaL_error(L, "argument 1 of setSquadStance is not a table");
            }
        }
        else
        {
            return luaL_error(L, "setSquadStance([squadIDs], <SquadFormation>, startX, startY, startZ, endX, endY, endZ)  expects 8 parameters");
        }
    }

    static int LuaGetSquadEntity(lua_State* L)
    {
        Core::SquadSystem* squadSystem = Core::world.m_systemHandler.GetSystem<Core::SquadSystem>();
        
        Core::Entity ent = squadSystem->GetSquadEntity(static_cast<int>(luaL_checknumber(L, 1)));
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

    static int LuaEnableOutline(lua_State* L)    
    {
        Core::SquadSystem* squadSystem = Core::world.m_systemHandler.GetSystem<Core::SquadSystem>();

        if(lua_gettop(L) == 5)
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
                    squads[i] = static_cast<int>(luaL_checknumber(L, -1));
                    lua_pop(L, 1);
                }

                squadSystem->EnableOutline(squads, nSquads, 
                        glm::vec4(luaL_checknumber(L, 2), 
                                    luaL_checknumber(L, 3), 
                                    luaL_checknumber(L, 4), 
                                    luaL_checknumber(L, 5))); 
                return 0;
            }
            else
            {
                return luaL_error(L, "argument 1 of enableOutline is not a table");
            }
        }
        else
        {
            return luaL_error(L, "enableOutline([squadIDs], Red, Green, Blue, Alpha)  expects 5 parameters");
        }
    }

    static int LuaDisableOutline(lua_State* L)    
    {
        Core::SquadSystem* squadSystem = Core::world.m_systemHandler.GetSystem<Core::SquadSystem>();

        if(lua_gettop(L) == 1)
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
                    squads[i] = static_cast<int>(luaL_checknumber(L, -1));
                    lua_pop(L, 1);
                }

                squadSystem->DisableOutline(squads, nSquads); 
                return 0;
            }
            else
            {
                return luaL_error(L, "argument 1 of disableOutline is not a table");
            }
        }
        else
        {
            return luaL_error(L, "disableOutline([squadIDs])  expects 1 parameter");
        }
    }

    static int LuaGetPossibleAbilities( lua_State *L )
    {
        Core::SquadSystem* squadSystem = Core::world.m_systemHandler.GetSystem<Core::SquadSystem>();
        luau_assert( L, lua_gettop(L) == 1 );

        std::vector<Core::SquadAbility> abilities = squadSystem->GetPossibleAbilities( luaL_checkinteger( L, 1 ) );
        lua_newtable( L );

        for( int i = 0; i < abilities.size(); i++ )
        {
            LuaUNewSquadAbility( L, abilities[i] );
            lua_rawseti(L, -2, i+1); 
        }

        return 1;
    }
}

namespace Core
{
    LuaSquadBridge::LuaSquadBridge(lua_State* L)
    {

        int sanity = lua_gettop(L);

        luaL_newmetatable(L, SQUAD_FORMATION_META_TYPE);
            luau_setfunction(L, "__eq", LuaSquadFormationEq);
            luau_setfunction(L, "__tostring", LuaSquadFormationToString )
        lua_pop(L, 1);

        luaL_newmetatable(L, SQUAD_ABILITY_META_TYPE);
            luau_setfunction(L, "__eq", LuaSquadAbilityEq );
            luau_setfunction(L, "__tostring", LuaSquadAbilityToString )
        lua_pop(L, 1);

        lua_getglobal(L, "core");
            lua_getfield(L, -1, "system");
                lua_newtable(L);
                    luau_setfunction(L, "setSquadGoal", LuaSetSquadGoal);
                    luau_setfunction(L, "setSquadFormation", LuaSetSquadFormation);
                    luau_setfunction(L, "previewSquadFormation", LuaPreviewSquadFormation);
                    luau_setfunction(L, "setSquadStance", LuaSetSquadStance);
                    luau_setfunction(L, "getSquadEntity", LuaGetSquadEntity);
                    luau_setfunction(L, "enableOutline", LuaEnableOutline);
                    luau_setfunction(L, "disableOutline", LuaDisableOutline);
                    luau_setfunction(L, "getPossibleAbilities", LuaGetPossibleAbilities );
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

                        formation = LuaUNewSquadFormation(L);
                        *formation = Core::SquadFormation::HALF_CIRCLE_FORMATION;
                        lua_setfield(L, -2, "HalfCircleFormation");
                    lua_setfield(L, -2, "formations");
                    lua_newtable( L );
                        Core::SquadAbility* ability = LuaUNewSquadAbility(L);    
                        *ability = Core::SquadAbility::SPRINT;
                        lua_setfield(L, -2, "Sprint" );
                    
                        ability = LuaUNewSquadAbility(L);
                        *ability = Core::SquadAbility::FLEE;
                        lua_setfield(L, -2, "Flee" );
    
                        ability = LuaUNewSquadAbility(L);
                        *ability = Core::SquadAbility::ROUT;
                        lua_setfield(L, -2, "Rout" );

                        ability = LuaUNewSquadAbility(L);
                        *ability = Core::SquadAbility::ATTACK;
                        lua_setfield( L, -2, "Attack" );

                        ability = LuaUNewSquadAbility(L);
                        *ability = Core::SquadAbility::ARREST_INDIVIDUAL;            
                        lua_setfield( L, -2, "ArrestIndividual" );

                        ability = LuaUNewSquadAbility(L);
                        *ability = Core::SquadAbility::ARREST_GROUP;
                        lua_setfield( L, -2, "ArrestGroup" );

                        ability = LuaUNewSquadAbility(L);
                        *ability = Core::SquadAbility::TEAR_GAS;
                        lua_setfield( L, -2, "TearGas" );

                        ability = LuaUNewSquadAbility(L);
                        *ability = Core::SquadAbility::TAZE;
                        lua_setfield( L, -2, "Taze" );

                        ability = LuaUNewSquadAbility(L);
                        *ability = Core::SquadAbility::BLITZ;
                        lua_setfield( L, -2, "Blitz" );

                    lua_setfield(L, -2, "abilities");
                lua_setfield(L, -2, "squad");
        lua_pop(L, 2);

        assert(sanity == lua_gettop(L));
    }

}
