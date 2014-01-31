#include "GoalSystem.hpp"
#include "World.hpp"
#include <logger/Logger.hpp>

#include <SystemDef.hpp>
#include <gfx/GFXInterface.hpp>
#include <GameUtility/PathfindingUtility.hpp>
#include <GameUtility/NavigationMesh.hpp>


Core::GoalSystem::GoalSystem()
	: BaseSystem( EntityHandler::GenerateAspect<
		WorldPositionComponent, UnitTypeComponent, AttributeComponent, FlowfieldComponent >(), 0ULL )
{
	m_visitedList = nullptr;
}

struct Core::GoalSystem::VisitedListMemoryHandler
{
	VisitedListMemoryHandler()
	{
		visited = nullptr;
	}

	~VisitedListMemoryHandler()
	{
		Core::world.m_systemHandler.GetSystem<Core::GoalSystem>()->InvalidateAllocatedLevelMemory();
		visited = nullptr;
	}

	bool* visited;
};



void Core::GoalSystem::CheckLineWithCornerCheck( glm::vec3 start, glm::vec3 goal, float radius, Core::NavigationMesh* instance, int startNode )
{
	// navmesh exists...
	if( !instance )
		return;

	// only allocate this once per frame, but do it 
	if( m_visitedList == nullptr && m_entities.size() != 0 )
	{
		m_visitedList = Core::world.m_levelHeap.NewObject<Core::GoalSystem::VisitedListMemoryHandler>();
		
		// check memory allocation...
		if( m_visitedList == nullptr )
		{
			LOG_FATAL << "Memory Allocation for GoalSystem failed!" << std::endl;
			return;
		}

		// allocate memory on levelHeap
		m_visitedList->visited = Core::world.m_frameHeap.NewPODArray<bool>( instance->nrNodes );
	}

	/*
	std::vector<int> nodeList;
	nodeList.reserve( instance->nrNodes * 0.4f );

	float distanceToTarget = glm::distance( start, goal );

	// start condition...
	nodeList.push_back( startNode );
	int head = 0;

	//int currentNode = startNode;
	while( nodeList.size() != head )
	{
		Core::NavigationMesh::Node* current = &instance->nodes[ nodeList[head] ];
		for( int p = 0; p < 4; p++ )
		{
			// check if triangle or non-collidable line...
			if( current->corners[p].length < 0 || current->corners[p].linksTo >= 0 )
				continue;
		
			int ii = p * 2;
			int oo = ( ii + 2 ) % 8;
			glm::vec3 lineStart = glm::vec3( current->points[ ii ], 0, current->points[ ii + 1 ] );
			glm::vec3 lineEnd	= glm::vec3( current->points[ oo ], 0, current->points[ oo + 1 ] );

			// check vs. ray
			float A = glm::determinant( glm::mat2x2( start.x, start.z, goal.x, goal.z ) );
			float B = glm::determinant( glm::mat2x2( lineStart.x, lineStart.z, lineEnd.x, lineEnd.z ) );
			float divider = glm::determinant( glm::mat2x2( start.x - goal.x, start.z - goal.z, lineStart.x - lineEnd.x, lineStart.z - lineEnd.z  ) );
				
			float intersectionX = glm::determinant( glm::mat2x2( A, start.x - goal.x , B, lineStart.x - lineEnd.x ) ) / divider;
			float intersectionZ = glm::determinant( glm::mat2x2( A, start.z - goal.z , B, lineStart.z - lineEnd.z ) ) / divider;
		
			glm::vec3 hit = glm::vec3( intersectionX, 0.0f, intersectionZ );
		
			float alongLine = glm::dot( hit - lineStart, (lineEnd - lineStart) * current->corners[p].inverseLength );

			// check vs. corner
			glm::vec3 toSphere = glm::vec3( current->points[ii], 0.0f, current->points[ii+1] ) - start;
			float squareLength = glm::dot( toSphere, toSphere );
			float projectedDistanceToSphere = glm::dot( toSphere, glm::normalize( goal - start ) );

			float t = std::numeric_limits<float>::max();

			// ray is inside the sphere, ignore...
			// sphere is behind the ray and ray is not inside it, ignore...
			// the sphere is too far from the line, ignore...
			if( !(squareLength < radius * radius || projectedDistanceToSphere < 0 || squareLength - projectedDistanceToSphere * projectedDistanceToSphere > radius * radius) ) 
			{
				float sphereIntersectionDelta = sqrt( radius * radius - (squareLength - projectedDistanceToSphere * projectedDistanceToSphere) );	
				if( projectedDistanceToSphere - sphereIntersectionDelta < result.t || result.t < 0.0f )
					t = projectedDistanceToSphere - sphereIntersectionDelta;
			}

			if( alongLine > 0 && alongLine < current->corners[p].length )
			{
				glm::vec3 hitPos = lineStart + alongLine * (lineEnd - lineStart) * current->corners[p].inverseLength;
				float line_t = glm::distance( hitPos, start );
				if( line_t < t && glm::dot( goal - start, hitPos - start ) > 0 )
					t = line_t;
			}

			if( ( t < result.t ||  result.t < 0 ) && t > 0.01f && t != std::numeric_limits<float>::max() )
			{
				result.node = nodeList[head];
				result.edge = p;
				result.t = t;
			}

		}

		// has not intersected with any m_nodes yet, add more m_nodes to list
		if( result.t < 0 )
		{
			for( int p = 0; p < 4; p++ )
			{
				// check if triangle or collidable line...
				if( current->corners[p].length < 0 
					|| current->corners[p].linksTo < 0 
					|| visited[current->corners[p].linksTo] )
					continue;
					
				visited[ current->corners[p].linksTo ] = true;
				nodeList.push_back( current->corners[p].linksTo );
			}
		}

		// done with current node, step to next...
		head++;
	}

	if( result.t > distanceToTarget - 0.01f )
		result.t = NO_LINE_COLLISION_VALUE;
	*/
}
/*
*/








void Core::GoalSystem::Update( float delta )
{


	CheckLineWithCornerCheck( glm::vec3(0), glm::vec3(0), 3.0f, Core::GetNavigationMesh(), 0 );


	//for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
	//{
	//	
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//}

}
