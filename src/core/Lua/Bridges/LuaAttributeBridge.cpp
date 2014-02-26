#include "LuaAttributeBridge.hpp"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <Components/AttributeComponent.hpp>
#include <Lua/LuaMetatableTypes.hpp>
#include <Lua/LuaUtility.hpp>
#include <logger/Logger.hpp>
#include <cassert>


extern "C"
{

    static int LuaStanceEq(lua_State* L)
    {
        Core::PoliceStance* lhs = luau_checkpolicestance(L, 1);
        Core::PoliceStance* rhs = luau_checkpolicestance(L, 2);
        lua_pushboolean(L, *lhs == *rhs);

        return 1;
    }

    static int LuaStanceToString( lua_State * L )
    {
        Core::PoliceStance* lhs = luau_checkpolicestance(L, 1);

        switch( *lhs )
        {
            case Core::PoliceStance::Passive:
                lua_pushstring( L, "PoliceStance.Passive" );
                break;
            case Core::PoliceStance::Aggressive:
                lua_pushstring( L, "PoliceStance.Aggressive" );
                break;
            case Core::PoliceStance::Defensive:
                lua_pushstring( L, "PoliceStance.Defensive" );
                break;
            default:
                lua_pushstring( L, "PoliceStance.NOT_IN_TABLE" );
                break;
        }

        return 1;
    }
}

static void PushPoliceObjectType( lua_State * L, const unsigned int value, const char * name, int table )
{
        lua_pushstring( L, name ); 
        unsigned int* uvalue= (unsigned int*)lua_newuserdata( L, sizeof( Core::PoliceStance ) );
        *uvalue = value;

            luaL_newmetatable( L, ATTRIBUTE_POLICE_COMPONENT_META_TYPE );
            lua_setmetatable( L, -2 );
        lua_settable( L, table );
}

static void PushRioterObjectType( lua_State * L, const unsigned int value, const char * name, int table )
{
        lua_pushstring( L, name );
        unsigned int* uvalue= (unsigned int*)lua_newuserdata( L, sizeof( Core::PoliceStance ) );
        *uvalue = value;

            luaL_newmetatable( L, ATTRIBUTE_RIOTER_COMPONENT_META_TYPE );
            lua_setmetatable( L, -2 );
        lua_settable( L, table );
}

static void PushPFObjectObjectType(lua_State * L, const unsigned int value, const char * name, int table)
{
	lua_pushstring(L, name);
	unsigned int* uvalue = (unsigned int*)lua_newuserdata(L, sizeof(Core::PoliceStance));
	*uvalue = value;

	luaL_newmetatable(L, ATTRIBUTE_PF_OBJECT_COMPONENT_META_TYPE);
	lua_setmetatable(L, -2);
	lua_settable(L, table);
}


Core::LuaAttributeComponentBridge::LuaAttributeComponentBridge( lua_State * L )
{

    int sanity = lua_gettop(L);

    luaL_newmetatable(L, POLICE_STANCE_META_TYPE);
        luau_setfunction(L, "__eq", LuaStanceEq);
        luau_setfunction(L, "__tostring", LuaStanceToString );
    lua_pop(L, 1);

    lua_getglobal( L, "core" );
        lua_newtable( L );
            Core::PoliceStance* stance = LuaUNewPoliceStance(L);
                *stance = Core::PoliceStance::Passive;
            lua_setfield(L, -2, "Passive");

            stance = LuaUNewPoliceStance(L);
                *stance = Core::PoliceStance::Defensive;
            lua_setfield(L, -2, "Defensive");

            stance = LuaUNewPoliceStance(L);
            *   stance = Core::PoliceStance::Aggressive;
            lua_setfield(L, -2, "Aggressive");
        lua_setfield(L, -2, "PoliceStance");    

        //lua_pushstring(L, "PoliceState");
        lua_newtable(L);
            int policeStateTable = lua_gettop(L);
            PushPoliceObjectType(L, Core::PoliceState::PS_Normal, "Normal", policeStateTable);
            PushPoliceObjectType(L, Core::PoliceState::PS_Attacking, "Attacking", policeStateTable);
            PushPoliceObjectType(L, Core::PoliceState::PS_Fleeing, "Fleeing", policeStateTable);
            PushPoliceObjectType(L, Core::PoliceState::PS_Routing, "Routing", policeStateTable);
        lua_setfield(L, -2, "PoliceState");

        lua_newtable( L ); // new table
            int rioterAlignmentTable = lua_gettop( L );
            PushRioterObjectType( L, Core::RioterAlignment::Anarchist, "Anarchist" , rioterAlignmentTable );
            PushRioterObjectType( L, Core::RioterAlignment::Pacifist, "Pacifist" , rioterAlignmentTable );
        lua_setfield(L, -2, "RioterAlignment");    

        lua_newtable(L); // new table
            int rioterStanceTable = lua_gettop(L);    
            PushRioterObjectType(L, Core::RioterStance::Normal, "Normal", rioterStanceTable);
            PushRioterObjectType(L, Core::RioterStance::Agitated, "Agitated", rioterStanceTable);
            PushRioterObjectType(L, Core::RioterStance::Attacking, "Attacking", rioterStanceTable);
            PushRioterObjectType(L, Core::RioterStance::Retreating, "Retreating", rioterStanceTable);
            PushRioterObjectType(L, Core::RioterStance::Civilian, "Civilian", rioterStanceTable);
        lua_setfield(L,-2, "RioterStance");

		lua_newtable(L); // new table
		int pfObjectTypeTable = lua_gettop(L);
			PushPFObjectObjectType(L, Core::PFObjectTypes::TearGas, "TearGas", pfObjectTypeTable);
			PushPFObjectObjectType(L, Core::PFObjectTypes::Molotov, "Molotov", pfObjectTypeTable);
		lua_setfield(L, -2, "PFObjectType");

        //lua_settable( L, coreTableIndex );

	lua_pop( L, 1 );

	assert( sanity == lua_gettop( L ) );
}

