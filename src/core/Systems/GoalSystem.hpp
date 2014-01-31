#ifndef SRC_CORE_SYSTEMS_GOALSYSTEM_HPP
#define SRC_CORE_SYSTEMS_GOALSYSTEM_HPP

#include <ComponentFramework/BaseSystem.hpp>
#include <glm/glm.hpp>

namespace Core
{
	struct NavigationMesh;

	/*!
		Overrides direction from flowfieldsystem
	*/
    class GoalSystem : public BaseSystem
    {
	private :
		void CheckLineWithCornerCheck( glm::vec3 start, glm::vec3 goal, float radius, NavigationMesh* instance, int startNode );

    public :
        virtual void Update( float delta ) override;

        GoalSystem();

        virtual const char* GetHumanName() { return "GoalSystem"; }

		void InvalidateAllocatedLevelMemory() { m_visitedList = nullptr; }

	private :
		struct VisitedListMemoryHandler;

		/*!
			Used to keep track of level switching...
		*/
		VisitedListMemoryHandler* m_visitedList;


    };
}


#endif
