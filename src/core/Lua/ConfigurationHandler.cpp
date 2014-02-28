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

std::vector<std::string> Core::ConfigurationHandler::GetVectorString( const char *name )
{
    std::vector<std::string> ret;
    PushConfigTable();
        lua_getfield( m_luaState->m_state, -1, name );
            if( lua_istable( m_luaState->m_state, -1 ) )
            {
                lua_pushnil( m_luaState->m_state );
                while( lua_next( m_luaState->m_state, -2 ) != 0 )
                {
                    if( lua_isstring( m_luaState->m_state, -1 ) )
                    {
                        ret.push_back( std::string( lua_tostring( m_luaState->m_state, -1 ) ) ); 
                    }
                    lua_pop( m_luaState->m_state, 1 ); //pop value, leave key
                } 
            }
        lua_pop( m_luaState->m_state, 1 );
    PopConfigTable();

    return ret;
}

void Core::ConfigurationHandler::SetString( const char *name, const char *value )
{
    PushConfigTable();
    lua_pushstring( m_luaState->m_state, value );
    lua_setfield( m_luaState->m_state, -2, name );
    PopConfigTable();
}

void Core::ConfigurationHandler::SetDouble( const char *name, double& value )
{
    PushConfigTable();
    lua_pushnumber( m_luaState->m_state, value );
    lua_setfield( m_luaState->m_state, -2, name );
    PopConfigTable();
}

void Core::ConfigurationHandler::SetInt( const char *name, int& value )
{
    PushConfigTable();
    lua_pushinteger( m_luaState->m_state, value );
    lua_setfield( m_luaState->m_state, -2, name );
    PopConfigTable();
}

void Core::ConfigurationHandler::SetBool( const char *name, bool& value )
{
    PushConfigTable();
    lua_pushboolean( m_luaState->m_state, value );
    lua_setfield( m_luaState->m_state, -2, name );
    PopConfigTable();
}

void Core::ConfigurationHandler::SetVec2( const char *name, glm::vec2& value )
{
    PushConfigTable();
    lua_newtable( m_luaState->m_state );
        PushVecX( &value[0], 2 );
    lua_setfield( m_luaState->m_state, -2, name );
    PopConfigTable();
}

void Core::ConfigurationHandler::SetVec3( const char *name, glm::vec3& value )
{
    PushConfigTable();
    lua_newtable( m_luaState->m_state );
        PushVecX( &value[0], 3 );
    lua_setfield( m_luaState->m_state, -2, name );
    PopConfigTable();
}

void Core::ConfigurationHandler::SetVec4( const char *name, glm::vec4& value )
{
    PushConfigTable();
    lua_newtable( m_luaState->m_state );
        PushVecX( &value[0], 4 );
    lua_setfield( m_luaState->m_state, -2, name );
    PopConfigTable();
}


void Core::ConfigurationHandler::PushVecX( float * val, int count )
{
    for( int i = 0; i < count; i++ )
    {
        lua_pushinteger( m_luaState->m_state, i+1 );
        lua_pushnumber( m_luaState->m_state, val[i] );
        lua_settable( m_luaState->m_state, -3 );
    }
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

    if( !lua_istable( m_luaState->m_state, -1 ) )
    {
        LOG_ERROR << "Config table is not table, it's " << lua_typename( m_luaState->m_state, -1 ) << std::endl;
        lua_pop( m_luaState->m_state, 1 );
        lua_newtable( m_luaState->m_state );
    }
}

void Core::ConfigurationHandler::PopConfigTable()
{
    lua_pop( m_luaState->m_state, 2 );
}
