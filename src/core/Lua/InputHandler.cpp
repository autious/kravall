#include "InputHandler.hpp"
#include <logger/Logger.hpp>
#include <World.hpp>
#include <lua.h>

namespace Core
{
    InputHandler::InputHandler()
    {
       m_luaState = Core::world.m_luaState; 
       m_canCallOnPress = false;
       m_canCallOnRelease = false;
    }

    void InputHandler::OnKeyEvent(const Core::KeyEvent &e)
    {
        lua_getglobal(m_luaState, "core");
        lua_getfield(m_luaState, -1, "keyboard");

        CheckFunctionSanity();

        if(e.action == GLFW_RELEASE)
        {
            if(m_canCallRelease)
            {
                lua_getfield(m_luaState, -1, "onrelease");
                if(!lua_isnil(m_luaState, -1) && lua_isfunction(m_luaState, -1))
                {
                    lua_pushinteger(m_luaState, e.key);
                    lua_pushboolean(m_luaState, static_cast<bool>(e.mods & GLFW_MOD_SHIFT));  
                    lua_pushboolean(m_luaState, static_cast<bool>(e.mods & GLFW_MOD_CONTROL));  
                    lua_pushboolean(m_luaState, static_cast<bool>(e.mods & GLFW_MOD_ALT));  
                    lua_pushboolean(m_luaState, static_cast<bool>(e.mods & GLFW_MOD_SUPER));  

                    int error = lua_pcall(m_luaState, 5, 0, 0);
                    if(error)
                    {
                        LOG_ERROR << "Unable to call lua function onrelease: " << lua_tostring(m_luaState, -1) << std::endl;
                        m_canCallOnRelease = false;
                        lua_pop(1);
                    }
                }
                else
                {
                    LOG_ERROR << "Unable to call lua function onrelease: value is nil or not function." << std::endl;
                    m_canCallOnRelease = false;
                }
                lua_pop(1);
            }
        }
        else if(e.action == GLFW_REPEAT || e.action == GFLW_PRESS)
        {
            if(m_canCallOnPress)
            {
                lua_getfield(m_luaState, -1, "onpress");
                if(!lua_isnil(m_luaState, -1) && lua_isfunction(m_luaState, -1))
                {
                    lua_pushinteger(m_luaState, e.key);
                    lua_pushboolean(m_luaState, static_cast<bool>(e.mods & GLFW_MOD_SHIFT));  
                    lua_pushboolean(m_luaState, static_cast<bool>(e.mods & GLFW_MOD_CONTROL));  
                    lua_pushboolean(m_luaState, static_cast<bool>(e.mods & GLFW_MOD_ALT));  
                    lua_pushboolean(m_luaState, static_cast<bool>(e.mods & GLFW_MOD_SUPER));  

                    int error = lua_pcall(m_luaState, 5, 0, 0);
                    if(error)
                    {
                        LOG_ERROR << "Unable to call lua function onpress: " << lua_tostring(m_luaState, -1) << std::endl;
                        m_canCallOnPress = false;
                        lua_pop(1);
                    }
                }
                else
                {
                    LOG_ERROR << "Unable to call lua function onpress: value is nil or not function." << std::endl;
                    m_canCallOnPress = false;
                }
                lua_pop(1);
            }
        }
        
    }

    void InputHandler::CheckFunctionSanity()
    {
        lua_getfield(m_luaState, -1, "onrelease");
        if(!lua_isnil(m_luaState, -1) && lua_isfunction(m_luaState, -1))
        {
            m_canCallOnRelease = true;
        }
        lua_pop(1);

        lua_getfield(m_luaState, -1, "onpress");
        if(!lua_isnil(m_luaState, -1) && lua_isfunction(m_luaState, -1))
        {
            m_canCallOnPress = true;
        }
    }
}
