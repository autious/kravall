#ifndef SRC_CORE_COMPONENTFRAMEWORK_ENTITYHANDLER_H
#define SRC_CORE_COMPONENTFRAMEWORK_ENTITYHANDLER_H

#include <typeinfo>

typedef unsigned int Entity;

namespace Core
{
    template<typename... Components>
    class EntityHandler
    {
    private:
        class ComponentMap
        {
            constexpr ComponentMap()
            {
                
            }

            void constexpr GetId()
            {
            
            }

            static constexpr std::type_info m_infoId[sizeof...(Components)];
        };
    public:
        EntityHandler();

        void *m_components[sizeof...(Components)];
        int m_componentSizes[sizeof...(Components)];
    };
}

#endif
