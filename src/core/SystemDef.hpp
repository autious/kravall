#ifndef SRC_CORE_SYSTEMDEF_H
#define SRC_CORE_SYSTEMDEF_H

#include <ComponentFramework/EntityHandlerTemplate.hpp>
#include <ComponentFramework/SystemHandlerTemplate.hpp>
#include <Components/ExampleComponent1.hpp>
#include <Components/ExampleComponent2.hpp>

namespace Core
{
    class ExampleSystem;
    typedef SystemHandlerTemplate<ExampleSystem> SystemHandler;
    typedef EntityHandlerTemplate<SystemHandler,ExampleComponent1,ExampleComponent2> EntityHandler;
}

/**********All systems after this line************/
#include <Systems/ExampleSystem.hpp>

#endif
