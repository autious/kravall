#include "PathfindingUtility.hpp"
#include <GameUtility/NavigationMesh.hpp>
#include <World.hpp>

#include <algorithm>
#include <logger/Logger.hpp>

//#define DRAW_GOAL_VISION_LINES
#ifdef DRAW_GOAL_VISION_LINES
#define DEBUG_DRAW_VISION( x ) x
#else
#define DEBUG_DRAW_VISION( x ) ;
#endif


namespace Core
{

	struct LineCheckReturnStruct
	{
		int node, edge;
		float t;
	};
	
	bool CheckLineWithCornerCheck( glm::vec3 start, glm::vec3 goal, float radius, Core::NavigationMesh* instance, int startNode )
	{
		bool samePoint = true;
		for( int i = 0; i < 3; i++ )
			if( start[i] != goal[i] )
				samePoint = false;
		if( samePoint )
			return false;

		glm::vec3 dir = glm::normalize( goal - start );
		float hitDistance = 0;
		float distanceToTarget = glm::distance( start, goal );
		int nextNode = startNode;
		int entryEdge = -1;
		while( hitDistance <= distanceToTarget && nextNode >= 0 )
		{
			Core::NavigationMesh::Node* current = &instance->nodes[ nextNode ];
			int nrCorners =  current->corners[3].length < 0 ? 3 : 4;

			nextNode = -1;

			for( int i = 0; i < nrCorners; i++ )
			{		
				if( i == entryEdge )
					continue;

				int ii = i * 2;
				int oo = ( ii + 2 ) % 8;
				glm::vec3 lineStart = glm::vec3( current->points[ ii ], 0, current->points[ ii + 1 ] );
				glm::vec3 lineEnd	= glm::vec3( current->points[ oo ], 0, current->points[ oo + 1 ] );

				glm::vec3 planeNormal = glm::normalize( glm::cross( (lineEnd - lineStart), glm::vec3( 0.0f, 1.0f, 0.0f ) ) );				
				float dot = glm::dot( planeNormal, dir );
				float distance = glm::dot(lineStart - start, planeNormal) / dot;				
				glm::vec3 hit = start + dir * distance;
		
				float alongLine = glm::dot( hit - lineStart, (lineEnd - lineStart) * current->corners[i].inverseLength );

				float t = std::numeric_limits<float>::max();

				// ray is inside the sphere, ignore...
				// sphere is behind the ray and ray is not inside it, ignore...
				// the sphere is too far from the line, ignore...
				if( current->corners[i].cornerConnectsToNode[0] == nextNode )
				{
					if( abs( glm::dot( goal - glm::vec3( current->points[ii], 0.0f, current->points[ii+1] ), 
						goal - glm::vec3( current->points[ii], 0.0f, current->points[ii+1] ) )) > radius * radius )
					{
						// check vs. corner
						glm::vec3 toSphere = glm::vec3( current->points[ii], 0.0f, current->points[ii+1] ) - start;
						float squareLength = glm::dot( toSphere, toSphere );
						float projectedDistanceToSphere = glm::dot( toSphere, glm::normalize( goal - start ) );

						if( !(squareLength < radius * radius || projectedDistanceToSphere < 0 || squareLength - projectedDistanceToSphere * projectedDistanceToSphere > radius * radius) ) 
						{
							float sphereIntersectionDelta = sqrt( radius * radius - (squareLength - projectedDistanceToSphere * projectedDistanceToSphere) );	
							t = projectedDistanceToSphere - sphereIntersectionDelta;
						}
					}
				}

				if( alongLine > 0 && alongLine < current->corners[i].length )
				{
					glm::vec3 hitPos = lineStart + alongLine * (lineEnd - lineStart) * current->corners[i].inverseLength;
					float line_t = glm::distance( hitPos, start );
					if( line_t < t && glm::dot( goal - start, hitPos - start ) > 0 )
						t = line_t;
				}

				if( t > 0.001f && t != std::numeric_limits<float>::max() && t > hitDistance + 0.001f )
				{
					nextNode = current->corners[i].linksTo;
					entryEdge = current->corners[i].linksToEdge;
					hitDistance = t;
				}
			}
		}

		DEBUG_DRAW_VISION( GFX::Debug::DrawLine( start, start + glm::normalize( goal - start ) * hitDistance, GFXColor( 1, 1, 0 , 1 ), true ) );


		if( hitDistance < distanceToTarget )
			return true;
		return false;
	}





	bool PathFinder::CheckLineVsNavMesh( glm::vec3 from, glm::vec3 to, float cornerRadius, int startNode )
	{
		Core::NavigationMesh* instance = Core::GetNavigationMesh();
		if( !instance || startNode < 0 )
			return false;

		return CheckLineWithCornerCheck( from, to, cornerRadius, instance, startNode );
	}

}