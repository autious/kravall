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
#include <Components/FlowfieldComponent.hpp>
#include <Components/AreaComponent.hpp>
#include <Components/NameComponent.hpp>
#include <Components/AnimationComponent.hpp>

/* TODO: Put your system up here instead of the end of the header. noobs */
#include <Systems/NameSystem.hpp>
#include <Systems/AreaSystem.hpp>

namespace Core
{
	class RenderingSystem;
	class MovementSystem;
	class FieldReactionSystem;
	class PickingSystem;
	class AIDebugSystem;
	class LightSystem;
	class CollisionSystem2D;
	class FlowfieldSystem;
	class NavMeshCollisionSystem;
	class FlowfieldPositioningSystem;
	class PoliceGoalSystem;
	
    typedef SystemHandlerTemplate<RenderingSystem, LightSystem, PoliceGoalSystem, FlowfieldPositioningSystem, NavMeshCollisionSystem, 
		CollisionSystem2D, PickingSystem, AIDebugSystem, FlowfieldSystem, FieldReactionSystem, 
		MovementSystem, NameSystem, AreaSystem> SystemHandler;

    typedef EntityHandlerTemplate<SystemHandler, WorldPositionComponent, RotationComponent, ScaleComponent, 
		GraphicsComponent, LightComponent, MovementComponent, UnitTypeComponent, AttributeComponent,
		BoundingVolumeComponent, FlowfieldComponent,AreaComponent,NameComponent, AnimationComponent> EntityHandler;
}

/**********All systems after this line************/
#include <Systems/RenderingSystem.hpp>
#include <Systems/MovementSystem.hpp>
#include <Systems/FieldReactionSystem.hpp>
#include <Systems/PickingSystem.hpp>
#include <Systems/AIDebugSystem.hpp>
#include <Systems/LightSystem.hpp>
#include <Systems/CollisionSystem2D.hpp>
#include <Systems/FlowfieldSystem.hpp>
#include <Systems/NavMeshCollisionSystem.hpp>
#include <Systems/FlowfieldPositioningSystem.hpp>
#include <Systems/PoliceGoalSystem.hpp>

#endif
