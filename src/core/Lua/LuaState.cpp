#include "LuaState.hpp"

#include <sstream>

#include <logger/Logger.hpp>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>


#include <Lua/Datatypes/LuaBitmask.hpp>
#include <Lua/LuaUtility.hpp>

#include <Timer.hpp>

#include <Lua/Bridges/LuaLoggerPrint.hpp>
#include <Lua/Bridges/LuaEntityBridge.hpp>
#include <Lua/Bridges/LuaContentManagerBridge.hpp>
#include <Lua/Bridges/LuaInputBridge.hpp>
#include <Lua/Bridges/LuaGFXBridge.hpp>
#include <Lua/Bridges/LuaAttributeBridge.hpp>
#include <Lua/Bridges/LuaBoundingVolumeBridge.hpp>
#include <Lua/Bridges/LuaUnitTypeBridge.hpp>
#include <Lua/Bridges/LuaNavMeshBridge.hpp>
#include <Lua/Bridges/LuaGLMBridge.hpp>
#include <Lua/Bridges/LuaCameraBridge.hpp>
#include <Lua/Bridges/LuaWindowBridge.hpp>
#include <Lua/Bridges/LuaLinerarHeapBridge.hpp>
#include <Lua/Bridges/LuaConsoleBridge.hpp>

namespace Core
{
    struct LuaStateBindings
    {
        LuaStateBindings( lua_State *L ):
        lb(L),
        llp(L),
        leb(L),
        lcmb(L),
        lib(L),
        gfxb(L),
        lacb(L),
        lbvc(L),
        lutcb(L),
        lnmb(L),
        lglmb(L),
        lcab(L),
        lwb(L),
        lcob(L),
		llhb(L)
        {}
            
        LuaBitmask lb;
        LuaLoggerPrint llp;
        LuaEntityBridge leb;
        LuaContentManagerBridge lcmb;
        LuaInputBridge lib;
        LuaGFXBridge gfxb;
        LuaAttributeComponentBridge lacb;
        LuaBoundingVolumeComponentBridge lbvc;
        LuaUnitTypeComponentBridge lutcb;
        LuaNavMeshBridge lnmb;
        LuaGLMBridge lglmb;
        LuaCameraBridge lcab;
        LuaWindowBridge lwb;
        LuaConsoleBridge lcob;
		LuaLinearHeapBridge llhb;
    };
}

extern "C"
{
    static int LuaCoreNewindex( lua_State * L )
    {
        if( lua_isstring( L, 2 ) && (STREQ( lua_tostring( L, 2 ), "config" ) 
                || STREQ( lua_tostring( L, 2 ), "update" )
                || STREQ( lua_tostring( L, 2 ), "init" ) 
                || STREQ( lua_tostring( L, 2 ), "stop" ) ) )
        {
            lua_rawset( L, 1 );
            return 0;
        }
        return luaL_error( L, "Read only, %s", lua_tostring(L,2 ) );
    }
}


Core::LuaState::LuaState()
{
    m_state = luaL_newstate();
    m_activeUpdate = false;
}

void Core::LuaState::OpenLibs()
{

    int sanity = lua_gettop( m_state );
    luaL_openlibs( m_state ); 

    //Add extra paths for require commands.
    lua_getglobal( m_state, "package" );
    lua_getfield( m_state, -1, "path" );
    const char * old_path = lua_tostring( m_state, -1 );

    std::stringstream ss;

    ss << old_path << ";scripts/?.lua";

    std::string newPath = ss.str();

    lua_pop( m_state, 1); //Remove the path string
    
    lua_pushstring( m_state, newPath.c_str() );
    lua_setfield( m_state, -2, "path" );
    lua_pop(m_state, 1); //Pop the package table

    lua_newtable( m_state );
        lua_newtable( m_state );
        lua_setfield( m_state, -2, "config" );
    lua_setglobal( m_state, "core" );

    bindings = new LuaStateBindings( m_state );

    lua_getglobal( m_state, "core" ); //Let's lock down core now.
        lua_newtable( m_state );
        luau_setfunction( m_state, "__newindex", LuaCoreNewindex );
    lua_setmetatable( m_state, -2 ),
    lua_pop( m_state, 1 );

    assert( sanity == lua_gettop(m_state) );
}

Core::LuaState::~LuaState()
{
    delete bindings;
    lua_close( m_state );
}

bool Core::LuaState::Execute( const char * filename )
{
    int error = luaL_dofile( m_state, filename );

    if( error )
    {
        LOG_ERROR << "Unable to load file: " << lua_tostring( m_state, -1 ) << std::endl; 
        lua_pop( m_state, 1 ); 
    }

    VerifyUpdateFunction();

    return error == 0;
}

bool Core::LuaState::DoBlock( const char * block )
{
    int error = luaL_loadstring( m_state, block ) || lua_pcall( m_state, 0,0,0 );

    if( error )
    {
        if( lua_isstring( m_state, -1 ) )
        {
            LOG_ERROR << "Unable to parse block: " << lua_tostring( m_state, -1 ) << std::endl;
            lua_pop( m_state, 1 );
        }
        else
        {
            LOG_ERROR << "Unable to parse block: " << error << std::endl;
        }
    }

    VerifyUpdateFunction();

    return error == 0;
}

int Core::LuaState::DoBlock( const char * block, int args, int rargs )
{
    int error = luaL_loadstring( m_state, block ) || lua_pcall( m_state, args, rargs, 0 );

    if( error )
    {
        LOG_ERROR << "Unable to parse block: " << lua_tostring( m_state, -1 ) << std::endl;
        lua_pop( m_state, 1 );
        return 0;
    }

    return rargs;
}

bool Core::LuaState::Init( )
{
    lua_getglobal( m_state, "core" );
    lua_getfield( m_state, -1, "init" );

    int error = lua_pcall( m_state, 0,0,0 );

    if( error )
    {
        LOG_ERROR << "Unable to call init: " << lua_tostring( m_state, -1 ) << std::endl;
        lua_pop( m_state, 1 );
    }

    lua_pop( m_state, 1 );

    return error == 0;
}

bool Core::LuaState::Stop( )
{
    lua_getglobal( m_state, "core" );
    lua_getfield( m_state, -1, "stop" );

    int error = lua_pcall( m_state, 0,0,0 );

    if( error )
    {
        LOG_ERROR << "Unable to call stop: " << lua_tostring( m_state, -1 ) << std::endl;
        lua_pop( m_state, 1 );
    }

    lua_pop( m_state, 1 );

    return error == 0;
}

void Core::LuaState::Update( float delta )
{ 
    if( m_activeUpdate )
    {
        lua_rawgeti( m_state, LUA_REGISTRYINDEX, m_coreUpdateFunctionReg );
        lua_pushnumber( m_state, delta );
		this->
        m_timer.Start();   
        int error = lua_pcall( m_state, 1,0,0 );
        m_timer.Stop();
        m_lastFrameTime = m_timer.GetDelta();

        if( error )
        {
            LOG_ERROR << "Unable to call update: " << lua_tostring( m_state, -1 ) << std::endl;
            LOG_ERROR << "Disabling update call until new lua file is loaded..." << std::endl;
            m_activeUpdate = false;
            lua_pop( m_state, 1 );
        }
    } 
}

std::chrono::microseconds Core::LuaState::GetUpdateTiming()
{
    return m_lastFrameTime;
}

int Core::LuaState::GetMemoryUse()
{
    return lua_gc( m_state, LUA_GCCOUNT, 0 );
}

lua_State* Core::LuaState::GetState()
{
    return m_state;
}

void Core::LuaState::VerifyUpdateFunction()
{
    lua_getglobal( m_state, "core" );
    lua_getfield( m_state, -1, "update" );
    lua_rawgeti( m_state, LUA_REGISTRYINDEX, m_coreUpdateFunctionReg );

    if( lua_equal( m_state, -1, -2 ) == 0 && lua_isnil( m_state, -2 ) == 0 )
    {
        LOG_INFO << "Loaded new core.update function, (re)activating update call." << std::endl;
        m_activeUpdate = true;

        if( lua_isnil( m_state, -1 ) == 0 )
        { 
            luaL_unref( m_state, LUA_REGISTRYINDEX, m_coreUpdateFunctionReg );
        }

        lua_pushvalue( m_state, -2 );
        m_coreUpdateFunctionReg = luaL_ref( m_state, LUA_REGISTRYINDEX );
    }

    lua_pop( m_state, 3 );
}
