#ifndef GTEST_CORE_SYSTEMHANDLERAUX_H
#define GTEST_CORE_SYSTEMHANDLERAUX_H

#include <ComponentFramework/SystemHandlerTemplate.hpp>
#include <ComponentFramework/BaseSystem.hpp>
namespace Core
{
    class System1 : public BaseSystem
    {
        public:
            System1() : BaseSystem(0,0){};
            virtual void Update( float delta ) override {}
            virtual ~System1() {};

    };

    class System2 : public BaseSystem
    {
        public:
            System2() : BaseSystem(0,0){};
            virtual void Update( float delta ) override {}
            virtual ~System2() {};
    };

    class System3 : public BaseSystem
    {
        public:
            System3() : BaseSystem(0,0){};
            virtual void Update( float delta ) override{}
            virtual ~System3() {};
    };

    typedef SystemHandlerTemplate<System1,System2,System3> SystemHandler;
}

#endif
