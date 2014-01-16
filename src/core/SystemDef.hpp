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
#include <Components/AttributeComponent.hpp>
#include <Components/BoundingVolumeComponent.hpp>

namespace Core
{
	class RenderingSystem;
	class MovementSystem;
	class FieldReactionSystem;
	class GPUPFSystem;
	class PickingSystem;
	class AIDebugSystem;
	class LightSystem;
	class CollisionSystem2D;
	class FlowfieldSystem;
	class NavMeshCollisionSystem;
	
    typedef SystemHandlerTemplate<RenderingSystem, LightSystem, NavMeshCollisionSystem, CollisionSystem2D,
		PickingSystem, AIDebugSystem, FlowfieldSystem, /*FieldReactionSystem,*/ GPUPFSystem, MovementSystem> SystemHandler;
    typedef EntityHandlerTemplate<SystemHandler, WorldPositionComponent, RotationComponent, ScaleComponent, 
		GraphicsComponent, LightComponent, MovementComponent, UnitTypeComponent, AttributeComponent,
		BoundingVolumeComponent> EntityHandler;
}

/**********All systems after this line************/
#include <Systems/RenderingSystem.hpp>
#include <Systems/MovementSystem.hpp>
#include <Systems/FieldReactionSystem.hpp>
#include <Systems/GPUPFSystem.hpp>
#include <Systems/PickingSystem.hpp>
#include <Systems/AIDebugSystem.hpp>
#include <Systems/LightSystem.hpp>
#include <Systems/CollisionSystem2D.hpp>
#include <Systems/FlowfieldSystem.hpp>
#include <Systems/NavMeshCollisionSystem.hpp>

#endif
