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
        if(lua_gettop(L) == 1)
        {
            if(lua_isnumber(L, 1))
            {
                bool result = Core::GetInputManager().GetKeyboardState().IsKeyDown(lua_tonumber(L, 1));
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
                bool result = Core::GetInputManager().GetKeyboardState().IsKeyUp(lua_tonumber(L, 1));
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
                bool result = Core::GetInputManager().GetMouseState().IsButtonDown(lua_tonumber(L, 1));
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
                bool result = Core::GetInputManager().GetMouseState().IsButtonUp(lua_tonumber(L, 1));
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
                    LOG_DEBUG << "Regestering new onkey function" << std::endl;
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
                luau_setconst( L, GLFW_PRESS, PRESS );
                luau_setconst( L, GLFW_RELEASE, RELEASE );
                luau_setconst( L, GLFW_REPEAT, REPEAT );

                lua_pushstring( L, "keyboard" );
                lua_newtable(L);
                    lua_pushcfunction(L, LuaIsKeyDown);
                    lua_setfield(L, -2, "iskeydown");
                    lua_pushcfunction(L, LuaIsKeyUp);
                    lua_setfield(L, -2, "iskeyup");
                    
                    PushKeyboardConst( L );

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

                    PushMouseConst( L );

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
        luau_setconst( state, GLFW_KEY_UNKNOWN           ,KEY_UNKNOWN        ); 
        luau_setconst( state, GLFW_KEY_SPACE             ,KEY_SPACE          ); 
        luau_setconst( state, GLFW_KEY_APOSTROPHE        ,KEY_APOSTROPHE     ); 
        luau_setconst( state, GLFW_KEY_COMMA             ,KEY_COMMA          ); 
        luau_setconst( state, GLFW_KEY_MINUS             ,KEY_MINUS          ); 
        luau_setconst( state, GLFW_KEY_PERIOD            ,KEY_PERIOD         ); 
        luau_setconst( state, GLFW_KEY_SLASH             ,KEY_SLASH          ); 
        luau_setconst( state, GLFW_KEY_0                 ,KEY_0              ); 
        luau_setconst( state, GLFW_KEY_1                 ,KEY_1              ); 
        luau_setconst( state, GLFW_KEY_2                 ,KEY_2              ); 
        luau_setconst( state, GLFW_KEY_3                 ,KEY_3              ); 
        luau_setconst( state, GLFW_KEY_4                 ,KEY_4              ); 
        luau_setconst( state, GLFW_KEY_5                 ,KEY_5              ); 
        luau_setconst( state, GLFW_KEY_6                 ,KEY_6              ); 
        luau_setconst( state, GLFW_KEY_7                 ,KEY_7              ); 
        luau_setconst( state, GLFW_KEY_8                 ,KEY_8              ); 
        luau_setconst( state, GLFW_KEY_9                 ,KEY_9              ); 
        luau_setconst( state, GLFW_KEY_SEMICOLON         ,KEY_SEMICOLON      ); 
        luau_setconst( state, GLFW_KEY_EQUAL             ,KEY_EQUAL          ); 
        luau_setconst( state, GLFW_KEY_A                 ,KEY_A              ); 
        luau_setconst( state, GLFW_KEY_B                 ,KEY_B              ); 
        luau_setconst( state, GLFW_KEY_C                 ,KEY_C              ); 
        luau_setconst( state, GLFW_KEY_D                 ,KEY_D              ); 
        luau_setconst( state, GLFW_KEY_E                 ,KEY_E              ); 
        luau_setconst( state, GLFW_KEY_F                 ,KEY_F              ); 
        luau_setconst( state, GLFW_KEY_G                 ,KEY_G              ); 
        luau_setconst( state, GLFW_KEY_H                 ,KEY_H              ); 
        luau_setconst( state, GLFW_KEY_I                 ,KEY_I              ); 
        luau_setconst( state, GLFW_KEY_J                 ,KEY_J              ); 
        luau_setconst( state, GLFW_KEY_K                 ,KEY_K              ); 
        luau_setconst( state, GLFW_KEY_L                 ,KEY_L              ); 
        luau_setconst( state, GLFW_KEY_M                 ,KEY_M              ); 
        luau_setconst( state, GLFW_KEY_N                 ,KEY_N              ); 
        luau_setconst( state, GLFW_KEY_O                 ,KEY_O              ); 
        luau_setconst( state, GLFW_KEY_P                 ,KEY_P              ); 
        luau_setconst( state, GLFW_KEY_Q                 ,KEY_Q              ); 
        luau_setconst( state, GLFW_KEY_R                 ,KEY_R              ); 
        luau_setconst( state, GLFW_KEY_S                 ,KEY_S              ); 
        luau_setconst( state, GLFW_KEY_T                 ,KEY_T              ); 
        luau_setconst( state, GLFW_KEY_U                 ,KEY_U              ); 
        luau_setconst( state, GLFW_KEY_V                 ,KEY_V              ); 
        luau_setconst( state, GLFW_KEY_W                 ,KEY_W              ); 
        luau_setconst( state, GLFW_KEY_X                 ,KEY_X              ); 
        luau_setconst( state, GLFW_KEY_Y                 ,KEY_Y              ); 
        luau_setconst( state, GLFW_KEY_Z                 ,KEY_Z              ); 
        luau_setconst( state, GLFW_KEY_LEFT_BRACKET      ,KEY_LEFT_BRACKET   ); 
        luau_setconst( state, GLFW_KEY_BACKSLASH         ,KEY_BACKSLASH      ); 
        luau_setconst( state, GLFW_KEY_RIGHT_BRACKET     ,KEY_RIGHT_BRACKET  ); 
        luau_setconst( state, GLFW_KEY_GRAVE_ACCENT      ,KEY_GRAVE_ACCENT   ); 
        luau_setconst( state, GLFW_KEY_WORLD_1           ,KEY_WORLD_1        );  
        luau_setconst( state, GLFW_KEY_WORLD_2           ,KEY_WORLD_2        );  

        luau_setconst( state, GLFW_KEY_ESCAPE            ,KEY_ESCAPE         ); 
        luau_setconst( state, GLFW_KEY_ENTER             ,KEY_ENTER          ); 
        luau_setconst( state, GLFW_KEY_TAB               ,KEY_TAB            ); 
        luau_setconst( state, GLFW_KEY_BACKSPACE         ,KEY_BACKSPACE      ); 
        luau_setconst( state, GLFW_KEY_INSERT            ,KEY_INSERT         ); 
        luau_setconst( state, GLFW_KEY_DELETE            ,KEY_DELETE         ); 
        luau_setconst( state, GLFW_KEY_RIGHT             ,KEY_RIGHT          ); 
        luau_setconst( state, GLFW_KEY_LEFT              ,KEY_LEFT           ); 
        luau_setconst( state, GLFW_KEY_DOWN              ,KEY_DOWN           ); 
        luau_setconst( state, GLFW_KEY_UP                ,KEY_UP             ); 
        luau_setconst( state, GLFW_KEY_PAGE_UP           ,KEY_PAGE_UP        ); 
        luau_setconst( state, GLFW_KEY_PAGE_DOWN         ,KEY_PAGE_DOWN      ); 
        luau_setconst( state, GLFW_KEY_HOME              ,KEY_HOME           ); 
        luau_setconst( state, GLFW_KEY_END               ,KEY_END            ); 
        luau_setconst( state, GLFW_KEY_CAPS_LOCK         ,KEY_CAPS_LOCK      ); 
        luau_setconst( state, GLFW_KEY_SCROLL_LOCK       ,KEY_SCROLL_LOCK    ); 
        luau_setconst( state, GLFW_KEY_NUM_LOCK          ,KEY_NUM_LOCK       ); 
        luau_setconst( state, GLFW_KEY_PRINT_SCREEN      ,KEY_PRINT_SCREEN   ); 
        luau_setconst( state, GLFW_KEY_PAUSE             ,KEY_PAUSE          ); 
        luau_setconst( state, GLFW_KEY_F1                ,KEY_F1             ); 
        luau_setconst( state, GLFW_KEY_F2                ,KEY_F2             ); 
        luau_setconst( state, GLFW_KEY_F3                ,KEY_F3             ); 
        luau_setconst( state, GLFW_KEY_F4                ,KEY_F4             ); 
        luau_setconst( state, GLFW_KEY_F5                ,KEY_F5             ); 
        luau_setconst( state, GLFW_KEY_F6                ,KEY_F6             ); 
        luau_setconst( state, GLFW_KEY_F7                ,KEY_F7             ); 
        luau_setconst( state, GLFW_KEY_F8                ,KEY_F8             ); 
        luau_setconst( state, GLFW_KEY_F9                ,KEY_F9             ); 
        luau_setconst( state, GLFW_KEY_F10               ,KEY_F10            ); 
        luau_setconst( state, GLFW_KEY_F11               ,KEY_F11            ); 
        luau_setconst( state, GLFW_KEY_F12               ,KEY_F12            ); 
        luau_setconst( state, GLFW_KEY_F13               ,KEY_F13            ); 
        luau_setconst( state, GLFW_KEY_F14               ,KEY_F14            ); 
        luau_setconst( state, GLFW_KEY_F15               ,KEY_F15            ); 
        luau_setconst( state, GLFW_KEY_F16               ,KEY_F16            ); 
        luau_setconst( state, GLFW_KEY_F17               ,KEY_F17            ); 
        luau_setconst( state, GLFW_KEY_F18               ,KEY_F18            ); 
        luau_setconst( state, GLFW_KEY_F19               ,KEY_F19            ); 
        luau_setconst( state, GLFW_KEY_F20               ,KEY_F20            ); 
        luau_setconst( state, GLFW_KEY_F21               ,KEY_F21            ); 
        luau_setconst( state, GLFW_KEY_F22               ,KEY_F22            ); 
        luau_setconst( state, GLFW_KEY_F23               ,KEY_F23            ); 
        luau_setconst( state, GLFW_KEY_F24               ,KEY_F24            ); 
        luau_setconst( state, GLFW_KEY_F25               ,KEY_F25            ); 
        luau_setconst( state, GLFW_KEY_KP_0              ,KEY_KP_0           ); 
        luau_setconst( state, GLFW_KEY_KP_1              ,KEY_KP_1           ); 
        luau_setconst( state, GLFW_KEY_KP_2              ,KEY_KP_2           ); 
        luau_setconst( state, GLFW_KEY_KP_3              ,KEY_KP_3           ); 
        luau_setconst( state, GLFW_KEY_KP_4              ,KEY_KP_4           ); 
        luau_setconst( state, GLFW_KEY_KP_5              ,KEY_KP_5           ); 
        luau_setconst( state, GLFW_KEY_KP_6              ,KEY_KP_6           ); 
        luau_setconst( state, GLFW_KEY_KP_7              ,KEY_KP_7           ); 
        luau_setconst( state, GLFW_KEY_KP_8              ,KEY_KP_8           ); 
        luau_setconst( state, GLFW_KEY_KP_9              ,KEY_KP_9           ); 
        luau_setconst( state, GLFW_KEY_KP_DECIMAL        ,KEY_KP_DECIMAL     ); 
        luau_setconst( state, GLFW_KEY_KP_DIVIDE         ,KEY_KP_DIVIDE      ); 
        luau_setconst( state, GLFW_KEY_KP_MULTIPLY       ,KEY_KP_MULTIPLY    ); 
        luau_setconst( state, GLFW_KEY_KP_SUBTRACT       ,KEY_KP_SUBTRACT    ); 
        luau_setconst( state, GLFW_KEY_KP_ADD            ,KEY_KP_ADD         ); 
        luau_setconst( state, GLFW_KEY_KP_ENTER          ,KEY_KP_ENTER       ); 
        luau_setconst( state, GLFW_KEY_KP_EQUAL          ,KEY_KP_EQUAL       ); 
        luau_setconst( state, GLFW_KEY_LEFT_SHIFT        ,KEY_LEFT_SHIFT     ); 
        luau_setconst( state, GLFW_KEY_LEFT_CONTROL      ,KEY_LEFT_CONTROL   ); 
        luau_setconst( state, GLFW_KEY_LEFT_ALT          ,KEY_LEFT_ALT       ); 
        luau_setconst( state, GLFW_KEY_LEFT_SUPER        ,KEY_LEFT_SUPER     ); 
        luau_setconst( state, GLFW_KEY_RIGHT_SHIFT       ,KEY_RIGHT_SHIFT    ); 
        luau_setconst( state, GLFW_KEY_RIGHT_CONTROL     ,KEY_RIGHT_CONTROL  ); 
        luau_setconst( state, GLFW_KEY_RIGHT_ALT         ,KEY_RIGHT_ALT      ); 
        luau_setconst( state, GLFW_KEY_RIGHT_SUPER       ,KEY_RIGHT_SUPER    ); 
        luau_setconst( state, GLFW_KEY_MENU              ,KEY_MENU           ); 
        luau_setconst( state, GLFW_KEY_LAST              ,KEY_LAST           ); 

        SetBitmask( state, GLFW_MOD_SHIFT,      "MOD_SHIFT" );
        SetBitmask( state, GLFW_MOD_CONTROL,    "MOD_CONTROL" );
        SetBitmask( state, GLFW_MOD_ALT,        "MOD_ALT" );
        SetBitmask( state, GLFW_MOD_SUPER,      "MOD_SUPER"  );
    }


    static void PushMouseConst( lua_State * state )
    {
        luau_setconst( state, GLFW_MOUSE_BUTTON_1        ,MOUSE_BUTTON_1     ); 
        luau_setconst( state, GLFW_MOUSE_BUTTON_2        ,MOUSE_BUTTON_2     ); 
        luau_setconst( state, GLFW_MOUSE_BUTTON_3        ,MOUSE_BUTTON_3     ); 
        luau_setconst( state, GLFW_MOUSE_BUTTON_4        ,MOUSE_BUTTON_4     ); 
        luau_setconst( state, GLFW_MOUSE_BUTTON_5        ,MOUSE_BUTTON_5     ); 
        luau_setconst( state, GLFW_MOUSE_BUTTON_6        ,MOUSE_BUTTON_6     ); 
        luau_setconst( state, GLFW_MOUSE_BUTTON_7        ,MOUSE_BUTTON_7     ); 
        luau_setconst( state, GLFW_MOUSE_BUTTON_8        ,MOUSE_BUTTON_8     ); 
        luau_setconst( state, GLFW_MOUSE_BUTTON_LAST     ,MOUSE_BUTTON_LAST  );
        luau_setconst( state, GLFW_MOUSE_BUTTON_LEFT     ,MOUSE_BUTTON_LEFT  );
        luau_setconst( state, GLFW_MOUSE_BUTTON_RIGHT    ,MOUSE_BUTTON_RIGHT );
        luau_setconst( state, GLFW_MOUSE_BUTTON_MIDDLE   ,MOUSE_BUTTON_MIDDLE);
    }

    /*
    static void PushJoystickConst( lua_State * state )
    {
        luau_setconst( state, GLFW_JOYSTICK_1            ,JOYSTICK_1         ); 
        luau_setconst( state, GLFW_JOYSTICK_2            ,JOYSTICK_2         ); 
        luau_setconst( state, GLFW_JOYSTICK_3            ,JOYSTICK_3         ); 
        luau_setconst( state, GLFW_JOYSTICK_4            ,JOYSTICK_4         ); 
        luau_setconst( state, GLFW_JOYSTICK_5            ,JOYSTICK_5         ); 
        luau_setconst( state, GLFW_JOYSTICK_6            ,JOYSTICK_6         ); 
        luau_setconst( state, GLFW_JOYSTICK_7            ,JOYSTICK_7         ); 
        luau_setconst( state, GLFW_JOYSTICK_8            ,JOYSTICK_8         ); 
        luau_setconst( state, GLFW_JOYSTICK_9            ,JOYSTICK_9         ); 
        luau_setconst( state, GLFW_JOYSTICK_10           ,JOYSTICK_10        ); 
        luau_setconst( state, GLFW_JOYSTICK_11           ,JOYSTICK_11        ); 
        luau_setconst( state, GLFW_JOYSTICK_12           ,JOYSTICK_12        ); 
        luau_setconst( state, GLFW_JOYSTICK_13           ,JOYSTICK_13        ); 
        luau_setconst( state, GLFW_JOYSTICK_14           ,JOYSTICK_14        ); 
        luau_setconst( state, GLFW_JOYSTICK_15           ,JOYSTICK_15        ); 
        luau_setconst( state, GLFW_JOYSTICK_16           ,JOYSTICK_16        ); 
        luau_setconst( state, GLFW_JOYSTICK_LAST         ,JOYSTICK_LAST      ); 
    }
    */
}


