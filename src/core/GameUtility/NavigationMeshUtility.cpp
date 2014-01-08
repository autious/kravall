#include "NavigationMesh.hpp"
#include <World.hpp>

#include <algorithm>
#include <logger/Logger.hpp>

namespace Core
{
	void NavigationMesh::DrawDebug()
	{
		Core::NavigationMesh* instance = Core::GetNavigationMesh();

		for( int i = 0; i < instance->nrNodes; i++ )
		{
			float* points = instance->nodes[i].points;

			for( int p = 0; p < 4; p++ )
			{
				if( instance->nodes[i].corners[p].length < 0 )
					continue;

				int ii = p * 2;
				int oo = (ii + 2) % 8;	

				glm::vec3 lineStart = glm::vec3( points[ ii ], 0.0f, points[ ii + 1 ] );
				glm::vec3 lineEnd	= glm::vec3( points[ oo ], 0.0f, points[ oo + 1 ] );
				GFX::Debug::DrawLine( 
					lineStart, lineEnd, GFXColor( 0.3f, 1.0f, 1.0f, 1.0f ), true );
			}
		}
	}


	bool NavigationMesh::CheckPointInsideNode( glm::vec3 point, int node )
	{
		Core::NavigationMesh* instance = Core::GetNavigationMesh();
		for( int i = 0; i < 4; i++ )
		{
			if( instance->nodes[node].corners[i].linksTo < -1.5f )
				continue;

			int ii = i * 2;
			int oo = (ii + 2) % 8;
			float* points = instance->nodes[node].points;

			glm::vec3 lineStart = glm::vec3( points[ ii ], 0.0f, points[ ii + 1 ] );
			glm::vec3 lineEnd	= glm::vec3( points[ oo ], 0.0f, points[ oo + 1 ] );

			// calc normal
			glm::vec3 cross = glm::normalize( glm::cross( (lineEnd - lineStart), glm::vec3( 0.0f, 1.0f, 0.0f ) ) );

			// check if behind plane
			float dot = glm::dot( cross, point - lineStart );
			if( dot <= 0 )
				return false;
		}

		// point is inside all lines
		return true;
	}

	struct TraversalData
	{
		TraversalData( int node, int entryEdge, float entryDistance ) 
		: node(node), entryEdge(entryEdge), entryDistance(entryDistance) {}

		int node;
		int entryEdge;
		float entryDistance;
	};

	bool sortingFunction( TraversalData a1, TraversalData a2 )
	{
		if( a1.entryDistance < a2.entryDistance)
			return true;
		return false;
	}


	bool NavigationMesh::CalculateFlowfieldForGroup( glm::vec3 point, int group )
	{
		if( group >= maxFlowfields )
		{
			LOG_FATAL << "Trying to calculate flowfield for group id above max nr groups, in function \"CalculateFlowfieldForGroup\"" << std::endl;
			assert( false );
			return false;
		}

		std::memset( flowfields[group].list, 0, nrNodes * sizeof( glm::vec3 ) );

		// find out what node we want to go to...
		int node = -1;
		for( int i = 0; i < nrNodes; i++ )
		{
			if( CheckPointInsideNode( point, i ) )
			{
				node = i;
				break;
			}	
		}

		if( node < 0 )
			return false;

		std::vector< TraversalData  > prioList;

		// rig first node...		
		for( int i = 0; i < 4; i++ )
		{
			int linksTo = nodes[ node ].corners[i].linksTo;
			if( linksTo >= 0 )
				prioList.push_back( TraversalData( linksTo, nodes[ node ].corners[i].linksToEdge, -5.0f ) );
		}
		std::sort( prioList.begin(), prioList.end(), sortingFunction );

		bool* visited = Core::world.m_frameHeap.NewPODArray<bool>( nrNodes );
		std::memset( visited, false, sizeof(bool) * nrNodes );

		// run algorithm
		while( prioList.size() != 0 )
		{
			Core::NavigationMesh::Node& current = nodes[ prioList[0].node ];
			
			glm::vec3 mid;			

			{
				int ii = prioList[0].entryEdge * 2;
				int oo = ( ii + 2 ) % 8;
				glm::vec3 lineStart = glm::vec3( current.points[ ii], 0.0f, current.points[ ii + 1 ] );
				glm::vec3 lineEnd	= glm::vec3( current.points[ oo ], 0.0f, current.points[ oo + 1 ] );
				mid = lineStart + (( lineEnd - lineStart ) * 0.5f );				
			}

			if( !visited[ prioList[0].node ] )
			{
				// for all corners of the current node
				for( int i = 0; i < 4; i++ )
				{
					if( current.corners[i].linksTo < 0 )
						continue;

					if( prioList.size() == 0 )
						break;

					glm::vec3 otherMid;

					{
						int ii = i * 2;
						int oo = ( ii + 2 ) % 8;
						glm::vec3 lineStart = glm::vec3( current.points[ ii], 0.0f, current.points[ ii + 1 ] );
						glm::vec3 lineEnd	= glm::vec3( current.points[ oo ], 0.0f, current.points[ oo + 1 ] );
						otherMid = lineStart + (( lineEnd - lineStart ) * 0.5f );
					}

					// distance to the next node
					float dist = glm::distance( otherMid, mid );
				
					// check so not the entry edge...
					if( i != prioList[0].entryEdge && !visited[ current.corners[i].linksTo ] )
					{
						// otherwise add new node to priolist
						prioList.push_back( TraversalData( current.corners[i].linksTo, current.corners[i].linksToEdge, dist + prioList[0].entryDistance ));
					}

				}
			}

			flowfields[group].list[ prioList[0].node ] = mid;

			visited[ prioList[0].node ] = true;
			prioList.erase( prioList.begin() );
			std::sort( prioList.begin(), prioList.end(), sortingFunction );

		}

		return true;
	}

}