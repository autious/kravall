#include "AttributeComponentBinding.hpp"

#include <World.hpp>

#include <Lua/Bridges/LuaAttributeBridge.hpp>

Core::ComponentGetters Core::AttributeComponentBinding::GetGetters()
{
    ComponentGetters getters; 

	// common...
	getters["health"] = []( Core::Entity entity, lua_State *L )
    {
        AttributeComponent *atrbc = WGETC<AttributeComponent>( entity ); 

        lua_pushnumber( L, atrbc->health );   
        return 1;
    };

	getters["stamina"] = []( Core::Entity entity, lua_State *L )
    {
        AttributeComponent *atrbc = WGETC<AttributeComponent>( entity ); 

        lua_pushnumber( L, atrbc->stamina );   
        return 1;
    };

	getters["morale"] = []( Core::Entity entity, lua_State *L )
    {
        AttributeComponent *atrbc = WGETC<AttributeComponent>( entity ); 
        lua_pushnumber( L, atrbc->morale );   
        return 1;
    };

	// police attributes...
	getters["stancePolice"] = []( Core::Entity entity, lua_State *L )
    {
		AttributeComponent *atrbc = WGETC<AttributeComponent>( entity ); 
        
        Core::PoliceStance* stance = (Core::PoliceStance*)lua_newuserdata( L, sizeof( Core::PoliceStance ) );
        *stance = atrbc->police.stance;
        
        luaL_newmetatable( L, ATTRIBUTE_POLICE_COMPONENT_TYPE_META );
        lua_setmetatable( L, -2 );
    
        return 1;
    };

	getters["statePolice"] = [](Core::Entity entity, lua_State *L)
	{
		AttributeComponent *atrbc = WGETC<AttributeComponent>(entity);

		Core::PoliceState* state = (Core::PoliceState*)lua_newuserdata(L, sizeof(Core::PoliceState));
		*state = atrbc->police.state;

		luaL_newmetatable(L, ATTRIBUTE_POLICE_COMPONENT_TYPE_META);
		lua_setmetatable(L, -2);

		return 1;
	};

	getters["defense"] = []( Core::Entity entity, lua_State *L )
    {
        AttributeComponent *atrbc = WGETC<AttributeComponent>( entity ); 

        lua_pushnumber( L, atrbc->police.defense );
        return 1;
    };

	getters["mobility"] = []( Core::Entity entity, lua_State *L )
    {
        AttributeComponent *atrbc = WGETC<AttributeComponent>( entity ); 

        lua_pushnumber( L, atrbc->police.mobility );   
        return 1;
    };

	getters["squadID"] = []( Core::Entity entity, lua_State *L )
    {
        AttributeComponent *atrbc = WGETC<AttributeComponent>( entity ); 

        lua_pushnumber( L, atrbc->police.squadID );   
        return 1;
    };

	// rioter attributes...
	getters["alignment"] = []( Core::Entity entity, lua_State *L )
    {
        AttributeComponent *atrbc = WGETC<AttributeComponent>( entity ); 
        
        Core::RioterAlignment* alignment = (Core::RioterAlignment*)lua_newuserdata( L, sizeof( Core::RioterAlignment ) );
        *alignment = atrbc->rioter.alignment;
        
        luaL_newmetatable( L, ATTRIBUTE_RIOTER_COMPONENT_TYPE_META );
        lua_setmetatable( L, -2 );
    
        return 1;
    };

	getters["rage"] = []( Core::Entity entity, lua_State *L )
    {
        AttributeComponent *atrbc = WGETC<AttributeComponent>( entity ); 

        lua_pushnumber( L, atrbc->rioter.rage );   
        return 1;
    };

	getters["pressure"] = []( Core::Entity entity, lua_State *L )
    {
        AttributeComponent *atrbc = WGETC<AttributeComponent>( entity ); 

        lua_pushnumber( L, atrbc->rioter.pressure );   
        return 1;
    };

	getters["groupID"] = []( Core::Entity entity, lua_State *L )
    {
        AttributeComponent *atrbc = WGETC<AttributeComponent>( entity ); 

        lua_pushnumber( L, atrbc->rioter.groupID );   
        return 1;
    };

	getters["stanceRioter"] = [](Core::Entity entity, lua_State *L)
	{
		AttributeComponent *atrbc = WGETC<AttributeComponent>(entity);
		Core::RioterStance* stance = (Core::RioterStance*)lua_newuserdata(L, sizeof(Core::RioterStance));
		*stance = atrbc->rioter.stance;
	
		luaL_newmetatable(L, ATTRIBUTE_RIOTER_COMPONENT_TYPE_META);
		lua_setmetatable(L, -2);
		return 1;
	};

    return getters;
}

Core::ComponentSetters Core::AttributeComponentBinding::GetSetters()
{
    ComponentSetters setters;

	// common...
    setters["health"] = []( Core::Entity entity, lua_State * L, int valueindex )
    {
        AttributeComponent *atrbc = WGETC<AttributeComponent>( entity ); 
        
        if( lua_isnumber(  L, valueindex ) )
        {
            atrbc->health = static_cast<int>(lua_tonumber( L, valueindex ));
        }
        else
        {
            luaL_error( L, "Unable to set health for ent %d, value is not number", entity );
        }
    };

	setters["stamina"] = []( Core::Entity entity, lua_State * L, int valueindex )
    {
        AttributeComponent *atrbc = WGETC<AttributeComponent>( entity ); 
        
        if( lua_isnumber(  L, valueindex ) )
        {
            atrbc->stamina = static_cast<int>(lua_tonumber( L, valueindex ));
        }
        else
        {
            luaL_error( L, "Unable to set stamina for ent %d, value is not number", entity );
        }
    };

	setters["morale"] = []( Core::Entity entity, lua_State * L, int valueindex )
    {
        AttributeComponent *atrbc = WGETC<AttributeComponent>( entity ); 
        
        if( lua_isnumber(  L, valueindex ) )
        {
            atrbc->morale = static_cast<float>(lua_tonumber( L, valueindex ));
        }
        else
        {
            luaL_error( L, "Unable to set morale for ent %d, value is not number", entity );
        }
    };

	// police...
	setters["stancePolice"] = []( Core::Entity entity, lua_State * L, int valueindex )
    {
        AttributeComponent *atrbc = WGETC<AttributeComponent>( entity ); 
        atrbc->police.stance = *(Core::PoliceStance*)luaL_checkudata( L, valueindex, ATTRIBUTE_POLICE_COMPONENT_TYPE_META );
    };

	setters["statePolice"] = [](Core::Entity entity, lua_State * L, int valueindex)
	{
		AttributeComponent *atrbc = WGETC<AttributeComponent>(entity);
		atrbc->police.state = *(Core::PoliceState*)luaL_checkudata(L, valueindex, ATTRIBUTE_POLICE_COMPONENT_TYPE_META);
	};

	setters["defense"] = []( Core::Entity entity, lua_State * L, int valueindex )
    {
        AttributeComponent *atrbc = WGETC<AttributeComponent>( entity ); 
        
        if( lua_isnumber(  L, valueindex ) )
        {
            atrbc->police.defense = static_cast<int>(lua_tonumber( L, valueindex ));
        }
        else
        {
            luaL_error( L, "Unable to set defense for ent %d, value is not number", entity );
        }
    };

	setters["mobility"] = []( Core::Entity entity, lua_State * L, int valueindex )
    {
        AttributeComponent *atrbc = WGETC<AttributeComponent>( entity ); 
        
        if( lua_isnumber(  L, valueindex ) )
        {
            atrbc->police.mobility = static_cast<float>(lua_tonumber( L, valueindex ));
        }
        else
        {
            luaL_error( L, "Unable to set mobility for ent %d, value is not number", entity );
        }
    };

	setters["squadID"] = []( Core::Entity entity, lua_State * L, int valueindex )
    {
        AttributeComponent *atrbc = WGETC<AttributeComponent>( entity ); 
        
        if( lua_isnumber(  L, valueindex ) )
        {
            atrbc->police.squadID = static_cast<int>(lua_tonumber( L, valueindex ));
        }
        else
        {
            luaL_error( L, "Unable to set pressure for ent %d, value is not number", entity );
        }
    };

	// rioter...
	setters["alignment"] = []( Core::Entity entity, lua_State * L, int valueindex )
    {
        AttributeComponent *atrbc = WGETC<AttributeComponent>( entity ); 
        atrbc->rioter.alignment = *(Core::RioterAlignment*)luaL_checkudata( L, valueindex, ATTRIBUTE_RIOTER_COMPONENT_TYPE_META );
    };

	setters["rage"] = []( Core::Entity entity, lua_State * L, int valueindex )
    {
        AttributeComponent *atrbc = WGETC<AttributeComponent>( entity ); 
        
        if( lua_isnumber(  L, valueindex ) )
        {
            atrbc->rioter.rage = static_cast<int>(lua_tonumber( L, valueindex ));
        }
        else
        {
            luaL_error( L, "Unable to set rage for ent %d, value is not number", entity );
        }
    };

	setters["pressure"] = []( Core::Entity entity, lua_State * L, int valueindex )
    {
        AttributeComponent *atrbc = WGETC<AttributeComponent>( entity ); 
        
        if( lua_isnumber(  L, valueindex ) )
        {
            atrbc->rioter.pressure = static_cast<int>(lua_tonumber( L, valueindex ));
        }
        else
        {
            luaL_error( L, "Unable to set pressure for ent %d, value is not number", entity );
        }
    };

	setters["groupID"] = []( Core::Entity entity, lua_State * L, int valueindex )
    {
        AttributeComponent *atrbc = WGETC<AttributeComponent>( entity ); 
        
        if( lua_isnumber(  L, valueindex ) )
        {
            atrbc->rioter.groupID = static_cast<int>(lua_tonumber( L, valueindex ));
        }
        else
        {
            luaL_error( L, "Unable to set pressure for ent %d, value is not number", entity );
        }
    };

	setters["stanceRioter"] = [](Core::Entity entity, lua_State * L, int valueindex)
	{
		AttributeComponent *atrbc = WGETC<AttributeComponent>(entity);
		atrbc->rioter.stance = *(Core::RioterStance*)luaL_checkudata(L, valueindex, ATTRIBUTE_RIOTER_COMPONENT_TYPE_META);
	};

    return setters;
}


Core::ComponentType Core::AttributeComponentBinding::GetComponentType()
{
    return EntityHandler::GetComponentType<AttributeComponent>();
}

const char* Core::AttributeComponentBinding::GetComponentLuaName()
{
    return "AttributeComponent";
}
