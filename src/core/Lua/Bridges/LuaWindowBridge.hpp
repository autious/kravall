#ifndef SRC_CORE_LUA_BRIDGES_LUAWINDOWBRIDGE_H
#define SRC_CORE_LUA_BRIDGES_LUAWINDOWBRIDGE_H

#include <lua.h>

#include <WindowHandling/GLFWWindowCallbackHandler.hpp>

namespace Core
{
    class LuaWindowBridge : public GLFWWindowCallbackInterface
    {
    public:
        LuaWindowBridge( lua_State * L );
        ~LuaWindowBridge();

        virtual void WindowResize( int width, int height ) override;
        virtual void FramebufferResize( int width, int height ) override;
        virtual void WindowFocus( int focus ) override;


    private:
        lua_State *m_state;
    };
}

#endif
