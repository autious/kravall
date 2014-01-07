#include "LuaBitmask.hpp"

#include <logger/Logger.hpp>
#include <Lua/LuaUtility.hpp>
#include <Lua/LuaMetatableTypes.hpp>

extern "C"
{
    static int LuaNew( lua_State *L )
    {
        uint64_t * bitmaskRet = Core::LuaUNewBitmask(L);

        if( lua_gettop( L )  == 1 )
        {
            uint64_t * bitmaskFirst = (uint64_t*)luaL_checkudata( L, 1, BITMASK_META_TYPE );

            *bitmaskRet = *bitmaskFirst;
            return 1;
        }
        else if( lua_gettop( L ) == 0 )
        {
            *bitmaskRet = 0ULL;
            return 1;
        }
        else
        {
            return luaL_error( L, "Invalid call, need bitmask or no parameter" );
        }
    }

    static int LuaOr( lua_State *L )
    {
        uint64_t * bitmaskFirst = (uint64_t*)luaL_checkudata( L, 1, BITMASK_META_TYPE );
        uint64_t * bitmaskSecond = (uint64_t*)luaL_checkudata( L, 2, BITMASK_META_TYPE );

        uint64_t * bitmaskRet = Core::LuaUNewBitmask(L);

        *bitmaskRet = *bitmaskFirst | *bitmaskSecond;

        return 1;
    }

    static int LuaAnd( lua_State *L )
    {
        uint64_t * bitmaskFirst = (uint64_t*)luaL_checkudata( L, 1, BITMASK_META_TYPE );
        uint64_t * bitmaskSecond = (uint64_t*)luaL_checkudata( L, 2, BITMASK_META_TYPE );

        uint64_t * bitmaskRet = Core::LuaUNewBitmask(L);

        *bitmaskRet = *bitmaskFirst & *bitmaskSecond;

        return 1;
    }

    static int LuaXor( lua_State *L )
    {
        uint64_t * bitmaskFirst = (uint64_t*)luaL_checkudata( L, 1, BITMASK_META_TYPE );
        uint64_t * bitmaskSecond = (uint64_t*)luaL_checkudata( L, 2, BITMASK_META_TYPE );

        uint64_t * bitmaskRet = Core::LuaUNewBitmask(L);

        *bitmaskRet = *bitmaskFirst ^ *bitmaskSecond;

        return 1;
    }

    static int LuaNot( lua_State *L )
    {
        uint64_t * bitmaskFirst = (uint64_t*)luaL_checkudata( L, 1, BITMASK_META_TYPE );
        uint64_t * bitmaskRet = Core::LuaUNewBitmask(L);

        *bitmaskRet = ~(*bitmaskFirst);

        return 1;
    }

    static int LuaEq( lua_State *L )
    {
        uint64_t * bitmaskFirst = (uint64_t*)luaL_checkudata( L, 1, BITMASK_META_TYPE );
        uint64_t * bitmaskSecond = (uint64_t*)luaL_checkudata( L, 2, BITMASK_META_TYPE );

        lua_pushboolean( L, *bitmaskFirst == *bitmaskSecond ? 1 : 0 );

        return 1;
    }
}

namespace Core
{
    void LuaBitmask::OpenLibs( lua_State * L )
    {
        int sanity = lua_gettop(L);
        lua_getglobal( L, "core" );

        if( !lua_isnil( L, -1 ) )
        {
            lua_pushstring( L, "bitmask" );
            lua_newtable( L );
            int bitmaskTable = lua_gettop(L);

                luau_setfunction( L, "new", LuaNew );
                luau_setfunction( L, "and", LuaAnd ); 
                luau_setfunction( L, "or", LuaOr );
                luau_setfunction( L, "xor", LuaXor );
                luau_setfunction( L, "not", LuaNot );
                luau_setfunction( L, "eq", LuaEq );

                luaL_newmetatable( L, BITMASK_META_TYPE );
                    lua_pushvalue( L, bitmaskTable );
                    lua_setfield( L, -2, "__index" );

                    luau_setfunction( L, "__eq", LuaEq );
                lua_pop( L, 1 ); //Metatable 
            lua_settable( L, -3 );
        } 
        else
        {
            LOG_ERROR << "Missing core table" << std::endl; 
        }
        
        lua_pop( L, 1 ); //core

        assert( sanity == lua_gettop( L ) );
    }
}
