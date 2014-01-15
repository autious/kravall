#ifndef SRC_CORE_WINDOWHANDLING_GLFWWINDOWCALLBACKHANDLER_H
#define SRC_CORE_WINDOWHANDLING_GLFWWINDOWCALLBACKHANDLER_H

#include <vector>

#include <WindowHandling/GLFWInclude.hpp>


namespace Core
{
    class GLFWWindowCallbackInterface
    {
        public:
            virtual void WindowResize( int width, int height ) = 0;
            virtual void FramebufferResize( int width, int height ) = 0;
    };

    class GLFWWindowCallbackHandler
    {
        public:
            static GLFWWindowCallbackHandler& Get();
            static void Free();
            void SetWindow( GLFWwindow * window );
            void FreeWindow( );
            void RegisterCallback( GLFWWindowCallbackInterface * inf );
            void DeregisterCallback( GLFWWindowCallbackInterface * inf );

            std::vector<GLFWWindowCallbackInterface*> m_callbacks;
            GLFWwindow * m_window = nullptr;
        private:
            static GLFWWindowCallbackHandler* instance;
            
    };
}
#endif
