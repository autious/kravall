#ifndef SRC_CORE_SYSTEMS_NAVMESHBLOCKINGSYSTEM_HPP
#define SRC_CORE_SYSTEMS_NAVMESHBLOCKINGSYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>

namespace Core
{
	/*!
		Calculates wether edges and nodes in the navigation mesh has been blocked by police.
	*/
    class NavMeshBlockingSystem : public BaseSystem
    {
	private:

		void CalculateBlockedNodes( int targetRioterGroup );
		void FreeBlockedNodes( int targetRioterGroup );


    public:
        virtual void Update( float delta ) override;

        NavMeshBlockingSystem();

        virtual const char* GetHumanName() { return "NavMeshBlockingSystem"; }
    };
}


#endif
