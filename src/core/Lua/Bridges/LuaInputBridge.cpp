#include "LuaInputBridge.hpp"

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <Input/InputManager.hpp>
#include <Lua/LuaUtility.hpp>
#include <Lua/LuaMetatableTypes.hpp>

#include <logger/Logger.hpp>
#include <World.hpp>
#include <Lua/Bridges/LuaInputBridge.hpp>
#include <Lua/LuaMetatableTypes.hpp>

extern "C"
{
    static int LuaIsKeyDown(lua_State* L)
    {
        lua_pushboolean(L, Core::GetInputManager().GetKeyboardState().IsKeyDown(luaL_checkinteger(L, 1)));
        return 1;
    }

    static int LuaIsKeyUp(lua_State* L)
    {
        lua_pushboolean(L, Core::GetInputManager().GetKeyboardState().IsKeyUp(luaL_checkinteger(L, 1)));
        return 1;
    }

    static int LuaIsKeyDownOnce( lua_State* L )
    {
        lua_pushboolean(L, Core::GetInputManager().IsKeyPressedOnce(luaL_checkinteger(L, 1)));
        return 1;
    }

    static int LuaIsButtonDown(lua_State* L)
    {
        lua_pushboolean(L, Core::GetInputManager().GetMouseState().IsButtonDown(luaL_checkinteger(L, 1)));
        return 1;
    }

    static int LuaIsButtonUp(lua_State* L)
    {
        lua_pushboolean(L,Core::GetInputManager().GetMouseState().IsButtonUp(luaL_checkinteger(L, 1)));
        return 1;
    }

    static int LuaIsButtonDownOnce( lua_State *L )
    {
        lua_pushboolean( L, Core::GetInputManager().IsMouseButtonDownOnce(luaL_checkinteger(L, 1)));
        return 1;
    }

    static int LuaKeyboardNewindex( lua_State *L )
    {
        if( lua_gettop( L ) == 3 )
        {
            if( lua_isstring( L, 2 ) && ( lua_isfunction( L, 3 ) || lua_isnil( L, 3 ) )  )
            { 
                const char * index = lua_tostring( L, 2 );

                if( STREQ( index, "onchar" ) )
                {
                    luaL_unref( L, LUA_REGISTRYINDEX, Core::LuaInputBridge::regOnChar );
                    lua_pushvalue( L, 3 ); 
                    Core::LuaInputBridge::regOnChar = luaL_ref( L, LUA_REGISTRYINDEX );
                    return 0;
                } 
                else if( STREQ( index, "onkey" ) )
                {
                    luaL_unref( L, LUA_REGISTRYINDEX, Core::LuaInputBridge::regOnKey );
                    lua_pushvalue( L, 3 ); 
                    Core::LuaInputBridge::regOnKey = luaL_ref( L, LUA_REGISTRYINDEX );
                    return 0;
                }
            }
            
        }
        return luaL_error( L, "%s: Read Only/Invalid index/Invalid Value", __FUNCTION__ );
    }
    
    static int LuaMouseNewindex( lua_State *L )
    {
        if( lua_gettop( L ) == 3 )
        {
            if( lua_isstring( L, 2 ) && ( lua_isfunction( L, 3 ) || lua_isnil( L,3 ) )  )
            { 
                const char * index = lua_tostring( L, 2 );

                if( STREQ( index, "onbutton" ) )
                {
                    luaL_unref( L, LUA_REGISTRYINDEX, Core::LuaInputBridge::regOnButton );
                    lua_pushvalue( L, 3 ); 
                    Core::LuaInputBridge::regOnButton = luaL_ref( L, LUA_REGISTRYINDEX );
                    return 0;
                } 
                else if( STREQ( index, "onposition" ) )
                {
                    luaL_unref( L, LUA_REGISTRYINDEX, Core::LuaInputBridge::regOnPosition );
                    lua_pushvalue( L, 3 ); 
                    Core::LuaInputBridge::regOnPosition = luaL_ref( L, LUA_REGISTRYINDEX );
                    return 0;
                }
                else if( STREQ( index, "onscroll" ) )
                {
                    luaL_unref( L, LUA_REGISTRYINDEX, Core::LuaInputBridge::regOnScroll );
                    lua_pushvalue( L, 3 ); 
                    Core::LuaInputBridge::regOnScroll = luaL_ref( L, LUA_REGISTRYINDEX );
                    return 0;
                }
            }
            
        }
        return luaL_error( L, "%s: Read Only/Invalid index/Invalid Value", __FUNCTION__ );
    }

    static int LuaInputNewindex( lua_State *L )
    {
        return luaL_error( L, "Read only" );
    }
}


namespace Core
{
    static void PushKeyboardConst( lua_State * state );
    static void PushMouseConst( lua_State * state );
    static void SetBitmask( lua_State * state, int value, const char *name );


    LuaInputHandler::LuaInputHandler( lua_State * L )
    {
        m_luaState = L;
    }

    LuaInputHandler::~LuaInputHandler()
    {
        
    }
    

    void LuaInputHandler::OnKeyEvent(const Core::KeyEvent &e)
    {
        if( Core::LuaInputBridge::regOnKey != LUA_NOREF )
        {
            lua_rawgeti( m_luaState, LUA_REGISTRYINDEX, Core::LuaInputBridge::regOnKey );
            if( !lua_isnil( m_luaState, -1) )
            {
                if( lua_isfunction(m_luaState, -1) )
                {
                    lua_pushvalue( m_luaState, -1 );
                    lua_pushinteger(m_luaState, e.key);
                    lua_pushinteger(m_luaState, e.scancode);
                    lua_pushinteger(m_luaState, e.action);
                    uint64_t * modMask = LuaUNewBitmask( m_luaState ); 

                    *modMask = e.mods;

                    int error = lua_pcall(m_luaState, 4, 0, 0);
                    if(error)
                    {
                        LOG_ERROR << "Unable to call lua function onkeyevent: " << lua_tostring(m_luaState, -1) << std::endl;
                        lua_pop(m_luaState, 1);
                    }
                }
                else
                {
                    LOG_ERROR << "Unable to call lua function onkey: value is a non-function value." << std::endl;
                }
            }
            lua_pop( m_luaState, 1 );
        }
    }

    void LuaInputHandler::OnCharEvent( const Core::CharEvent &e ) 
    {
        if( Core::LuaInputBridge::regOnChar != LUA_NOREF && Core::LuaInputBridge::regOnChar != LUA_REFNIL )
        {
            lua_rawgeti( m_luaState, LUA_REGISTRYINDEX, Core::LuaInputBridge::regOnChar );
            if( lua_isfunction( m_luaState, -1 ) )
            {
                lua_pushvalue( m_luaState, -1 );
                lua_pushinteger( m_luaState, e.codepoint );

                int error = lua_pcall(m_luaState, 1, 0, 0);
                if(error)
                {
                    LOG_ERROR << "Unable to call lua function onkeyevent: " << lua_tostring(m_luaState, -1) << std::endl;
                    lua_pop(m_luaState, 1);
                }
            }
            else
            {
                LOG_ERROR << "Unable to call lua function onchar: value is nonfunctional value." << std::endl;
            }
            lua_pop( m_luaState, 1);
        }
    }

    void LuaInputHandler::OnButtonEvent( const Core::ButtonEvent &e ) 
    {
        if( Core::LuaInputBridge::regOnButton != LUA_NOREF && Core::LuaInputBridge::regOnButton != LUA_REFNIL )
        {
            lua_rawgeti( m_luaState, LUA_REGISTRYINDEX, Core::LuaInputBridge::regOnButton );
            if( lua_isfunction( m_luaState, -1 ) )
            {
                lua_pushvalue( m_luaState, -1 );
                lua_pushinteger( m_luaState, e.button );
                lua_pushinteger( m_luaState, e.action );
                lua_pushinteger( m_luaState, e.mods );

                int error = lua_pcall(m_luaState, 3, 0, 0);
                if(error)
                {
                    LOG_ERROR << "Unable to call lua function onbutton: " << lua_tostring(m_luaState, -1) << std::endl;
                    lua_pop(m_luaState, 1);
                }
            }
            else
            {
                LOG_ERROR << "Unable to call lua function onbutton: value is nonfunctional value." << std::endl;
            }
            lua_pop( m_luaState, 1);
        }
    }

    void LuaInputHandler::OnPositionEvent( const Core::PositionEvent &e ) 
    {
        if( Core::LuaInputBridge::regOnPosition != LUA_NOREF && Core::LuaInputBridge::regOnPosition != LUA_REFNIL )
        {
            lua_rawgeti( m_luaState, LUA_REGISTRYINDEX, Core::LuaInputBridge::regOnPosition );
            if( lua_isfunction( m_luaState, -1 ) )
            {
                lua_pushvalue( m_luaState, -1 );
                lua_pushinteger( m_luaState, e.xpos );
                lua_pushinteger( m_luaState, e.ypos );

                int error = lua_pcall(m_luaState, 2, 0, 0);
                if(error)
                {
                    LOG_ERROR << "Unable to call lua function onposition: " << lua_tostring(m_luaState, -1) << std::endl;
                    lua_pop(m_luaState, 1);
                }
            }
            else
            {
                LOG_ERROR << "Unable to call lua function onposition: value is nonfunctional value." << std::endl;
            }
            lua_pop( m_luaState, 1);
        }
    }

    void LuaInputHandler::OnScrollEvent( const Core::ScrollEvent &e ) 
    {
        if( Core::LuaInputBridge::regOnScroll != LUA_NOREF && Core::LuaInputBridge::regOnScroll != LUA_REFNIL )
        {
            lua_rawgeti( m_luaState, LUA_REGISTRYINDEX, Core::LuaInputBridge::regOnScroll );
            if( lua_isfunction( m_luaState, -1 ) )
            {
                lua_pushvalue( m_luaState, -1 );
                lua_pushinteger( m_luaState, e.xoffset );
                lua_pushinteger( m_luaState, e.yoffset );

                int error = lua_pcall(m_luaState, 2, 0, 0);
                if(error)
                {
                    LOG_ERROR << "Unable to call lua function onscroll: " << lua_tostring(m_luaState, -1) << std::endl;
                    lua_pop(m_luaState, 1);
                }
            }
            else
            {
                LOG_ERROR << "Unable to call lua function onscroll: value is nonfunctional value." << std::endl;
            }
            lua_pop( m_luaState, 1);
        }
    }

    LuaInputHandler* LuaInputBridge::inputHandler = nullptr;

    int LuaInputBridge::regOnKey        = LUA_NOREF;
    int LuaInputBridge::regOnChar       = LUA_NOREF;
    int LuaInputBridge::regOnButton     = LUA_NOREF;
    int LuaInputBridge::regOnPosition   = LUA_NOREF;
    int LuaInputBridge::regOnScroll     = LUA_NOREF;

    void LuaInputBridge::OpenLibs(lua_State* L)
    {
        int sanity = lua_gettop( L );
        lua_getglobal(L, "core");
            lua_pushstring( L, "input" );
            lua_newtable( L );
                luau_setconst( L, GLFW_PRESS, Press );
                luau_setconst( L, GLFW_RELEASE, Release );
                luau_setconst( L, GLFW_REPEAT, Repeat );

                lua_pushstring( L, "keyboard" );
                lua_newtable(L);
                    lua_pushcfunction(L, LuaIsKeyDown);
                    lua_setfield(L, -2, "iskeydown");
                    luau_setfunction(L, "iskeydownonce", LuaIsKeyDownOnce );
                    lua_pushcfunction(L, LuaIsKeyUp);
                    lua_setfield(L, -2, "iskeyup");
                   
                    lua_newtable( L );
                        PushKeyboardConst( L );
                    lua_setfield( L, -2, "key" );

                    lua_newtable( L );
                        SetBitmask( L, GLFW_MOD_SHIFT,      "Shift" );
                        SetBitmask( L, GLFW_MOD_CONTROL,    "Control" );
                        SetBitmask( L, GLFW_MOD_ALT,        "Alt" );
                        SetBitmask( L, GLFW_MOD_SUPER,      "Super"  );
                    lua_setfield( L, -2, "mod" );

                    lua_newtable( L );
                        luau_setfunction( L, "__newindex", LuaKeyboardNewindex );
                    lua_setmetatable( L, -2 );
                lua_settable( L, -3 );

                lua_pushstring( L, "mouse" );
                lua_newtable( L );
                    lua_pushcfunction(L, LuaIsButtonDown);
                    lua_setfield(L, -2, "isbuttondown");
                    lua_pushcfunction(L, LuaIsButtonUp);
                    lua_setfield(L, -2, "isbuttonup");
                    luau_setfunction( L, "isbuttondownonce", LuaIsButtonDownOnce );
                    
                    lua_newtable( L );
                        PushMouseConst( L );
                    lua_setfield( L, -2, "button" );

                    lua_newtable( L );
                        luau_setfunction( L, "__newindex", LuaMouseNewindex );
                    lua_setmetatable( L, -2 );
                lua_settable( L, -3 );

                lua_newtable( L );
                    luau_setfunction( L, "__newindex", LuaInputNewindex );
                lua_setmetatable( L, -2 );
            lua_settable( L, -3 );
        lua_pop( L, 1 );//Core
        assert( sanity == lua_gettop( L ) );

        inputHandler = new LuaInputHandler( L );    

        Core::GetInputManager().AddKeyEventListener( inputHandler );  
        Core::GetInputManager().AddCharEventListener( inputHandler );  
        Core::GetInputManager().AddButtonEventListener( inputHandler );  
        Core::GetInputManager().AddPositionEventListener( inputHandler );  
        Core::GetInputManager().AddScrollEventListener( inputHandler );  
    }

    void LuaInputBridge::CloseLibs( lua_State *L )
    {
        regOnKey        = LUA_NOREF;
        regOnChar       = LUA_NOREF;
        regOnButton     = LUA_NOREF;
        regOnPosition   = LUA_NOREF;
        regOnScroll     = LUA_NOREF;

        Core::GetInputManager().RemoveKeyEventListener( inputHandler );  
        Core::GetInputManager().RemoveCharEventListener( inputHandler );  
        Core::GetInputManager().RemoveButtonEventListener( inputHandler );  
        Core::GetInputManager().RemovePositionEventListener( inputHandler );  
        Core::GetInputManager().RemoveScrollEventListener( inputHandler );  

        delete inputHandler;
        inputHandler = nullptr;  
    }

    static void SetBitmask( lua_State * state, int value, const char *name )
    {
        uint64_t *bitmask = LuaUNewBitmask( state );
        *bitmask = value;
        lua_setfield( state, -2, name );
    }

    static void PushKeyboardConst( lua_State * state )
    {
        luau_setconst( state, GLFW_KEY_UNKNOWN           ,Unknown        ); 
        luau_setconst( state, GLFW_KEY_SPACE             ,Space          ); 
        luau_setconst( state, GLFW_KEY_APOSTROPHE        ,Apostrophe     ); 
        luau_setconst( state, GLFW_KEY_COMMA             ,Comma          ); 
        luau_setconst( state, GLFW_KEY_MINUS             ,Minus          ); 
        luau_setconst( state, GLFW_KEY_PERIOD            ,Period         ); 
        luau_setconst( state, GLFW_KEY_SLASH             ,Slash          ); 
        luau_setconst( state, GLFW_KEY_0                 ,0              ); 
        luau_setconst( state, GLFW_KEY_1                 ,1              ); 
        luau_setconst( state, GLFW_KEY_2                 ,2              ); 
        luau_setconst( state, GLFW_KEY_3                 ,3              ); 
        luau_setconst( state, GLFW_KEY_4                 ,4              ); 
        luau_setconst( state, GLFW_KEY_5                 ,5              ); 
        luau_setconst( state, GLFW_KEY_6                 ,6              ); 
        luau_setconst( state, GLFW_KEY_7                 ,7              ); 
        luau_setconst( state, GLFW_KEY_8                 ,8              ); 
        luau_setconst( state, GLFW_KEY_9                 ,9              ); 
        luau_setconst( state, GLFW_KEY_SEMICOLON         ,Semicolon      ); 
        luau_setconst( state, GLFW_KEY_EQUAL             ,Equal          ); 
        luau_setconst( state, GLFW_KEY_A                 ,A              ); 
        luau_setconst( state, GLFW_KEY_B                 ,B              ); 
        luau_setconst( state, GLFW_KEY_C                 ,C              ); 
        luau_setconst( state, GLFW_KEY_D                 ,D              ); 
        luau_setconst( state, GLFW_KEY_E                 ,E              ); 
        luau_setconst( state, GLFW_KEY_F                 ,F              ); 
        luau_setconst( state, GLFW_KEY_G                 ,G              ); 
        luau_setconst( state, GLFW_KEY_H                 ,H              ); 
        luau_setconst( state, GLFW_KEY_I                 ,I              ); 
        luau_setconst( state, GLFW_KEY_J                 ,J              ); 
        luau_setconst( state, GLFW_KEY_K                 ,K              ); 
        luau_setconst( state, GLFW_KEY_L                 ,L              ); 
        luau_setconst( state, GLFW_KEY_M                 ,M              ); 
        luau_setconst( state, GLFW_KEY_N                 ,N              ); 
        luau_setconst( state, GLFW_KEY_O                 ,O              ); 
        luau_setconst( state, GLFW_KEY_P                 ,P              ); 
        luau_setconst( state, GLFW_KEY_Q                 ,Q              ); 
        luau_setconst( state, GLFW_KEY_R                 ,R              ); 
        luau_setconst( state, GLFW_KEY_S                 ,S              ); 
        luau_setconst( state, GLFW_KEY_T                 ,T              ); 
        luau_setconst( state, GLFW_KEY_U                 ,U              ); 
        luau_setconst( state, GLFW_KEY_V                 ,V              ); 
        luau_setconst( state, GLFW_KEY_W                 ,W              ); 
        luau_setconst( state, GLFW_KEY_X                 ,X              ); 
        luau_setconst( state, GLFW_KEY_Y                 ,Y              ); 
        luau_setconst( state, GLFW_KEY_Z                 ,Z              ); 
        luau_setconst( state, GLFW_KEY_LEFT_BRACKET      ,Left_bracket   ); 
        luau_setconst( state, GLFW_KEY_BACKSLASH         ,Backslash      ); 
        luau_setconst( state, GLFW_KEY_RIGHT_BRACKET     ,Right_bracket  ); 
        luau_setconst( state, GLFW_KEY_GRAVE_ACCENT      ,Grave_accent   ); 
        luau_setconst( state, GLFW_KEY_WORLD_1           ,World_1        );  
        luau_setconst( state, GLFW_KEY_WORLD_2           ,World_2        );  

        luau_setconst( state, GLFW_KEY_ESCAPE            ,Escape         ); 
        luau_setconst( state, GLFW_KEY_ENTER             ,Enter          ); 
        luau_setconst( state, GLFW_KEY_TAB               ,Tab            ); 
        luau_setconst( state, GLFW_KEY_BACKSPACE         ,Backspace      ); 
        luau_setconst( state, GLFW_KEY_INSERT            ,Insert         ); 
        luau_setconst( state, GLFW_KEY_DELETE            ,Delete         ); 
        luau_setconst( state, GLFW_KEY_RIGHT             ,Right          ); 
        luau_setconst( state, GLFW_KEY_LEFT              ,Left           ); 
        luau_setconst( state, GLFW_KEY_DOWN              ,Down           ); 
        luau_setconst( state, GLFW_KEY_UP                ,Up             ); 
        luau_setconst( state, GLFW_KEY_PAGE_UP           ,Page_up        ); 
        luau_setconst( state, GLFW_KEY_PAGE_DOWN         ,Page_down      ); 
        luau_setconst( state, GLFW_KEY_HOME              ,Home           ); 
        luau_setconst( state, GLFW_KEY_END               ,End            ); 
        luau_setconst( state, GLFW_KEY_CAPS_LOCK         ,Caps_lock      ); 
        luau_setconst( state, GLFW_KEY_SCROLL_LOCK       ,Scroll_lock    ); 
        luau_setconst( state, GLFW_KEY_NUM_LOCK          ,Num_lock       ); 
        luau_setconst( state, GLFW_KEY_PRINT_SCREEN      ,Print_screen   ); 
        luau_setconst( state, GLFW_KEY_PAUSE             ,Pause          ); 
        luau_setconst( state, GLFW_KEY_F1                ,F1             ); 
        luau_setconst( state, GLFW_KEY_F2                ,F2             ); 
        luau_setconst( state, GLFW_KEY_F3                ,F3             ); 
        luau_setconst( state, GLFW_KEY_F4                ,F4             ); 
        luau_setconst( state, GLFW_KEY_F5                ,F5             ); 
        luau_setconst( state, GLFW_KEY_F6                ,F6             ); 
        luau_setconst( state, GLFW_KEY_F7                ,F7             ); 
        luau_setconst( state, GLFW_KEY_F8                ,F8             ); 
        luau_setconst( state, GLFW_KEY_F9                ,F9             ); 
        luau_setconst( state, GLFW_KEY_F10               ,F10            ); 
        luau_setconst( state, GLFW_KEY_F11               ,F11            ); 
        luau_setconst( state, GLFW_KEY_F12               ,F12            ); 
        luau_setconst( state, GLFW_KEY_F13               ,F13            ); 
        luau_setconst( state, GLFW_KEY_F14               ,F14            ); 
        luau_setconst( state, GLFW_KEY_F15               ,F15            ); 
        luau_setconst( state, GLFW_KEY_F16               ,F16            ); 
        luau_setconst( state, GLFW_KEY_F17               ,F17            ); 
        luau_setconst( state, GLFW_KEY_F18               ,F18            ); 
        luau_setconst( state, GLFW_KEY_F19               ,F19            ); 
        luau_setconst( state, GLFW_KEY_F20               ,F20            ); 
        luau_setconst( state, GLFW_KEY_F21               ,F21            ); 
        luau_setconst( state, GLFW_KEY_F22               ,F22            ); 
        luau_setconst( state, GLFW_KEY_F23               ,F23            ); 
        luau_setconst( state, GLFW_KEY_F24               ,F24            ); 
        luau_setconst( state, GLFW_KEY_F25               ,F25            ); 
        luau_setconst( state, GLFW_KEY_KP_0              ,Kp_0           ); 
        luau_setconst( state, GLFW_KEY_KP_1              ,Kp_1           ); 
        luau_setconst( state, GLFW_KEY_KP_2              ,Kp_2           ); 
        luau_setconst( state, GLFW_KEY_KP_3              ,Kp_3           ); 
        luau_setconst( state, GLFW_KEY_KP_4              ,Kp_4           ); 
        luau_setconst( state, GLFW_KEY_KP_5              ,Kp_5           ); 
        luau_setconst( state, GLFW_KEY_KP_6              ,Kp_6           ); 
        luau_setconst( state, GLFW_KEY_KP_7              ,Kp_7           ); 
        luau_setconst( state, GLFW_KEY_KP_8              ,Kp_8           ); 
        luau_setconst( state, GLFW_KEY_KP_9              ,Kp_9           ); 
        luau_setconst( state, GLFW_KEY_KP_DECIMAL        ,Kp_decimal     ); 
        luau_setconst( state, GLFW_KEY_KP_DIVIDE         ,Kp_divide      ); 
        luau_setconst( state, GLFW_KEY_KP_MULTIPLY       ,Kp_multiply    ); 
        luau_setconst( state, GLFW_KEY_KP_SUBTRACT       ,Kp_subtract    ); 
        luau_setconst( state, GLFW_KEY_KP_ADD            ,Kp_add         ); 
        luau_setconst( state, GLFW_KEY_KP_ENTER          ,Kp_enter       ); 
        luau_setconst( state, GLFW_KEY_KP_EQUAL          ,Kp_equal       ); 
        luau_setconst( state, GLFW_KEY_LEFT_SHIFT        ,Left_shift     ); 
        luau_setconst( state, GLFW_KEY_LEFT_CONTROL      ,Left_control   ); 
        luau_setconst( state, GLFW_KEY_LEFT_ALT          ,Left_alt       ); 
        luau_setconst( state, GLFW_KEY_LEFT_SUPER        ,Left_super     ); 
        luau_setconst( state, GLFW_KEY_RIGHT_SHIFT       ,Right_shift    ); 
        luau_setconst( state, GLFW_KEY_RIGHT_CONTROL     ,Right_control  ); 
        luau_setconst( state, GLFW_KEY_RIGHT_ALT         ,Right_alt      ); 
        luau_setconst( state, GLFW_KEY_RIGHT_SUPER       ,Right_super    ); 
        luau_setconst( state, GLFW_KEY_MENU              ,Menu           ); 
        luau_setconst( state, GLFW_KEY_LAST              ,Last           ); 
    }


    static void PushMouseConst( lua_State * state )
    {
        luau_setconst( state, GLFW_MOUSE_BUTTON_1        ,1     ); 
        luau_setconst( state, GLFW_MOUSE_BUTTON_2        ,2     ); 
        luau_setconst( state, GLFW_MOUSE_BUTTON_3        ,3     ); 
        luau_setconst( state, GLFW_MOUSE_BUTTON_4        ,4     ); 
        luau_setconst( state, GLFW_MOUSE_BUTTON_5        ,5     ); 
        luau_setconst( state, GLFW_MOUSE_BUTTON_6        ,6     ); 
        luau_setconst( state, GLFW_MOUSE_BUTTON_7        ,7     ); 
        luau_setconst( state, GLFW_MOUSE_BUTTON_8        ,8     ); 
        luau_setconst( state, GLFW_MOUSE_BUTTON_LAST     ,Last  );
        luau_setconst( state, GLFW_MOUSE_BUTTON_LEFT     ,Left  );
        luau_setconst( state, GLFW_MOUSE_BUTTON_RIGHT    ,Right );
        luau_setconst( state, GLFW_MOUSE_BUTTON_MIDDLE   ,Middle);
    }

    /*
    static void PushJoystickConst( lua_State * state )
    {
        luau_setconst( state, GLFW_JOYSTICK_1            ,1         ); 
        luau_setconst( state, GLFW_JOYSTICK_2            ,2         ); 
        luau_setconst( state, GLFW_JOYSTICK_3            ,3         ); 
        luau_setconst( state, GLFW_JOYSTICK_4            ,4         ); 
        luau_setconst( state, GLFW_JOYSTICK_5            ,5         ); 
        luau_setconst( state, GLFW_JOYSTICK_6            ,6         ); 
        luau_setconst( state, GLFW_JOYSTICK_7            ,7         ); 
        luau_setconst( state, GLFW_JOYSTICK_8            ,8         ); 
        luau_setconst( state, GLFW_JOYSTICK_9            ,9         ); 
        luau_setconst( state, GLFW_JOYSTICK_10           ,10        ); 
        luau_setconst( state, GLFW_JOYSTICK_11           ,11        ); 
        luau_setconst( state, GLFW_JOYSTICK_12           ,12        ); 
        luau_setconst( state, GLFW_JOYSTICK_13           ,13        ); 
        luau_setconst( state, GLFW_JOYSTICK_14           ,14        ); 
        luau_setconst( state, GLFW_JOYSTICK_15           ,15        ); 
        luau_setconst( state, GLFW_JOYSTICK_16           ,16        ); 
        luau_setconst( state, GLFW_JOYSTICK_LAST         ,Last      ); 
    }
    */
}


