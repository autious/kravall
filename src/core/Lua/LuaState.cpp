#include "LuaState.hpp"


#include <sstream>


Core::LuaState::LuaState()
{
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
}

Core::LuaState::~LuaState()
{
    lua_close( m_state );
}

#include <logger/Logger.hpp>

void Core::LuaState::Execute( const char * filename )
{
    int error = luaL_dofile( m_state, filename );

    if( error )
    {
       LOG::ERROR << "Unable to load file." << std::endl; 
    }
}


