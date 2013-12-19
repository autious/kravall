#ifndef CORE_LUA_INPUT_HANDLER_HPP
#define CORE_LUA_INPUT_HANDLER_HPP

#include <Input/Interfaces/KeyEventListener.hpp>

namespace Core
{
    class InputHandler : public Core::KeyEventListener
    {
    public:
        explicit InputHandler();

        void OnKeyEvent(const Core::KeyEvent &e) override;

    private:
        void CheckFunctionSanity();
        bool m_canCallOnPress;
        bool m_canCallOnRelease;
        lua_State* m_luaState;
    };
}


#endif
