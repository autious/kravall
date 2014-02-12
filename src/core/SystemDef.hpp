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
#include <Components/HoverTextComponent.hpp>

/**********All systems after this line************/
#include <Systems/RenderingSystem.hpp>
#include <Systems/NameSystem.hpp>
#include <Systems/AIsupport/AreaSystem.hpp>
#include <Systems/AIsupport/GroupDataSystem.hpp>
#include <Systems/AI/NavMeshBlockingSystem.hpp>
#include <Systems/AnimationSystem.hpp>
#include <Systems/AI/SquadSystem.hpp>
#include <Systems/AI/TargetingSystem.hpp>
#include <Systems/AI/MoodStateSystem.hpp>
#include <Systems/AIsupport/RemovalSystem.hpp>
#include <Systems/HoverTextSystem.hpp>
#include <Systems/AI/RioterAnimationSystem.hpp>
#include <Systems/AI/AttackAnimationSystem.hpp>
#include <Systems/AIsupport/PriorityResetSystem.hpp>
#include <Systems/AI/RioterGoalSystem.hpp>
#include <Systems/AIsupport/AIDebugSystem.hpp>
#include <Systems/AIsupport/CollisionSystem2D.hpp>
#include <Systems/AIsupport/NavMeshCollisionSystem.hpp>
#include <Systems/AIsupport/FlowfieldPositioningSystem.hpp>
#include <Systems/AI/MovementSystem.hpp>
#include <Systems/FieldReactionSystem.hpp>
#include <Systems/AI/GPUPFSystem.hpp>
#include <Systems/PickingSystem.hpp>
#include <Systems/LightSystem.hpp>
#include <Systems/AI/FlowfieldSystem.hpp>
#include <Systems/AI/PoliceGoalSystem.hpp>

namespace Core
{

    typedef SystemHandlerTemplate<PriorityResetSystem, LightSystem, FlowfieldPositioningSystem, PoliceGoalSystem, NavMeshCollisionSystem, 
	CollisionSystem2D, PickingSystem, TargetingSystem, AttackAnimationSystem, AIDebugSystem, FlowfieldSystem, /*FieldReactionSystem,*/ GPUPFSystem,
	RioterGoalSystem, MovementSystem, NameSystem, GroupDataSystem, AreaSystem, NavMeshBlockingSystem, MoodStateSystem, RioterAnimationSystem, 
	AnimationSystem, SquadSystem, RemovalSystem, HoverTextSystem, RenderingSystem> SystemHandler;

    typedef EntityHandlerTemplate<SystemHandler, WorldPositionComponent, RotationComponent, ScaleComponent, 
		GraphicsComponent, LightComponent, MovementComponent, UnitTypeComponent, AttributeComponent,
		BoundingVolumeComponent, FlowfieldComponent, AreaComponent,NameComponent, AnimationComponent,
        SquadComponent, FormationComponent, TargetingComponent, HoverTextComponent> EntityHandler;
}



#endif
