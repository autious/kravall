#ifndef SRC_CORE_SYSTEMS_NAVIGATIONMESHCOLLISIONSYSTEM_HPP
#define SRC_CORE_SYSTEMS_NAVIGATIONMESHCOLLISIONSYSTEM_HPP

#include <SystemDef.hpp>


namespace Core
{
	/*!
		System responsible for checking entities positions vs. the navigation mesh
		and resolve eventual collisions.
	*/
    class NavMeshCollisionSystem : public BaseSystem
    {

    public:
        virtual void Update( float delta ) override;

        NavMeshCollisionSystem();

        virtual const char* GetHumanName() { return "NavMeshCollisionSystem"; }
    };
}


#endif
