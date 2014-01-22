#include "LuaWindowBridge.hpp"

#include <logger/Logger.hpp>

#include <WindowHandling/GLFWInclude.hpp>
#include <Lua/LuaUtility.hpp>


extern "C" 
{
    static int LuaGetWindowSize( lua_State * L )
    {
        int width, height; 
        glfwGetWindowSize( mainWindow, &width, &height );

        lua_pushinteger( L, width );
        lua_pushinteger( L, height );

        return 2;
    }
}

namespace Core
{

    LuaWindowBridge::LuaWindowBridge( lua_State *L )
    {
        m_state = L;
        lua_getglobal( L, "core" );
            lua_newtable( L );
                luau_setfunction( L, "getSize", LuaGetWindowSize );
            lua_setfield( L, -2, "window" );
        lua_pop( L, 1 );

        Core::GLFWWindowCallbackHandler::Get().RegisterCallback( this );
    }

    LuaWindowBridge::~LuaWindowBridge()
    {
        m_state = nullptr;
        Core::GLFWWindowCallbackHandler::Get().DeregisterCallback( this );
    }

    void LuaWindowBridge::WindowResize( int width, int height )
    {
        lua_getglobal( m_state, "core" );
        lua_getfield( m_state, -1, "window" );
        lua_getfield( m_state, -1, "windowSizeCallback" );

        if( lua_isfunction( m_state, -1 ) )
        {
            lua_pushinteger( m_state, width );
            lua_pushinteger( m_state, height );

            int error = lua_pcall( m_state, 2, 0, 0);

            if( error )
            {
                LOG_ERROR << "Error when using callback core.window.windowSizeCallback:" << lua_tostring( m_state, -1 ) << std::endl;
                lua_pop( m_state, 1 );
            }
        }
        else
        {
            lua_pop( m_state,1 );
        }

        lua_pop( m_state, 2 );
    }

    void LuaWindowBridge::FramebufferResize( int width, int height )
    {
        lua_getglobal( m_state, "core" );
        lua_getfield( m_state, -1, "window" );
        lua_getfield( m_state, -1, "framebufferSizeCallback" );

        if( lua_isfunction( m_state, -1 ) )
        {
            lua_pushinteger( m_state, width );
            lua_pushinteger( m_state, height );

            int error = lua_pcall( m_state, 2, 0, 0);

            if( error )
            {
                LOG_ERROR << "Error when using callback core.window.framebufferSizeCallback:" << lua_tostring( m_state, -1 ) << std::endl;
                lua_pop( m_state, 1 );
            }
        }
        else
        {
            lua_pop( m_state,1 );
        }

        lua_pop( m_state, 2 );

    }

    void LuaWindowBridge::WindowFocus( int focus )
    {
        lua_getglobal( m_state, "core" );
        lua_getfield( m_state, -1, "window" );
        lua_getfield( m_state, -1, "windowFocusCallback" );

        if( lua_isfunction( m_state, -1 ) )
        {
            lua_pushinteger( m_state, focus );

            int error = lua_pcall( m_state, 1, 0, 0);

            if( error )
            {
                LOG_ERROR << "Error when using callback core.window.framebufferSizeCallback:" << lua_tostring( m_state, -1 ) << std::endl;
                lua_pop( m_state, 1 );
            }
        }
        else
        {
            lua_pop( m_state,1 );
        }

        lua_pop( m_state, 2 );
    }
}
