#include "ConfigurationHandler.hpp"

#include <logger/Logger.hpp>
#include <string.h>
#include <string>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <Lua/LuaState.hpp>


Core::ConfigurationHandler::ConfigurationHandler( LuaState* state )
{
    m_luaState = state;
}

std::string Core::ConfigurationHandler::GetString( const char *name, const char *fallback )
{
}

double Core::ConfigurationHandler::GetDouble( const char *name, double fallback )
{
}

int Core::ConfigurationHandler::GetInt( const char *name, int fallback )
{
}

bool Core::ConfigurationHandler::GetBool( const char *name, bool fallback )
{
}

glm::vec2 Core::ConfigurationHandler::GetVec2( const char *name, glm::vec2 fallback )
{
}

glm::vec3 Core::ConfigurationHandler::GetVec3( const char *name, glm::vec3 fallback )
{

}

glm::vec4 Core::ConfigurationHandler::GetVec4( const char *name, glm::vec4 fallback )
{
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

void Core::ConfigurationHandler::PushConfigTable()
{

}

void Core::ConfigurationHandler::PopConfigTable()
{

}

