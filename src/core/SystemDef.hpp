#ifndef SRC_CORE_SYSTEMDEF_H
#define SRC_CORE_SYSTEMDEF_H

#include <ComponentFramework/EntityHandlerTemplate.hpp>
#include <ComponentFramework/SystemHandlerTemplate.hpp>

#include <Components/ExampleComponent1.hpp>
#include <Components/ExampleComponent2.hpp>
#include <Components/WorldPositionComponent.hpp>
#include <Components/RotationComponent.hpp>
#include <Components/ScaleComponent.hpp>
#include <Components/GraphicsComponent.hpp>

namespace Core
{
    class ExampleSystem;
	class RenderingSystem;
    typedef SystemHandlerTemplate<ExampleSystem, RenderingSystem> SystemHandler;
    typedef EntityHandlerTemplate<SystemHandler,ExampleComponent1,ExampleComponent2, 
		WorldPositionComponent, RotationComponent, ScaleComponent, GraphicsComponent> EntityHandler;
}

/**********All systems after this line************/
#include <Systems/ExampleSystem.hpp>
#include <Systems/RenderingSystem.hpp>
#endif
