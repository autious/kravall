#include "GraphicsComponentBinding.hpp"
#include <World.hpp>

#include <cstring>

#include <logger/Logger.hpp>

#include <Lua/Bridges/LuaGFXBridge.hpp>

Core::ComponentGetters Core::GraphicsComponentBinding::GetGetters()
{
    ComponentGetters getters; 
    getters["mesh"] = []( Core::Entity entity, lua_State * L )
    {
        GraphicsComponent *gc = WGETC<GraphicsComponent>( entity );
        
        lua_pushnumber( L, GFX::GetBitmaskValue( gc->bitmask, GFX::BITMASK::MESH_ID ) );
        
        return 1;
    };

    getters["material"] = []( Core::Entity entity, lua_State * L )
    {
        GraphicsComponent *gc = WGETC<GraphicsComponent>( entity );

        lua_pushnumber( L, GFX::GetBitmaskValue( gc->bitmask, GFX::BITMASK::MATERIAL_ID ) );
    
        return 1;
    };

    getters["type"] = []( Core::Entity entity, lua_State * L )
    {
        GraphicsComponent *gc = WGETC<GraphicsComponent>( entity );
        
        GFXObjectType * gfxot = (GFXObjectType*)lua_newuserdata( L, sizeof( GFXObjectType ) );
        *gfxot = GFX::GetBitmaskValue( gc->bitmask, GFX::BITMASK::TYPE );
        
        luaL_newmetatable( L, GFX_OBJECT_TYPE_META );
        lua_setmetatable( L, -2 );
    
        return 1;
    };

    return getters;
}

Core::ComponentSetters Core::GraphicsComponentBinding::GetSetters()
{
    ComponentSetters setters;

    setters["mesh"] = [](Core::Entity entity, lua_State * L, int valueindex )
    {
        GraphicsComponent *gc = WGETC<GraphicsComponent>( entity );

        if( lua_isnumber( L, valueindex ) )
        {
            GFX::SetBitmaskValue( gc->bitmask, GFX::BITMASK::MESH_ID, lua_tointeger( L, valueindex ) );
        }  
        else
        {
            luaL_error( L, "Unable to set mesh, given parameter is not integer value" );
        }
    };

    setters["material"] = [](Core::Entity entity, lua_State * L, int valueindex )
    {
        GraphicsComponent *gc = WGETC<GraphicsComponent>( entity );
            
        if( lua_isnumber( L, valueindex ) )
        {
            GFX::SetBitmaskValue( gc->bitmask, GFX::BITMASK::MATERIAL_ID, lua_tointeger( L, valueindex ) );
        }  
        else
        {
            luaL_error( L, "Unable to set material, given parameter is not integer value" );
        }
    };

    setters["type"] = [](Core::Entity entity, lua_State * L, int valueindex )
    {
        GraphicsComponent *gc = WGETC<GraphicsComponent>( entity );

        GFXObjectType * gfxot = (GFXObjectType*)luaL_checkudata( L, valueindex, GFX_OBJECT_TYPE_META );

        GFX::SetBitmaskValue( gc->bitmask, GFX::BITMASK::TYPE, *gfxot );
    };

    return setters;
}

Core::ComponentType Core::GraphicsComponentBinding::GetComponentType()
{
    return EntityHandler::GetComponentType<GraphicsComponent>();
}

const char* Core::GraphicsComponentBinding::GetComponentLuaName()
{
    return "GraphicsComponent";
}
