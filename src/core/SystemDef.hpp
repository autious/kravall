#ifndef SRC_CORE_SYSTEMDEF_HPP
#define SRC_CORE_SYSTEMDEF_HPP

#include <ComponentFramework/EntityHandlerTemplate.hpp>
#include <ComponentFramework/SystemHandlerTemplate.hpp>

#include <Components/WorldPositionComponent.hpp>
#include <Components/RotationComponent.hpp>
#include <Components/ScaleComponent.hpp>
#include <Components/GraphicsComponent.hpp>
#include <Components/LightComponent.hpp>
#include <Components/MovementComponent.hpp>
#include <Components/UnitTypeComponent.hpp>
#include <Components/AttributeRioterComponent.hpp>
#include <Components/AttributePoliceComponent.hpp>
#include <Components/BoundingVolumeComponent.hpp>

namespace Core
{
	class RenderingSystem;
	class MovementSystem;
	class PickingSystem;
	class AIDebugSystem;
	class LightSystem;
	class CollisionSystem;
	
    typedef SystemHandlerTemplate<RenderingSystem, LightSystem, MovementSystem, PickingSystem, AIDebugSystem, CollisionSystem> SystemHandler;
    typedef EntityHandlerTemplate<SystemHandler, WorldPositionComponent, RotationComponent, ScaleComponent, 
		GraphicsComponent, LightComponent, MovementComponent, UnitTypeComponent, AttributeRioterComponent, AttributePoliceComponent,
		BoundingVolumeComponent> EntityHandler;
}

/**********All systems after this line************/
#include <Systems/RenderingSystem.hpp>
#include <Systems/MovementSystem.hpp>
#include <Systems/PickingSystem.hpp>
#include <Systems/AIDebugSystem.hpp>
#include <Systems/LightSystem.hpp>
#include <Systems/CollisionSystem.hpp>
#endif
