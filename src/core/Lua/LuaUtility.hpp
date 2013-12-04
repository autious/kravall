#ifndef LUA_UTILTIY_H
#define LUA_UTILITY_H

#include <lua.h>

#define luau_set_const(state, name) { lua_pushnumber(state, name ); lua_setglobal(state, #name ); }
#define luau_set_const_table(state, name) { lua_pushstring(state, #name ); lua_pushnumber(state, name ); lua_settable( state, -3 ); }

#define luau_push_function_table(state, name, func) { lua_pushstring( state, name ); lua_pushcfunction( state, func ); lua_settable( state, -3 ); }
#define luau_push_type(state, type) { lua_pushstring( state, "type" ); lua_pushstring( state, type );  lua_settable( state, -3 ); }
#define luau_push_name(state, name) { lua_pushstring( state, "name" ); lua_pushstring( state, name );  lua_settable( state, -3 ); }


#endif
