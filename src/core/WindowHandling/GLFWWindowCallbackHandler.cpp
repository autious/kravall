#include "GLFWWindowCallbackHandler.hpp"
#include <gfx/GFXInterface.hpp>
#include <logger/Logger.hpp>

#include <vector>
#include <algorithm>

extern "C"
{
	static void FramebufferResizeCallback(GLFWwindow* window, int width, int height)
	{

        for( unsigned int i = 0; i < Core::GLFWWindowCallbackHandler::Get().m_callbacks.size(); i++ )
        {
            Core::GLFWWindowCallbackHandler::Get().m_callbacks[i]->FramebufferResize( width, height );
        }
	}

    static void WindowResizeCallback( GLFWwindow * window, int width, int height )
    {
        for( unsigned int i = 0; i < Core::GLFWWindowCallbackHandler::Get().m_callbacks.size(); i++ )
        {
            Core::GLFWWindowCallbackHandler::Get().m_callbacks[i]->WindowResize( width, height );
        }
    }
}

namespace Core
{
    GLFWWindowCallbackHandler* GLFWWindowCallbackHandler::instance = nullptr;
    
    GLFWWindowCallbackHandler& GLFWWindowCallbackHandler::Get()
    {
        if( instance == nullptr )
        {
            instance = new GLFWWindowCallbackHandler();
        } 

        return *instance;

    }

    void GLFWWindowCallbackHandler::Free()
    {

        if( instance != nullptr )
        {
            instance->FreeWindow();
            delete instance;
        }

        instance = nullptr;
    }

    void GLFWWindowCallbackHandler::SetWindow( GLFWwindow * window )
    {
        m_window = window;
        glfwSetFramebufferSizeCallback(window, FramebufferResizeCallback);
        glfwSetWindowSizeCallback(window, WindowResizeCallback);
    }
     
    void GLFWWindowCallbackHandler::FreeWindow( )
    {
        if( m_window != nullptr )
        {
            glfwSetFramebufferSizeCallback(m_window, nullptr);
            glfwSetWindowSizeCallback(m_window, nullptr);
        }

        m_window = nullptr;
    }

    void GLFWWindowCallbackHandler::RegisterCallback( GLFWWindowCallbackInterface * inf )
    {
        m_callbacks.push_back( inf );
    }

    void GLFWWindowCallbackHandler::DeregisterCallback( GLFWWindowCallbackInterface * inf )
    {
        std::vector<GLFWWindowCallbackInterface*>::iterator it 
           = std::find( m_callbacks.begin(), m_callbacks.end(), inf );

        if( it != m_callbacks.end() )
            m_callbacks.erase( it );
    }
}
