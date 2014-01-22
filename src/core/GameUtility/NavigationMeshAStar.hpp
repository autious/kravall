#ifndef GAME_UTILITY_PATHFINDER_HPP
#define GAME_UTILITY_PATHFINDER_HPP

#include <algorithm>
#include <logger/Logger.hpp>
#include <glm/glm.hpp>

namespace Core
{
	// Im going to fix this one of these days ;)
	//struct PathComponent
	//{
	//	int nrNodes;
	//	float nodes[ 2 * 50 ];
	//};
	//static PathComponent GetPath( glm::vec3 start, glm::vec3 goal );



	class PathFinder
	{

	public:		
		

		static bool CheckLineVsNavMesh( glm::vec3 from, glm::vec3 to );
		static bool CheckLineVsNavMesh( glm::vec3 from, glm::vec3 to, int startNode );

	
	private:

				


	};

	
}

#endif

