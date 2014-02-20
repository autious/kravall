#include "NavigationMesh.hpp"
#include <World.hpp>

#include <algorithm>
#include <logger/Logger.hpp>

#define EDGE_HEAT_DENSITY_THREASHOLD_FOR_BLOCKING 5.0f

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


	bool NavigationMesh::CalculateFlowfieldForGroup(const glm::vec3& point, int group )
	{
		if( group >= maxFlowfields )
		{
			LOG_FATAL << "Trying to calculate flowfield for group id above max nr groups, in function \"CalculateFlowfieldForGroup\"" << std::endl;
			assert( false );
			return false;
		}

		// reset group metadata...
		flowfields[group].goal[ 0 ] = std::numeric_limits<float>::max();
		flowfields[group].goal[ 1 ] = std::numeric_limits<float>::max();
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

		flowfields[group].goal[ 0 ] = point.x;
		flowfields[group].goal[ 1 ] = point.z;


		std::vector< TraversalData > prioList;

		// rig first node...		
		for( int i = 0; i < 4; i++ )
		{
			int linksTo = nodes[ node ].corners[i].linksTo;
			if( linksTo >= 0 && flowfields[ group ].blocked[ node * 4 + i] < EDGE_HEAT_DENSITY_THREASHOLD_FOR_BLOCKING 
				&& flowfields[ group ].blocked[ nodes[ node ].corners[i].linksTo * 4 + nodes[ node ].corners[i].linksToEdge ] < EDGE_HEAT_DENSITY_THREASHOLD_FOR_BLOCKING )
				prioList.push_back( TraversalData( linksTo, nodes[ node ].corners[i].linksToEdge, -5.0f, -1 ) );
		}
		std::sort( prioList.begin(), prioList.end(), sortingFunction );

		if( prioList.size() == 0 )
		{
			flowfields[group].goal[ 0 ] = std::numeric_limits< float >::max();
			return false;
		}

		// allocate and initialize data
		bool* visited = Core::world.m_frameHeap.NewPODArray<bool>( nrNodes );
		if( visited == nullptr )
		{
			LOG_FATAL << "Memory allocation failed when calculating flowfield!" << std::endl;
			return false;
		}
		std::memset( visited, false, sizeof(bool) * nrNodes );

		float* distances = Core::world.m_frameHeap.NewPODArray<float>( nrNodes );
		if( distances == nullptr )
		{
			LOG_FATAL << "Memory allocation failed when calculating flowfield!" << std::endl;
			return false;
		}
		std::memset( distances, 0, sizeof(float) * nrNodes );

		glm::vec3* points = (glm::vec3*)Core::world.m_frameHeap.NewPODArray<float>( nrNodes * sizeof( glm::vec3 ) );
		if( points == nullptr )
		{
			LOG_FATAL << "Memory allocation failed when calculating flowfield!" << std::endl;
			return false;
		}
		std::memset( points, 0, nrNodes * sizeof( glm::vec3 ) );


		// run algorithm
		while( prioList.size() != 0 )
		{
			Core::NavigationMesh::Node& current = nodes[ prioList[0].node ];
			
			// redundancy check, if sorting is perfect, this should never have any effect
			// note; this block kills of added nodes that are already visited. this is vital functionality.
			if( visited[ prioList[0].node ] )
			{
				if( prioList[0].entryDistance < distances[ prioList[0].node ] ) 
				{
					LOG_ERROR << __FILE__ << "   " << __FUNCTION__ << " -  This should not happen! " << std::endl;
					assert(false);
				}

				prioList.erase( prioList.begin() );
				std::sort( prioList.begin(), prioList.end(), sortingFunction );
				continue;
			}
			
			// calculate entrypoint for this node... 
			/*
				this should probobly be altered to use the calculated entry point from previous nodes ... ? 
				parent entry node is no longer always in the middle.
			*/
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
						// the later half of this if will not be run if the first one is true, henc no explotion... 
						//if( current.corners[i].linksTo < 0 || nodes[ current.corners[ i ].linksTo ].blocked[ current.corners[ i ].linksToEdge ] )
						if( current.corners[i].linksTo < 0 || flowfields[ group ].blocked[ 4 * prioList[0].node + i ] > EDGE_HEAT_DENSITY_THREASHOLD_FOR_BLOCKING || 
							flowfields[ group ].blocked[ current.corners[i].linksTo * 4 + current.corners[i].linksToEdge ] > EDGE_HEAT_DENSITY_THREASHOLD_FOR_BLOCKING )
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
				
						// check so not the entry edge, otherwise add new node to priolist
						if( i != prioList[0].entryEdge )
						{
							
							prioList.push_back( TraversalData( current.corners[i].linksTo, current.corners[i].linksToEdge, dist + prioList[0].entryDistance, prioList[0].node ));
						}
					}
				}
			}

			if( prioList[0].node == 4 )
				int pp = 0;
				

			glm::vec3 parentMidLine;
			glm::vec3 ownMidLine;
			glm::vec3 ownLineStart;
			glm::vec3 ownLineEnd;
			
			{
				int ownNode = prioList[0].node;
				int ownEntryEdge = prioList[0].entryEdge;
				int ii = ownEntryEdge * 2;
				int oo = ( ii + 2 ) % 8;
				ownLineStart = glm::vec3( nodes[ ownNode ].points[ ii ], 0.0f, nodes[ ownNode ].points[ ii + 1 ] );
				ownLineEnd	= glm::vec3( nodes[ ownNode ].points[ oo ], 0.0f, nodes[ ownNode ].points[ oo + 1 ] );

				ownMidLine = ownLineStart + ( ownLineEnd - ownLineStart ) * 0.5f;
			}	

			{
				int parentNode = current.corners[ prioList[0].entryEdge ].linksTo;
				int parentEntryEdge = flowfields[group].edges[ parentNode ];
				int ii = parentEntryEdge * 2;
				int oo = ( ii + 2 ) % 8;
				glm::vec3 startOfLine = glm::vec3( nodes[ parentNode ].points[ ii ], 0.0f, nodes[ parentNode ].points[ ii + 1 ] );
				glm::vec3 endOfLine	= glm::vec3( nodes[ parentNode ].points[ oo ], 0.0f, nodes[ parentNode ].points[ oo + 1 ] );

				parentMidLine = startOfLine + ( endOfLine - startOfLine ) * 0.5f;
			}


			// first node condition...
			if( prioList[0].parentNode < 0 )
			{
				flowfields[group].list[ prioList[0].node ] = ownMidLine;
				flowfields[group].edges[ prioList[0].node ] = prioList[0].entryEdge;
				distances[ prioList[0].node ] = prioList[0].entryDistance;
				points[ prioList[0].node ] = ownMidLine;

				// set metadata for calculation...
				visited[ prioList[0].node ] = true;
				prioList.erase( prioList.begin() );
				std::sort( prioList.begin(), prioList.end(), sortingFunction );

				continue;
			}



			glm::vec3 positions[3];
			positions[0] = ownLineStart + ( ownLineEnd - ownLineStart ) * 0.25f;
			positions[1] = ownMidLine;
			positions[2] = ownLineEnd + ( ownLineStart - ownLineEnd ) * 0.25f;

			glm::vec3 test = points[ prioList[0].parentNode ];

			// sort...
			std::sort( &positions[0], &positions[3], 
				[&test]( const glm::vec3& A, const glm::vec3& B )
			{ 
				if( glm::distance(A, test) < glm::distance( B, test ) ) 
					return true; 
				return false; 
			} );


			
			points[ prioList[0].node ] = positions[0];
			flowfields[group].list[ prioList[0].node ] = points[ prioList[0].parentNode ];
			flowfields[group].edges[ prioList[0].node ] = prioList[0].entryEdge;

			// utility
			distances[ prioList[0].node ] = prioList[0].entryDistance;

			// set metadata for calculation...
			visited[ prioList[0].node ] = true;
			prioList.erase( prioList.begin() );
			std::sort( prioList.begin(), prioList.end(), sortingFunction );
		}

		return true;
	}















	/// AStar functionality

	struct AStarData
	{
		AStarData( int node, int entryEdge, int parentNode, float entryDistance, float distanceToGoal ) 
		: node(node), entryEdge(entryEdge), parentNode(parentNode), entryDistance(entryDistance), distanceToGoal( distanceToGoal ) {}
	
		int parentNode;
		int node;
		int entryEdge;
		float entryDistance;
		float distanceToGoal;
	};

	bool AstarSortingFunction( const AStarData& a1, const AStarData& a2 )
	{
		if( a1.entryDistance + a1.distanceToGoal < a2.entryDistance + a2.distanceToGoal)
				return true;
			return false;
	}

	bool Core::NavigationMesh::AllocateFrameMemoryForAstar()
	{
		// allocate memory
		visited = Core::world.m_frameHeap.NewPODArray<bool>( nrNodes );
		if( visited == nullptr ) 
		{ LOG_FATAL << "Memory allocation failed when calculating flowfield!" << std::endl; return false; }
				
		distances = Core::world.m_frameHeap.NewPODArray<float>( nrNodes );
		if( distances == nullptr )
		{ LOG_FATAL << "Memory allocation failed when calculating flowfield!" << std::endl; return false; }
		
		points = (glm::vec3*)Core::world.m_frameHeap.NewPODArray<float>( nrNodes * sizeof( glm::vec3 ) );
		if( points == nullptr )
		{ LOG_FATAL << "Memory allocation failed when calculating flowfield!" << std::endl; return false; }

		tempField.list = (glm::vec3*)Core::world.m_frameHeap.NewPODArray<float>( nrNodes * sizeof(glm::vec3) );
		if( tempField.list == nullptr )
		{ LOG_FATAL << "Memory allocation failed when calculating flowfield!" << std::endl; return false; }

		tempField.edges = Core::world.m_frameHeap.NewPODArray<int>( nrNodes );
		if( tempField.list == nullptr )
		{ LOG_FATAL << "Memory allocation failed when calculating flowfield!" << std::endl; return false; }

		return true;
	}

	Core::PathData Core::NavigationMesh::CalculateShortPath( int ownNode, glm::vec3 ownPosition, int otherNode, glm::vec3 otherPosition )
	{
		std::vector< AStarData > prioList;

		// init memory...
		std::memset( visited, false, sizeof(bool) * nrNodes );
		std::memset( distances, 0, sizeof(float) * nrNodes );
		std::memset( points, 0, sizeof( glm::vec3 ) * nrNodes );
		std::memset( tempField.list, 0, sizeof(glm::vec3) * nrNodes );
		std::memset( tempField.edges, 0, sizeof(int) * nrNodes );


		// rig first node...		
		for( int i = 0; i < 4; i++ )
		{
			int linksTo = nodes[ otherNode ].corners[i].linksTo;
			if( linksTo < 0 || nodes[ otherNode ].corners[i].length < 0 )
				continue;

			int tt = i * 2;
			int qq = ( tt + 2 ) % 8;
			glm::vec3 lineStart = glm::vec3( nodes[ otherNode ].points[ tt ], 0.0f, nodes[ otherNode ].points[ tt + 1 ] );
			glm::vec3 lineEnd	= glm::vec3( nodes[ otherNode ].points[ qq ], 0.0f, nodes[ otherNode ].points[ qq + 1 ] );
			glm::vec3 mid = lineStart + (( lineEnd - lineStart ) * 0.5f );

			float dist = glm::distance( otherPosition, mid );
			float distToGoal = glm::distance( ownPosition, mid );

			prioList.push_back( AStarData( linksTo, nodes[ otherNode ].corners[i].linksToEdge, -1, dist, distToGoal ) );
		}
		std::sort( prioList.begin(), prioList.end(), AstarSortingFunction );

		glm::vec3 aa = glm::vec3( 1, 0, 1 );
		float dist = glm::dot( aa, aa );

		// run algorithm
		while( prioList.size() != 0 )
		{
			Core::NavigationMesh::Node& current = nodes[ prioList[0].node ];

			if( prioList[0].node == ownNode )
			{
				// if the goal in the adjacent, push special case to priolist...
				if( prioList[0].parentNode < 0 )
				{
					glm::vec3 positions[3];
					positions[0] = nodes[ownNode].GetLineStart( prioList[0].entryEdge ) + 
						( nodes[ownNode].GetLineEnd( prioList[0].entryEdge ) - 
						nodes[ownNode].GetLineStart( prioList[0].entryEdge ) ) * 0.25f;

					positions[1] = nodes[ownNode].GetMidPoint( prioList[0].entryEdge );
					
					positions[2] = nodes[ownNode].GetLineEnd( prioList[0].entryEdge ) + 
						( nodes[ownNode].GetLineStart( prioList[0].entryEdge ) - 
						nodes[ownNode].GetLineEnd( prioList[0].entryEdge ) ) * 0.25f;

					glm::vec3 posFromParent = ownPosition;

					// sort...
					std::sort( &positions[0], &positions[3], 
						[&posFromParent]( const glm::vec3& A, const glm::vec3& B )
					{ 
						if( glm::distance(A, posFromParent) < glm::distance( B, posFromParent ) ) 
							return true; 
						return false; 
					} );

					int o = 0;

					return PathData( otherNode, prioList[0].entryEdge, positions[0] );
				}

				//if(  )
				//glm::vec3 otherMid = nodes[ prioList[0].node ].GetMidPoint( prioList[0].entryEdge );
				//
				//// distance to the next node
				//float dist = glm::distance( otherMid, mid );
				//prioList.push_back( 
				//				AStarData( current.corners[i].linksTo, current.corners[i].linksToEdge, prioList[0].node, 
				//				dist + prioList[0].entryDistance,
				//				glm::distance( mid, ownPosition ) ));
				break;
			}
			
			// redundancy check, if sorting is perfect, this should never have any effect
			// note; this block kills of added nodes that are already visited. this is vital functionality.
			if( visited[ prioList[0].node ] )
			{
				prioList.erase( prioList.begin() );
				std::sort( prioList.begin(), prioList.end(), AstarSortingFunction );
				continue;
			}
			
			// calculate entrypoint for this node... 
			/*
				this should probobly be altered to use the calculated entry point from previous nodes ... ? 
				parent entry node is no longer always in the middle.
			*/
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
						// the later half of this if will not be run if the first one is true, henc no explotion... 
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
				
						// check so not the entry edge, otherwise add new node to priolist
						if( i != prioList[0].entryEdge )
						{
							
							prioList.push_back( 
								AStarData( current.corners[i].linksTo, current.corners[i].linksToEdge, prioList[0].node, 
								dist + prioList[0].entryDistance,
								glm::distance( mid, ownPosition ) ));
						}
					}
				}
			}

			glm::vec3 parentMidLine;
			glm::vec3 ownMidLine;
			glm::vec3 ownLineStart;
			glm::vec3 ownLineEnd;
			
			{
				int ownNode = prioList[0].node;
				int ownEntryEdge = prioList[0].entryEdge;
				int ii = ownEntryEdge * 2;
				int oo = ( ii + 2 ) % 8;
				ownLineStart = glm::vec3( nodes[ ownNode ].points[ ii ], 0.0f, nodes[ ownNode ].points[ ii + 1 ] );
				ownLineEnd	= glm::vec3( nodes[ ownNode ].points[ oo ], 0.0f, nodes[ ownNode ].points[ oo + 1 ] );

				ownMidLine = ownLineStart + ( ownLineEnd - ownLineStart ) * 0.5f;
			}	

			{
				int parentNode = current.corners[ prioList[0].entryEdge ].linksTo;
				int parentEntryEdge = tempField.edges[ parentNode ];
				int ii = parentEntryEdge * 2;
				int oo = ( ii + 2 ) % 8;
				glm::vec3 startOfLine = glm::vec3( nodes[ parentNode ].points[ ii ], 0.0f, nodes[ parentNode ].points[ ii + 1 ] );
				glm::vec3 endOfLine	= glm::vec3( nodes[ parentNode ].points[ oo ], 0.0f, nodes[ parentNode ].points[ oo + 1 ] );

				parentMidLine = startOfLine + ( endOfLine - startOfLine ) * 0.5f;
			}


			// first node condition...
			if( prioList[0].parentNode < 0 )
			{
				tempField.list[ prioList[0].node ] = ownMidLine;
				tempField.edges[ prioList[0].node ] = prioList[0].entryEdge;
				distances[ prioList[0].node ] = prioList[0].entryDistance;
				points[ prioList[0].node ] = ownMidLine;

				// set metadata for calculation...
				visited[ prioList[0].node ] = true;
				prioList.erase( prioList.begin() );
				std::sort( prioList.begin(), prioList.end(), AstarSortingFunction );

				continue;
			}


			glm::vec3 positions[3];
			positions[0] = ownLineStart + ( ownLineEnd - ownLineStart ) * 0.25f;
			positions[1] = ownMidLine;
			positions[2] = ownLineEnd + ( ownLineStart - ownLineEnd ) * 0.25f;

			glm::vec3 posFromParent = points[ prioList[0].parentNode ];

			// sort...
			std::sort( &positions[0], &positions[3], 
				[&posFromParent]( const glm::vec3& A, const glm::vec3& B )
			{ 
				if( glm::distance(A, posFromParent) < glm::distance( B, posFromParent ) ) 
					return true; 
				return false; 
			} );
			
			points[ prioList[0].node ] = positions[0];
			tempField.list[ prioList[0].node ] = points[ prioList[0].parentNode ];
			tempField.edges[ prioList[0].node ] = prioList[0].entryEdge;

			// utility
			distances[ prioList[0].node ] = prioList[0].entryDistance;

			// set metadata for calculation...
			visited[ prioList[0].node ] = true;
			prioList.erase( prioList.begin() ); 
			std::sort( prioList.begin(), prioList.end(), AstarSortingFunction );
		}

		return PathData( prioList[0].node, prioList[0].entryEdge, points[ prioList[0].parentNode ] );
	}
}
