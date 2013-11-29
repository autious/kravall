#ifndef SRC_CORE_COMPONENTFRAMEWORK_ENTITYHANDLER_H
#define SRC_CORE_COMPONENTFRAMEWORK_ENTITYHANDLER_H

#include "PVector.hpp"
#include "EntityVector.hpp"
#include "ComponentType.hpp"
#include "CheckTemplatePresence.hpp"

#include <cstdint>
#include <cassert>
#include <array>

#define SA_COMPONENT_USE "Component doesn't exist in EntityHandler. Maybe you forgot to add it?"

namespace Core
{

    template<typename... Components>
    class EntityHandlerTemplate
    {
        static const int COMPONENT_COUNT = sizeof...(Components);

        std::vector<int[COMPONENT_COUNT]> m_entityIds;
        std::vector<int> m_deletedEntities;

        std::array<PVector,sizeof...(Components)> p = {{PVector(1024,64,sizeof(Components))...}};

    public:
        EntityHandlerTemplate()
        {
        }

        template<typename... EntityComponents>
        Entity CreateEntity()
        {
            
        }

        template<typename Component>
        size_t constexpr GetComponentTypeId( )
        {
            static_assert(std::is_pod<Component>::value, "Components must be Pure Data Objects");
            static_assert( Match<Component,Components...>::exists, SA_COMPONENT_USE );
            return Index<Component,std::tuple<Components...>>::value;
        }
    
        template<typename Component>
        Component* GetComponentTmpPointer( Entity entity )
        {
            static_assert( Match<Component,Components...>::exists, SA_COMPONENT_USE );
        }

        template<typename... AspectComponents>
        Aspect constexpr GenerateAspect( )
        {
            static const size_t ids[] = { GetComponentTypeId<AspectComponents>()... };
            return GenerateAspect( ids, Aspect(), 0, sizeof...(AspectComponents) ); 
        }

        Aspect constexpr GenerateAspect( const size_t *id, Aspect asp, int i, int size )
        {
               return asp |= (1ULL << id[i] | (i < size ? GenerateAspect(id,asp,i+1,size) : 0ULL )); 
        }

    private:
    };
}
#endif
