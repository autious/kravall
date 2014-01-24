#include "NavigationMesh.hpp"
#include <World.hpp>

#include <algorithm>
#include <logger/Logger.hpp>

namespace Core
{
	struct TraversalData
	{
		TraversalData( int node, int entryEdge, float entryDistance, int parentNode ) 
		: node(node), entryEdge(entryEdge), entryDistance(entryDistance), parentNode(parentNode) {}

		int parentNode;
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

		// reset group metadata...
		flowfields[group].goal[ 0 ] = FLT_MAX;
		flowfields[group].goal[ 1 ] = FLT_MAX;
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


		m_flowfields[group].goal[ 0 ] = point.x;
		m_flowfields[group].goal[ 1 ] = point.z;


		std::vector< TraversalData > prioList;

		// rig first node...		
		for( int i = 0; i < 4; i++ )
		{
			int linksTo = nodes[ node ].corners[i].linksTo;
			if( linksTo >= 0 )
				prioList.push_back( TraversalData( linksTo, nodes[ node ].corners[i].linksToEdge, -5.0f, -1 ) );
		}
		std::sort( prioList.begin(), prioList.end(), sortingFunction );

		bool* visited = Core::world.m_frameHeap.NewPODArray<bool>( nrNodes );
		std::memset( visited, false, sizeof(bool) * nrNodes );

		float* distances = Core::world.m_frameHeap.NewPODArray<float>( m_nrNodes );
		std::memset( distances, 0.0f, sizeof(float) * m_nrNodes );


		// run algorithm
		while( prioList.size() != 0 )
		{
			Core::NavigationMesh::Node& current = nodes[ prioList[0].node ];
			
			// redundancy check, if sorting is perfect, this should never have any effect
			// note; this block kills of added m_nodes that are already visited. this is vital functionality.
			if( visited[ prioList[0].node ] )
			{
				if( prioList[0].node == 4 )
					int o = 0;

				float Adist = prioList[0].entryDistance;
				float Bdist = distances[ prioList[0].node ];

				if( prioList[0].entryDistance < distances[ prioList[0].node ] ) 
				{
					LOG_ERROR << __FILE__ << "   " << __FUNCTION__ << "   this should not happen! " << std::endl;
					assert(false);
				}

				prioList.erase( prioList.begin() );
				std::sort( prioList.begin(), prioList.end(), sortingFunction );
				continue;
			}
			
			// calculate entrypoint for this node... this could probablly be read from the already calculated flowfield
			// left here for easier bug-tracking...
			glm::vec3 mid;			
			int tt = prioList[0].entryEdge * 2;
			int qq = ( tt + 2 ) % 8;
			glm::vec3 lineStart = glm::vec3( current.points[ tt ], 0.0f, current.points[ tt + 1 ] );
			glm::vec3 lineEnd	= glm::vec3( current.points[ qq ], 0.0f, current.points[ qq + 1 ] );
			mid = lineStart + (( lineEnd - lineStart ) * 0.5f );				


			if( !visited[ prioList[0].node ] )
			{
				if( prioList.size() != 0 )
				{
					// for all corners of the current node
					for( int i = 0; i < 4; i++ )
					{
						if( current.corners[i].linksTo < 0 )
							continue;

						// calculate midpoint for the outgoing edge...
						glm::vec3 otherMid;
						int ii = i * 2;
						int oo = ( ii + 2 ) % 8;
						glm::vec3 lineStart = glm::vec3( current.points[ ii], 0.0f, current.points[ ii + 1 ] );
						glm::vec3 lineEnd	= glm::vec3( current.points[ oo ], 0.0f, current.points[ oo + 1 ] );
						otherMid = lineStart + (( lineEnd - lineStart ) * 0.5f );

						// distance to the next node
						float dist = glm::distance( otherMid, mid );

						//if( prioList[0] )
				
						// check so not the entry edge, otherwise add new node to priolist
						if( i != prioList[0].entryEdge )
						{
							
							prioList.push_back( TraversalData( current.corners[i].linksTo, current.corners[i].linksToEdge, dist + prioList[0].entryDistance, prioList[0].node ));
						}
					}
				}
			}

			if( prioList[0].node == 8 )
				int pp = 0;

			// calculate mid point of the entry edge in parent node, this makes the path a bit more flowing
			glm::vec3 parentMid;
			int parentNode = current.corners[ prioList[0].entryEdge ].linksTo;
			int parentEntryEdge = m_flowfields[group].edges[ parentNode ];		
			int ii = parentEntryEdge * 2;
			int oo = ( ii + 2 ) % 8;			
			glm::vec3 startOfLine = glm::vec3( m_nodes[ parentNode ].points[ ii ], 0.0f, m_nodes[ parentNode ].points[ ii + 1 ] );
			glm::vec3 endOfLine	= glm::vec3( m_nodes[ parentNode ].points[ oo ], 0.0f, m_nodes[ parentNode ].points[ oo + 1 ] );
			parentMid = startOfLine + (( endOfLine - startOfLine ) * 0.5f );	

			// assign values to the flowfield...
			m_flowfields[group].list[ prioList[0].node ] = parentMid;
			m_flowfields[group].edges[ prioList[0].node ] = prioList[0].entryEdge;
			distances[ prioList[0].node ] = prioList[0].entryDistance;

			// set metadata for calculation...
			visited[ prioList[0].node ] = true;
			prioList.erase( prioList.begin() );
			std::sort( prioList.begin(), prioList.end(), sortingFunction );
		}

		return true;
	}
}
