#ifndef SRC_CORE_WORLD_H
#define SRC_CORE_WORLD_H
#include <SystemDef.hpp>

#define WGETC Core::world.m_entityHandler.GetComponentTmpPointer

namespace Core
{
    class World
    {
    public:
        World() : m_systemHandler(), m_entityHandler( &m_systemHandler ){}
        SystemHandler m_systemHandler;
        EntityHandler m_entityHandler;             
    };

    extern World world;
}


#endif
