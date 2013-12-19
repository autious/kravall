#include "LuaInputBridge.hpp"
#include <Input/InputManager.hpp>


extern "C"
{
    static int LuaIsKeyDown(lua_State* L)
    {
        if(lua_gettop(L) == 1)
        {
            if(lua_isnumber(L, 1))
            {
                bool result = GetInputManager().GetKeyboardState().IsKeyDown(lua_tonumber(L, 1));
                lua_pushboolean(L, result);
            }
            else
            {
                return luaL_error(L, "The function takes numerical argument");
            }
        }
        else
        {
            return luaL_error(L, "The function takes one argument");
        }

        return 1;
    }

    static int LuaIsKeyUp(lua_State* L)
    {
        if(lua_gettop(L) == 1)
        {
            if(lua_isnumber(L, 1))
            {
                bool result = GetInputManager().GetKeyboardState().IsKeyUp(lua_tonumber(L, 1));
                lua_pushboolean(L, result);
            }
            else
            {
                return luaL_error(L, "The function takes numerical argument");
            }
        }
        else
        {
            return luaL_error(L, "The function takes one argument");
        }

        return 1;
    }

    static int LuaIsButtonDown(lua_State* L)
    {
        if(lua_gettop(L) == 1)
        {
            if(lua_isnumber(L, 1))
            {
                bool result = GetInputManager().GetMouseState().IsButtonDown(lua_tonumber(L, 1));
                lua_pushboolean(L, result);
            }
            else
            {
                return luaL_error(L, "The function takes numerical argument");
            }
        }
        else
        {
            return luaL_error(L, "The function takes one argument");
        }

        return 1;
    }

    static int LuaIsButtonUp(lua_State* L)
    {
        if(lua_gettop(L) == 1)
        {
            if(lua_isnumber(L, 1))
            {
                bool result = GetInputManager().GetMouseState().IsButtonUp(lua_tonumber(L, 1));
                lua_pushboolean(L, result);
            }
            else
            {
                return luaL_error(L, "The function takes numerical argument");
            }
        }
        else
        {
            return luaL_error(L, "The function takes one argument");
        }

        return 1;
    }
}


namespace Core
{
    LuaInputBridge::OpenLibs(lua_State* state)
    {
        lua_getglobal(state, "core");

        lua_newtable(state);
        lua_setfield(state, -2, "keyboard");
        lua_getfield(state, -1, "keyboard");
        lua_pushcfunction(state, LuaIsKeyDown);
        lua_setfield(state, -2, "iskeydown");
        lua_pushcfunction(state, LuaIsKeyUp);
        lua_setfield(state, -2, "iskeyup");
        lua_pop(state, 1);        

        lua_newtable(state);
        lua_setfield(state, -2, "mouse");
        lua_getfield(state, -1, "mouse");
        lua_pushcfunction(state, LuaIsButtonDown);
        lua_setfield(state, -2, "isbuttondown");
        lua_pushcfunction(state, LuaIsButtonUp);
        lua_setfield(statem -2, "isbuttonup");
        lua_pop(state, 2);
    }
    
}
