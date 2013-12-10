#include "LuaState.hpp"

#include <sstream>

#include <logger/Logger.hpp>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <Lua/Bridges/LuaLoggerPrint.hpp>

Core::LuaState::LuaState()
{
    LOG_DEBUG << "Creating lua state" << std::endl;
    m_state = luaL_newstate();

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

    LuaLoggerPrint::OpenLibs( m_state );
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
}

void Core::LuaState::DoBlock( const char * block )
{
    int error = luaL_loadstring( m_state, block ) || lua_pcall( m_state, 0,0,0 );

    if( error )
    {
        LOG_ERROR << "Unable to parse block: " << lua_tostring( m_state, -1 ) << std::endl;
        lua_pop( m_state, 1 );
    }
}
