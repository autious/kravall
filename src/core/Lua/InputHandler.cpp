#include "InputHandler.hpp"

#include <logger/Logger.hpp>
#include <World.hpp>

#define RELEASE 0
#define PRESS 1
#define REPEAT 2

#define MOD_SHIFT 1
#define MOD_CONTROL 2
#define MOD_ALT 4
#define MOD_SUPER 8

namespace Core
{
    InputHandler::InputHandler()
    {
       m_luaState = Core::world.m_luaState.GetState(); 
       m_canCallOnPress = false;
       m_canCallOnRelease = false;
    }

    void InputHandler::OnKeyEvent(const Core::KeyEvent &e)
    {
        lua_getglobal(m_luaState, "core");
        lua_getfield(m_luaState, -1, "keyboard");

        CheckFunctionSanity();

        if(e.action == RELEASE)
        {
            if(m_canCallOnRelease)
            {
                lua_getfield(m_luaState, -1, "onrelease");
                if(!lua_isnil(m_luaState, -1) && lua_isfunction(m_luaState, -1))
                {
                    lua_pushinteger(m_luaState, e.key);
                    lua_pushboolean(m_luaState, static_cast<bool>(e.mods & MOD_SHIFT));  
                    lua_pushboolean(m_luaState, static_cast<bool>(e.mods & MOD_CONTROL));  
                    lua_pushboolean(m_luaState, static_cast<bool>(e.mods & MOD_ALT));  
                    lua_pushboolean(m_luaState, static_cast<bool>(e.mods & MOD_SUPER));  

                    int error = lua_pcall(m_luaState, 5, 0, 0);
                    if(error)
                    {
                        LOG_ERROR << "Unable to call lua function onrelease: " << lua_tostring(m_luaState, -1) << std::endl;
                        m_canCallOnRelease = false;
                        lua_pop(m_luaState, 1);
                    }
                }
                else
                {
                    LOG_ERROR << "Unable to call lua function onrelease: value is nil or not function." << std::endl;
                    m_canCallOnRelease = false;
                }
                lua_pop(m_luaState, 1);
            }
        }
        else if(e.action == REPEAT || e.action == PRESS)
        {
            if(m_canCallOnPress)
            {
                lua_getfield(m_luaState, -1, "onpress");
                if(!lua_isnil(m_luaState, -1) && lua_isfunction(m_luaState, -1))
                {
                    lua_pushinteger(m_luaState, e.key);
                    lua_pushboolean(m_luaState, static_cast<bool>(e.mods & MOD_SHIFT));  
                    lua_pushboolean(m_luaState, static_cast<bool>(e.mods & MOD_CONTROL));  
                    lua_pushboolean(m_luaState, static_cast<bool>(e.mods & MOD_ALT));  
                    lua_pushboolean(m_luaState, static_cast<bool>(e.mods & MOD_SUPER));  

                    int error = lua_pcall(m_luaState, 5, 0, 0);
                    if(error)
                    {
                        LOG_ERROR << "Unable to call lua function onpress: " << lua_tostring(m_luaState, -1) << std::endl;
                        m_canCallOnPress = false;
                        lua_pop(m_luaState, 1);
                    }
                }
                else
                {
                    LOG_ERROR << "Unable to call lua function onpress: value is nil or not function." << std::endl;
                    m_canCallOnPress = false;
                }
                lua_pop(m_luaState, 1);
            }
        }
        lua_pop(m_luaState, 2);        
    }

    void InputHandler::CheckFunctionSanity()
    {
        lua_getfield(m_luaState, -1, "onrelease");
        if(!lua_isnil(m_luaState, -1) && lua_isfunction(m_luaState, -1))
        {
            m_canCallOnRelease = true;
        }
        lua_pop(m_luaState, 1);

        lua_getfield(m_luaState, -1, "onpress");
        if(!lua_isnil(m_luaState, -1) && lua_isfunction(m_luaState, -1))
        {
            m_canCallOnPress = true;
        }
        lua_pop(m_luaState, 1);
    }
}
