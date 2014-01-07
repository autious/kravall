#ifndef LUA_UTILTIY_H
#define LUA_UTILITY_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define luau_setfunction(state, name, func) { lua_pushstring( state, name ); lua_pushcfunction( state, func ); lua_settable( state, -3 ); }
#define luau_push_type(state, type) { lua_pushstring( state, "type" ); lua_pushstring( state, type );  lua_settable( state, -3 ); }
#define luau_push_name(state, name) { lua_pushstring( state, "name" ); lua_pushstring( state, name );  lua_settable( state, -3 ); }
#define luau_setconst( state, var, name ) { lua_pushstring( state, #name ); lua_pushnumber( state, var ); lua_settable( state, -3 ); }

#define STREQ( str1, str2 ) strcmp(str1,str2)==0


#endif
