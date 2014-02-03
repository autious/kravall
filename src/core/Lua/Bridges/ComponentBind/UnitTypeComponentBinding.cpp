#include "UnitTypeComponentBinding.hpp"

#include <World.hpp>

#include <Lua/LuaMetatableTypes.hpp>

Core::ComponentGetters Core::UnitTypeComponentBinding::GetGetters()
{
    ComponentGetters getters;

    getters["unitType"] = []( Core::Entity entity, lua_State *L )
    {
        UnitTypeComponent *utc = WGETC<UnitTypeComponent>( entity ); 
        
        Core::UnitType* type = (Core::UnitType*)lua_newuserdata( L, sizeof( Core::UnitType ) );
        *type = utc->type;
        
        luaL_newmetatable( L, UNIT_TYPE_OBJECT_TYPE_META );
        lua_setmetatable( L, -2 );
    
        return 1;
    };

    return getters;

}

Core::ComponentSetters Core::UnitTypeComponentBinding::GetSetters()
{
    ComponentSetters setters;

    setters["unitType"] = []( Core::Entity entity, lua_State * L, int valueindex )
    {
        UnitTypeComponent *utc = WGETC<UnitTypeComponent>( entity ); 
		utc->type = *(Core::UnitType*)luaL_checkudata( L, valueindex, UNIT_TYPE_OBJECT_TYPE_META );
    };

    return setters;
}


Core::ComponentType Core::UnitTypeComponentBinding::GetComponentType()
{
    return EntityHandler::GetComponentType<UnitTypeComponent>();
}

const char* Core::UnitTypeComponentBinding::GetComponentLuaName()
{
    return "UnitTypeComponent";
}
