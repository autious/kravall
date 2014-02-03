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
#include <Components/SquadComponent.hpp>
#include <Components/FormationComponent.hpp>
#include <Components/TargetingComponent.hpp>

/* TODO: Put your system up here instead of the end of the header. noobs */
#include <Systems/RenderingSystem.hpp>
#include <Systems/NameSystem.hpp>
#include <Systems/AreaSystem.hpp>
#include <Systems/GroupDataSystem.hpp>
#include <Systems/AnimationSystem.hpp>
#include <Systems/SquadSystem.hpp>
#include <Systems/TargetingSystem.hpp>
#include <Systems/MoodStateSystem.hpp>

namespace Core
{
	class MovementSystem;
	class FieldReactionSystem;
	class GPUPFSystem;
	class PickingSystem;
	class AIDebugSystem;
	class LightSystem;
	class CollisionSystem2D;
	class FlowfieldSystem;
	class NavMeshCollisionSystem;
    class GroupDataSystem;
	class FlowfieldPositioningSystem;
	class PoliceGoalSystem;
	class MoodStateSystem;
	
    typedef SystemHandlerTemplate<LightSystem, PoliceGoalSystem, FlowfieldPositioningSystem, NavMeshCollisionSystem, 
	CollisionSystem2D, PickingSystem, TargetingSystem, AIDebugSystem, FlowfieldSystem, /*FieldReactionSystem,*/ GPUPFSystem,
	MovementSystem, NameSystem, GroupDataSystem, AreaSystem, MoodStateSystem, AnimationSystem, SquadSystem, RenderingSystem> SystemHandler;

    typedef EntityHandlerTemplate<SystemHandler, WorldPositionComponent, RotationComponent, ScaleComponent, 
		GraphicsComponent, LightComponent, MovementComponent, UnitTypeComponent, AttributeComponent,
		BoundingVolumeComponent, FlowfieldComponent, AreaComponent,NameComponent, AnimationComponent,
        SquadComponent, FormationComponent, TargetingComponent> EntityHandler;
}

/**********All systems after this line************/
#include <Systems/MovementSystem.hpp>
#include <Systems/FieldReactionSystem.hpp>
#include <Systems/GPUPFSystem.hpp>
#include <Systems/PickingSystem.hpp>
#include <Systems/AIDebugSystem.hpp>
#include <Systems/LightSystem.hpp>
#include <Systems/CollisionSystem2D.hpp>
#include <Systems/FlowfieldSystem.hpp>
#include <Systems/NavMeshCollisionSystem.hpp>
#include <Systems/FlowfieldPositioningSystem.hpp>
#include <Systems/PoliceGoalSystem.hpp>

#endif
