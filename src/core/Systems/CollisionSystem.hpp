#ifndef SRC_CORE_SYSTEMS_COLLISIONSYSTEM_HPP
#define SRC_CORE_SYSTEMS_COLLISIONSYSTEM_HPP

#include <SystemDef.hpp>


namespace Core
{
	/*!
		Handles collision detection and resolution for bounding sphere - bounding shpere enteties. 
		Records enteties with WorldPositionComponent and BoundingVolumeComponents.
		Will not move enteties marked as static in boundingVolumeComponent.
	*/
    class CollisionSystem : public BaseSystem
    {

    public:
        virtual void Update( float delta ) override;

        CollisionSystem();

        virtual const char* GetHumanName() { return "CollisionSystem"; }
    };
}

#endif
