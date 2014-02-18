#include "HoverTextComponentBinding.hpp"

#include <Lua/LuaMetatableTypes.hpp>

#include <SystemDef.hpp>
#include <World.hpp>

namespace Core
{
    ComponentGetters HoverTextComponentBinding::GetGetters()
    {
        ComponentGetters getters; 
        getters["hoverTextId"] = []( Core::Entity entity, lua_State * L )
        {
            LuaHoverText *lht = LuaUNewHoverText( L );
            HoverTextComponent *htc = WGETC<HoverTextComponent>( entity );
            
            lht->hoverTextId = htc->hoverTextId;

            return 1;
        };

        getters["color"] = []( Core::Entity entity, lua_State * L )
        {
            HoverTextComponent *htc = WGETC<HoverTextComponent>( entity );
            lua_newtable(L);
                lua_pushnumber( L, htc->color[0] );
                lua_rawseti( L, -2, 1 ); 
                lua_pushnumber( L, htc->color[1] );
                lua_rawseti( L, -2, 2 ); 
                lua_pushnumber( L, htc->color[2] );
                lua_rawseti( L, -2, 3 ); 
                lua_pushnumber( L, htc->color[4] );
                lua_rawseti( L, -2, 4 ); 
            return 1;
        };

        getters["render"] = []( Core::Entity entity, lua_State * L )
        {
            HoverTextComponent *htc = WGETC<HoverTextComponent>( entity );

            lua_pushboolean( L, htc->render );
            return 1;
        };

        getters["font"] = []( Core::Entity entity, lua_State * L )
        {
            HoverTextComponent *htc = WGETC<HoverTextComponent>( entity );

            if( htc->font != nullptr )
            {
                LuaTTFFont *luaFont = LuaUNewTTFFont( L );
                luaFont->fontPtr = htc->font;
            }
            else
            {
                lua_pushnil( L ); 
            }
            
            return 1;
        };
        

        return getters;
    }

    ComponentSetters HoverTextComponentBinding::GetSetters()
    {
        ComponentSetters setters;

        setters["hoverTextId"] = [](Core::Entity entity, lua_State * L, int valueindex )
        {
            LuaHoverText *lht = luau_checkhovertext( L, valueindex );
            HoverTextComponent *htc = WGETC<HoverTextComponent>( entity );

            htc->hoverTextId = lht->hoverTextId;
        };

        setters["color"] = [](Core::Entity entity, lua_State * L, int valueindex )
        {
            if( lua_istable( L, valueindex ) )
            {
                HoverTextComponent *htc = WGETC<HoverTextComponent>( entity );

                for( int i = 0; i < 4; i++ )
                {
                    lua_rawgeti( L, valueindex, i+1 );
                    htc->color[i] = luau_checkfloat( L, -1 );
                    lua_pop(L,1);
                }
            }
            else
            {
                luaL_error( L, "Value color in HoverTextComponent is not table" );
            }
        };

        setters["render"] = [](Core::Entity entity, lua_State * L, int valueindex )
        {
            if( lua_isboolean( L, valueindex ) )
            {
                HoverTextComponent *htc = WGETC<HoverTextComponent>( entity );
                htc->render = lua_toboolean( L, valueindex );
            }
            else
            {
                luaL_error( L, "Value hide in HoverTextComponent is not boolean" );
            }
        };

        setters["font"] = [](Core::Entity entity, lua_State * L, int valueindex )
        {
            if( lua_isuserdata( L, valueindex ) )
            {
                HoverTextComponent *htc = WGETC<HoverTextComponent>( entity );
                LuaTTFFont *luaFont = luau_checkttffont( L, valueindex );
                htc->font = luaFont->fontPtr;
            }
            else
            {
                luaL_error( L, "Value font in HoverTextComponent is not font data" );
            }
        };

        return setters;
    }

    ComponentType HoverTextComponentBinding::GetComponentType()
    {
        return EntityHandler::GetComponentType<HoverTextComponent>();
    }

    const char* HoverTextComponentBinding::GetComponentLuaName()
    {
        return "HoverTextComponent";
    }
}
