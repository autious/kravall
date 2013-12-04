#ifndef SRC_CORE_WORLD_H
#define SRC_CORE_WORLD_H
#include <SystemDef.hpp>
#include <Lua/LuaState.hpp>
#include <Lua/ConfigurationHandler.hpp>

#define WGETC Core::world.m_entityHandler.GetComponentTmpPointer

namespace Core
{
    class World
    {
    public:
        World() : m_systemHandler(), m_entityHandler( &m_systemHandler ), m_luaState(), m_config( &m_luaState ) {}
        SystemHandler m_systemHandler;
        EntityHandler m_entityHandler;             
        LuaState m_luaState;
        ConfigurationHandler m_config;
    };

    extern World world;
}


#endif
