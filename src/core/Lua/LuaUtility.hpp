#ifndef LUA_UTILTIY_H
#define LUA_UTILITY_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define luau_setfunction(state, name, func) { lua_pushstring( state, name ); lua_pushcfunction( state, func ); lua_settable( state, -3 ); }
#define luau_push_type(state, type) { lua_pushstring( state, "type" ); lua_pushstring( state, type );  lua_settable( state, -3 ); }
#define luau_push_name(state, name) { lua_pushstring( state, "name" ); lua_pushstring( state, name );  lua_settable( state, -3 ); }
#define luau_setconst( state, var, name ) { lua_pushstring( state, #name ); lua_pushnumber( state, var ); lua_settable( state, -3 ); }
#define luau_setconsti( state, var, index ) { lua_pushinteger( state, index ); lua_pushnumber( state, var ); lua_settable( state, -3 ); }
#define luau_checkfloat( state, id ) (float)luaL_checknumber( state, id )
#define luau_tofloat( state, id ) (float)lua_tonumber( state, id )

#define STREQ( str1, str2 ) strcmp(str1,str2)==0

#define luau_assert(L, expr) { \
    if ( !expr ){ \
        luaL_error(L, "%s:%d:%s(): Assert! \"%s\" failed in evaluation",__FILE__,  __LINE__, __FUNCTION__, #expr );\
    }\
}
#endif
