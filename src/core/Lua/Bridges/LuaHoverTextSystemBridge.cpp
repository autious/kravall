#include "LuaHoverTextSystemBridge.hpp"

#include <Lua/LuaUtility.hpp>
#include <Lua/LuaMetatableTypes.hpp>
#include <World.hpp>

extern "C" 
{
    static int LuaAllocateString( lua_State * L )
    {
        const char * string = luaL_checkstring( L, 1 );
        Core::HoverTextSystem *hts = Core::world.m_systemHandler.GetSystem<Core::HoverTextSystem>();
        LuaHoverText * lht = Core::LuaUNewHoverText( L );

        lht->hoverTextId = hts->AllocateString( string );

        return 1;
    }

    static int LuaReallocateString( lua_State * L )
    {
        LuaHoverText * lht = luau_checkhovertext( L, 1 );
        const char * string  = luaL_checkstring( L, 2 );
        Core::HoverTextSystem *hts = Core::world.m_systemHandler.GetSystem<Core::HoverTextSystem>();

        if( lht->hoverTextId == -1 )
        {
           lht->hoverTextId = hts->AllocateString( string );
        } 
        else
        {
           hts->SetString( lht->hoverTextId, string );
        }

        return 0;
    }

    static int LuaChangeString( lua_State * L )
    {
        LuaHoverText * lht = luau_checkhovertext( L, 1 );
        const char * string = luaL_checkstring( L, 2 );

        Core::HoverTextSystem *hts = Core::world.m_systemHandler.GetSystem<Core::HoverTextSystem>();

        if( lht->hoverTextId != -1 )
        {
            hts->SetString( lht->hoverTextId, string );
        }
        else
        {
            luaL_error( L, "Invalid hover text id" );
        }

        return 0;
    }

    static int LuaFreeString( lua_State * L )
    {
        LuaHoverText * lht = luau_checkhovertext( L, 1 );
        Core::HoverTextSystem *hts = Core::world.m_systemHandler.GetSystem<Core::HoverTextSystem>();

        if( lht->hoverTextId != -1 )
        {
            hts->FreeString( lht->hoverTextId );
            lht->hoverTextId = -1;
        }
        else
        {
            LOG_DEBUG << "Ignoring free of invalid string userdata obeject" << std::endl;
        } 
       
        return 0;
    }
}

namespace Core
{
    LuaHoverTextSystemBridge::LuaHoverTextSystemBridge( lua_State * L )
    {
        int sanity = lua_gettop( L );

        lua_getglobal( L, "core" );
            lua_getfield( L, -1, "system" );
                lua_newtable( L );
                    lua_newtable( L );
                    int hoverIndex = lua_gettop( L );
                        luaL_newmetatable( L, HOVER_TEXT_META_TYPE ); 
                            lua_pushvalue( L, hoverIndex );
                            lua_setfield( L, -2, "__index" );
                            luau_setfunction( L, "__gc", LuaFreeString );
                        lua_pop( L, 1 ); 

                        luau_setfunction( L, "new", LuaAllocateString );
                        luau_setfunction( L, "reallocate", LuaReallocateString );
                        luau_setfunction( L, "set", LuaChangeString );
                        luau_setfunction( L, "free", LuaFreeString );
                    lua_setfield( L, -2, "string" );
                lua_setfield( L, -2, "hoverText" );
        lua_pop( L, 2 );

        assert( lua_gettop( L ) == sanity );
    }
}
