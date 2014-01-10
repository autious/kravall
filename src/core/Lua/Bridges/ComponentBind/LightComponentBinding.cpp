#include "LightComponentBinding.hpp"
#include <World.hpp>

#include <cstring>

#include <logger/Logger.hpp>

#include <Lua/Bridges/LuaGFXBridge.hpp>

Core::ComponentGetters Core::LightComponentBinding::GetGetters()
{
    ComponentGetters getters; 

    getters["type"] = []( Core::Entity entity, lua_State * L )
    {
        LightComponent *lc = WGETC<LightComponent>( entity );
        
        GFXObjectType * gfxot = (GFXObjectType*)lua_newuserdata( L, sizeof( GFXObjectType ) );
        *gfxot = GFX::GetBitmaskValue( lc->bitmask, GFX::BITMASK::TYPE );
        
        luaL_newmetatable( L, GFX_OBJECT_TYPE_META );
        lua_setmetatable( L, -2 );
    
        return 1;
    };

    getters["lighttype"] = []( Core::Entity entity, lua_State * L )
    {
        LightComponent *lc = WGETC<LightComponent>( entity );
        
        GFXLightType * gfxot = (GFXLightType*)lua_newuserdata( L, sizeof( GFXLightType ) );
        *gfxot = LightComponent::GetLightType( *lc ); 
        
        luaL_newmetatable( L, GFX_LIGHT_TYPE_META );
        lua_setmetatable( L, -2 );
    
        return 1;
    };

    getters["color"] = []( Core::Entity entity, lua_State * L )
    {
        LightComponent *lc = WGETC<LightComponent>( entity );
        
        lua_newtable( L );  

        for( int i = 0; i < 3; i++ )
        {
            lua_pushinteger( L, i+1 );
            lua_pushnumber( L, lc->color[i] );
            lua_settable( L, -3 );
        }
        
        return 1;
    };

    getters["intensity"] = []( Core::Entity entity, lua_State * L )
    {
        LightComponent *lc = WGETC<LightComponent>( entity );

        lua_pushnumber( L, lc->intensity );
    
        return 1;
    };

    getters["spotlightangle"] = []( Core::Entity entity, lua_State * L )
    {
        LightComponent *lc = WGETC<LightComponent>( entity );

        lua_pushnumber( L, lc->lightSpecific.spotLight.angle );
    
        return 1;
    };

    return getters;
}

Core::ComponentSetters Core::LightComponentBinding::GetSetters()
{
    ComponentSetters setters;

    setters["type"] = [](Core::Entity entity, lua_State * L, int valueindex )
    {
        LightComponent *gc = WGETC<LightComponent>( entity );

        GFXObjectType * gfxot = (GFXObjectType*)luaL_checkudata( L, valueindex, GFX_OBJECT_TYPE_META );

        GFX::SetBitmaskValue( gc->bitmask, GFX::BITMASK::TYPE, *gfxot );
    };

    setters["lighttype"] = [](Core::Entity entity, lua_State * L, int valueindex )
    {
        LightComponent *gc = WGETC<LightComponent>( entity );

        GFXLightType * gfxot = (GFXLightType*)luaL_checkudata( L, valueindex, GFX_LIGHT_TYPE_META );

        GFX::SetBitmaskValue( gc->bitmask, GFX::BITMASK::LIGHT_TYPE, *gfxot );
    };

    setters["color"] = [](Core::Entity entity, lua_State * L, int valueindex )
    {
        LightComponent *lc = WGETC<LightComponent>( entity );

        if( lua_istable( L, valueindex ) )
        {
            for( int i = 0; i < 3; i++ )
            {
                lua_pushinteger( L, i+1 );
                lua_gettable( L, valueindex );
                    
                if( lua_isnumber( L, -1 ) )
                {
                    lc->color[i] = static_cast<float>(lua_tonumber( L, -1 ));  
                }

                lua_pop( L, 1 );
            }
        }
    };

    setters["intensity"] = [](Core::Entity entity, lua_State * L, int valueindex )
    {
        LightComponent *lc = WGETC<LightComponent>( entity );
            
        if( lua_isnumber( L, valueindex ) )
        {
            lc->intensity = static_cast<float>(lua_tonumber( L, valueindex ));
        }  
        else
        {
            luaL_error( L, "Unable to set material, given parameter is not integer value" );
        }
    };

    setters["spotlightangle"] = [](Core::Entity entity, lua_State * L, int valueindex )
    {
        LightComponent *lc = WGETC<LightComponent>( entity );
            
        if( lua_isnumber( L, valueindex ) )
        {
            lc->lightSpecific.spotLight.angle = static_cast<float>(lua_tonumber( L, valueindex ));
        }  
        else
        {
            luaL_error( L, "Unable to set material, given parameter is not integer value" );
        }
    };

    

    return setters;
}

Core::ComponentType Core::LightComponentBinding::GetComponentType()
{
    return EntityHandler::GetComponentType<LightComponent>();
}

const char* Core::LightComponentBinding::GetComponentLuaName()
{
    return "LightComponent";
}
