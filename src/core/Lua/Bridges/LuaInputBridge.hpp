#ifndef CORE_LUA_BRIDGES_LUA_INPUT_BRIDGE_HPP
#define CORE_LUA_BRIDGES_LUA_INPUT_BRIDGE_HPP

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <Input/Interfaces/KeyEventListener.hpp>
#include <Input/Interfaces/CharEventListener.hpp>
#include <Input/Interfaces/ButtonEventListener.hpp>
#include <Input/Interfaces/PositionEventListener.hpp>
#include <Input/Interfaces/ScrollEventListener.hpp>

namespace Core
{

    class LuaInputHandler : public Core::KeyEventListener, public Core::CharEventListener, public Core::ButtonEventListener, public Core::PositionEventListener, public Core::ScrollEventListener 
    {
    public:
        LuaInputHandler( lua_State * L );
        virtual ~LuaInputHandler();

        void OnKeyEvent(const Core::KeyEvent &e) override;
        void OnCharEvent( const Core::CharEvent &e ) override;
        void OnButtonEvent( const Core::ButtonEvent &e ) override;
        void OnPositionEvent( const Core::PositionEvent &e ) override;
        void OnScrollEvent( const Core::ScrollEvent &e ) override;

    private:
        lua_State* m_luaState;
    };

    class LuaInputBridge
    {
    public:
        LuaInputBridge( lua_State * state );
        ~LuaInputBridge();
    private:

        LuaInputHandler* inputHandler;
        lua_State * state;
        

        
    };
}


#endif
