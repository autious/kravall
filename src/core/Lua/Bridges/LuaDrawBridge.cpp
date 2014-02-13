#include "LuaDrawBridge.hpp"

#include <gfx/GFXInterface.hpp>
#include <utility/Colors.hpp>

#include <main.hpp>

#include <Lua/LuaMetatableTypes.hpp>
#include <Lua/LuaUtility.hpp>

extern "C" 
{
    static int LuaDrawText( lua_State *L )
    {
        GFX::RenderText( localFontData, glm::vec2( luaL_checkinteger(L ,1 ), luaL_checkinteger( L, 2 ) ),1.0f, Colors::White, luaL_checkstring( L, 3 ) );
		return 0;
    }

    static int LuaDrawTextbox( lua_State *L )
    {
        GFX::RenderTextbox( 
							localFontData, 
							glm::vec4( luaL_checkinteger(L ,1 ), luaL_checkinteger( L, 2 ), luaL_checkinteger(L ,3 ), luaL_checkinteger( L, 4 ) ),
							luaL_checknumber(L, 5),
							1.0f, 
							Colors::White, 
							luaL_checkstring( L, 6 ) 
			);
		return 0;
    }
}

namespace Core
{
    LuaDrawBridge::LuaDrawBridge( lua_State * L )
    {
        lua_getglobal( L, "core" );
            lua_newtable( L );
                luau_setfunction( L, "drawText", LuaDrawText ); 
                luau_setfunction( L, "drawTextbox", LuaDrawTextbox );    
            lua_setfield( L, -2, "draw" );
        lua_pop( L,1 );
    }
}
