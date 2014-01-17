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

/* TODO: Put your system up here instead of the end of the header. noobs */
#include <Systems/NameSystem.hpp>
#include <Systems/AreaLuaUpdateSystem.hpp>

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
    class GroupDataSystem;
	class FlowfieldPositioningSystem;
	
    typedef SystemHandlerTemplate<RenderingSystem, LightSystem, FlowfieldPositioningSystem, NavMeshCollisionSystem, 
		CollisionSystem2D, PickingSystem, AIDebugSystem, FlowfieldSystem, FieldReactionSystem, 
		MovementSystem, NameSystem, GroupDataSystem, AreaLuaUpdateSystem> SystemHandler;

    typedef EntityHandlerTemplate<SystemHandler, WorldPositionComponent, RotationComponent, ScaleComponent, 
		GraphicsComponent, LightComponent, MovementComponent, UnitTypeComponent, AttributeComponent,
		BoundingVolumeComponent, FlowfieldComponent,AreaComponent,NameComponent> EntityHandler;
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
<<<<<<< HEAD
#include <Systems/GroupDataSystem.hpp>
=======
#include <Systems/FlowfieldPositioningSystem.hpp>
>>>>>>> 6307259a30528a556be4b2c3dd81777c67674513

#endif
