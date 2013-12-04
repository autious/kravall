#include "ConfigurationHandler.hpp"

#include <logger/Logger.hpp>
#include <string>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <Lua/LuaState.hpp>
#include <logger/Logger.hpp>


Core::ConfigurationHandler::ConfigurationHandler( LuaState* state )
{
    m_luaState = state;
}

std::string Core::ConfigurationHandler::GetString( const char *name, const char *fallback )
{
    std::string str;
    PushConfigTable();

    lua_getfield( m_luaState->m_state, -1, name );  

    if( lua_isstring( m_luaState->m_state, -1 ) )
    {
        str = std::string( lua_tostring( m_luaState->m_state, -1 ) );
    }
    else
    {
        if( false == lua_isnil( m_luaState->m_state, -1 ) )
        {
            LOG_ERROR << "Unable to get config value as string: " << name << std::endl;
        }

        str = std::string(fallback);
    }

    lua_pop( m_luaState->m_state, 1 );

    PopConfigTable(); 

    return str;
}

double Core::ConfigurationHandler::GetDouble( const char *name, double fallback )
{
    double val;
    PushConfigTable();

    lua_getfield( m_luaState->m_state, -1, name );  

    if( lua_isnumber( m_luaState->m_state, -1 ) )
    {
        val = lua_tonumber( m_luaState->m_state, -1 );
    }
    else
    {
        if( false == lua_isnil( m_luaState->m_state, -1 ) )
        {
            LOG_ERROR << "Unable to get config value as string: " << name << std::endl;
        }

        val = fallback;
    }

    lua_pop( m_luaState->m_state, 1 );

    PopConfigTable(); 

    return val;
}

int Core::ConfigurationHandler::GetInt( const char *name, int fallback )
{
    int val;
    PushConfigTable();

    lua_getfield( m_luaState->m_state,-1, name );  

    if( lua_isnumber( m_luaState->m_state, -1 ) )
    {
        val = lua_tointeger( m_luaState->m_state, -1 );
    }
    else
    {
        if( false == lua_isnil( m_luaState->m_state, -1 ) )
        {
            LOG_ERROR << "Unable to get config value as string: " << name << std::endl;
        }

        val = fallback;
    }

    lua_pop( m_luaState->m_state, 1 );

    PopConfigTable(); 

    return val;
}

bool Core::ConfigurationHandler::GetBool( const char *name, bool fallback )
{
    bool val;
    PushConfigTable();

    lua_getfield( m_luaState->m_state,-1, name );  

    if( lua_isboolean( m_luaState->m_state, -1 ) )
    {
        val = lua_toboolean( m_luaState->m_state, -1 );
    }
    else
    {
        if( false == lua_isnil( m_luaState->m_state, -1 ) )
        {
            LOG_ERROR << "Unable to get config value as string: " << name << std::endl;
        }

        val = fallback;
    }

    lua_pop( m_luaState->m_state, 1 );

    PopConfigTable(); 

    return val;
}

glm::vec2 Core::ConfigurationHandler::GetVec2( const char *name, glm::vec2 fallback )
{
    glm::vec2 val;
    PushConfigTable();

    lua_getfield( m_luaState->m_state, -1, name );  

    if( lua_istable( m_luaState->m_state, -1 ) )
    {
        GetVecX( name, &val[0], 2 );
    }
    else
    {
        if( false == lua_isnil( m_luaState->m_state, -1 ) )
        {
            LOG_ERROR << "Unable to get config value as string: " << name << std::endl;
        }

        val = fallback;
    }

    lua_pop( m_luaState->m_state, 1 );

    PopConfigTable(); 

    return val;
}

glm::vec3 Core::ConfigurationHandler::GetVec3( const char *name, glm::vec3 fallback )
{
    glm::vec3 val;
    PushConfigTable();

    lua_getfield( m_luaState->m_state, -1, name );  

    if( lua_istable( m_luaState->m_state, -1 ) )
    {
        GetVecX( name, &val[0], 3 );
    }
    else
    {
        if( false == lua_isnil( m_luaState->m_state, -1 ) )
        {
            LOG_ERROR << "Unable to get config value as string: " << name << std::endl;
        }

        val = fallback;
    }

    lua_pop( m_luaState->m_state, 1 );

    PopConfigTable(); 

    return val;
}

glm::vec4 Core::ConfigurationHandler::GetVec4( const char *name, glm::vec4 fallback )
{
    glm::vec4 val;
    PushConfigTable();

    lua_getfield( m_luaState->m_state, -1, name );  

    if( lua_istable( m_luaState->m_state, -1 ) )
    {
        GetVecX( name, &val[0], 4 );
    }
    else
    {
        if( false == lua_isnil( m_luaState->m_state, -1 ) )
        {
            LOG_ERROR << "Unable to get config value as string: " << name << std::endl;
        }

        val = fallback;
    }

    lua_pop( m_luaState->m_state, 1 );

    PopConfigTable(); 

    return val;
}

void Core::ConfigurationHandler::SetString( const char *name, const char *value )
{
}

void Core::ConfigurationHandler::SetDouble( const char *name, double& value )
{
}

void Core::ConfigurationHandler::SetInt( const char *name, int& value )
{
}

void Core::ConfigurationHandler::SetBool( const char *name, bool& value )
{
}

void Core::ConfigurationHandler::SetVec2( const char *name, glm::vec2& value )
{
}

void Core::ConfigurationHandler::SetVec3( const char *name, glm::vec3& value )
{
}

void Core::ConfigurationHandler::SetVec4( const char *name, glm::vec4& value )
{
}

void Core::ConfigurationHandler::GetVecX( const char *name, float * val, int count )
{
    for( int i = 0; i < count; i++ )
    {
        lua_pushnumber( m_luaState->m_state, i+1 );
        lua_gettable( m_luaState->m_state, -2 );
        if( lua_isnumber( m_luaState->m_state, -1 ) )
        {
            val[i] = lua_tonumber( m_luaState->m_state, -1 );
        }
        else
        {
            LOG_ERROR << "Unable to parse the " << i << "th value in " << name << std::endl;
        }

        lua_pop( m_luaState->m_state, 1 );
    }
}

void Core::ConfigurationHandler::PushConfigTable()
{
    lua_getglobal( m_luaState->m_state, "core" );
    lua_getfield( m_luaState->m_state, -1, "config" );
}

void Core::ConfigurationHandler::PopConfigTable()
{
    lua_pop( m_luaState->m_state, 2 );
}

