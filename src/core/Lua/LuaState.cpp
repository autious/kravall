#include "LuaState.hpp"

#include <sstream>

#include <logger/Logger.hpp>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <Lua/Bridges/LuaLoggerPrint.hpp>
#include <Lua/Bridges/LuaEntityBridge.hpp>
#include <Lua/Bridges/LuaContentManagerBridge.hpp>
#include <Lua/Bridges/LuaGFXBridge.hpp>
#include <Lua/Bridges/LuaAttributeBridge.hpp>
#include <Lua/Bridges/LuaBoundingVolumeBridge.hpp>
#include <Lua/Bridges/LuaUnitTypeBridge.hpp>


#include <Timer.hpp>

Core::LuaState::LuaState()
{
    LOG_DEBUG << "Creating lua state" << std::endl;
    m_state = luaL_newstate();
    m_activeUpdate = false;

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
    lua_setglobal( m_state, "core" );

    LuaLoggerPrint::OpenLibs( m_state );
    LuaEntityBridge::OpenLibs( m_state );
    LuaContentManagerBridge::OpenLibs( m_state );
    LuaGFXBridge::OpenLibs( m_state );
	LuaAttributeComponentBridge::OpenLibs( m_state );
	LuaBoundingVolumeComponentBridge::OpenLibs( m_state );
	LuaUnitTypeComponentBridge::OpenLibs( m_state );
}

Core::LuaState::~LuaState()
{
    lua_close( m_state );
}

void Core::LuaState::Execute( const char * filename )
{
    int error = luaL_dofile( m_state, filename );

    if( error )
    {
        LOG_ERROR << "Unable to load file: " << lua_tostring( m_state, -1 ) << std::endl; 
        lua_pop( m_state, 1 ); 
    }

    VerifyUpdateFunction();
}

void Core::LuaState::DoBlock( const char * block )
{
    int error = luaL_loadstring( m_state, block ) || lua_pcall( m_state, 0,0,0 );

    if( error )
    {
        LOG_ERROR << "Unable to parse block: " << lua_tostring( m_state, -1 ) << std::endl;
        lua_pop( m_state, 1 );
    }

    VerifyUpdateFunction();
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

void Core::LuaState::Update( float delta )
{ 
    if( m_activeUpdate )
    {
        lua_rawgeti( m_state, LUA_REGISTRYINDEX, m_coreUpdateFunctionReg );
        lua_pushnumber( m_state, delta );
        Timer().Start();   
        int error = lua_pcall( m_state, 1,0,0 );
        Timer().Stop();
        m_lastFrameTime = Timer().GetDelta();

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
