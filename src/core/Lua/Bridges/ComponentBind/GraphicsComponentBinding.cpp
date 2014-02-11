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


	getters["layer"] = [](Core::Entity entity, lua_State * L)
	{
		GraphicsComponent *gc = WGETC<GraphicsComponent>(entity);

		GFXObjectType * gfxot = (GFXObjectType*)lua_newuserdata(L, sizeof(GFXObjectType));
		*gfxot = GFX::GetBitmaskValue(gc->bitmask, GFX::BITMASK::LAYER);

		luaL_newmetatable(L, GFX_LAYER_TYPE_META);
		lua_setmetatable(L, -2);

		return 1;
	};

    getters["render"] = []( Core::Entity entity, lua_State *L )
    {
        GraphicsComponent *gc = WGETC<GraphicsComponent>( entity );
        
        lua_pushboolean( L, gc->render ); 

        return 1;
    };

	getters["outlineColor"] = [](Core::Entity entity, lua_State *L)
	{
		GraphicsComponent *gc = WGETC<GraphicsComponent>(entity);

		lua_newtable(L);
        int index = lua_gettop(L); 

		for (int i = 0; i < 4; i++)
		{
			lua_pushnumber(L, gc->outlineColor[i]);
			lua_rawseti( L, index, i + 1 );
		}

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

	setters["layer"] = [](Core::Entity entity, lua_State * L, int valueindex)
	{
		GraphicsComponent *gc = WGETC<GraphicsComponent>(entity);

		GFXLayerType * gfxot = (GFXLayerType*)luaL_checkudata(L, valueindex, GFX_LAYER_TYPE_META);

		GFX::SetBitmaskValue(gc->bitmask, GFX::BITMASK::LAYER, *gfxot);
	};

    setters["render"] = [](Core::Entity entity, lua_State * L, int valueindex )
    {
        GraphicsComponent *gc = WGETC<GraphicsComponent>( entity );

        gc->render = lua_toboolean( L, valueindex );
    };

	setters["outlineColor"] = [](Core::Entity entity, lua_State * L, int valueindex)
	{
		GraphicsComponent *gc = WGETC<GraphicsComponent>(entity);

		if (lua_istable(L, valueindex))
		{
			for (int i = 0; i < 4; i++)
			{
				lua_pushinteger(L, i + 1);
				lua_gettable(L, valueindex);

				if (lua_isnumber(L, -1))
				{
					gc->outlineColor[i] = static_cast<float>(lua_tonumber(L, -1));
				}

				lua_pop(L, 1);
			}
		}
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
