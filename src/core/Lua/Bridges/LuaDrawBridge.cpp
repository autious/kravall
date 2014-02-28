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

	static int LuaDrawTextbox(lua_State *L)
	{
		GFX::RenderTextbox(
			localFontData,
			glm::vec4(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3), luaL_checkinteger(L, 4)),
			luaL_checknumber(L, 5),
			1.0f,
			Colors::White,
			luaL_checkstring(L, 6)
			);
		return 0;

	}

    static int LuaDrawRectangle( lua_State * L )
    {
        GFX::Debug::DrawRectangle( glm::vec2( luau_tofloat( L, 1 ), luau_tofloat( L, 2 ) ), glm::vec2( luau_tofloat( L, 3 ), luau_tofloat( L , 4 ) ), lua_toboolean( L, 5 ), Colors::Blue );

        return 0;
    }

    static int LuaDrawLine( lua_State * L )
    {
        GFX::Debug::DrawLine( glm::vec3( luau_tofloat( L, 1 ), luau_tofloat( L, 2 ), luau_tofloat( L, 3 )), glm::vec3( luau_tofloat( L, 4 ), luau_tofloat( L , 5 ), luau_tofloat( L, 6 )), Color(luau_tofloat(L, 7), luau_tofloat(L, 8), luau_tofloat(L, 9), 1.0f), luau_tofloat(L, 10), true );
        
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
                luau_setfunction( L, "drawRectangle", LuaDrawRectangle );
                luau_setfunction( L, "drawLine", LuaDrawLine );

            lua_setfield( L, -2, "draw" );
        lua_pop( L,1 );
    }
}