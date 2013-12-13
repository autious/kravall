#ifndef SRC_CORE_SYSTEMDEF_HPP
#define SRC_CORE_SYSTEMDEF_HPP

#include <ComponentFramework/EntityHandlerTemplate.hpp>
#include <ComponentFramework/SystemHandlerTemplate.hpp>
#include <Components/ExampleComponent1.hpp>
#include <Components/ExampleComponent2.hpp>

#include <Components/WorldPositionComponent.hpp>
#include <Components/RotationComponent.hpp>
#include <Components/ScaleComponent.hpp>
#include <Components/GraphicsComponent.hpp>
#include "Components/MovementComponent.hpp"
#include "Components/UnitTypeComponent.hpp"
#include "Components/AttributeRioterComponent.hpp"
#include "Components/AttributePoliceComponent.hpp"
#include "Components/BoundingVolumeComponent.hpp"

namespace Core
{
    class ExampleSystem;
	class RenderingSystem;
	class MovementSystem;
	class PickingSystem;
	class AIDebugSystem;
	
    typedef SystemHandlerTemplate<ExampleSystem, RenderingSystem, MovementSystem, PickingSystem, AIDebugSystem> SystemHandler;
    typedef EntityHandlerTemplate<SystemHandler,ExampleComponent1,ExampleComponent2, WorldPositionComponent, 
		RotationComponent, ScaleComponent, GraphicsComponent, MovementComponent, UnitTypeComponent, 
		AttributeRioterComponent, AttributePoliceComponent, BoundingVolumeComponent> EntityHandler;
}

/**********All systems after this line************/
#include <Systems/ExampleSystem.hpp>
#include <Systems/RenderingSystem.hpp>
#include "Systems/MovementSystem.hpp"
#include <Systems/PickingSystem.hpp>
#include <Systems/AIDebugSystem.hpp>
#endif
