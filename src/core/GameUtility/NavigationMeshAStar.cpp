#include "NavigationMeshAStar.hpp"
#include <GameUtility/NavigationMesh.hpp>
#include <World.hpp>

#include <algorithm>
#include <logger/Logger.hpp>

#define NO_LINE_COLLISION -0.5f
#define NO_LINE_COLLISION_VALUE -1.0f

namespace Core
{

	struct LineCheckReturnStruct
	{
		int node, edge;
		float t;
	};

	
	void CheckLine( glm::vec3 start, glm::vec3 goal, Core::NavigationMesh* instance, int startNode, LineCheckReturnStruct& result )
	{
		result.t = NO_LINE_COLLISION_VALUE;
		result.node = startNode;
		
		bool* visited = Core::world.m_frameHeap.NewPODArray<bool>( instance->nrNodes );
		std::memset( visited, false, sizeof(bool) * instance->nrNodes );

		std::vector<int> nodeList;
		nodeList.reserve( instance->nrNodes );

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
		
				float A = glm::determinant( glm::mat2x2( start.x, start.z, goal.x, goal.z ) );
				float B = glm::determinant( glm::mat2x2( lineStart.x, lineStart.z, lineEnd.x, lineEnd.z ) );
				float divider = glm::determinant( glm::mat2x2( start.x - goal.x, start.z - goal.z, lineStart.x - lineEnd.x, lineStart.z - lineEnd.z  ) );
		
				float intersectionX = glm::determinant( glm::mat2x2( A, start.x - goal.x , B, lineStart.x - lineEnd.x ) ) / divider;
				float intersectionZ = glm::determinant( glm::mat2x2( A, start.z - goal.z , B, lineStart.z - lineEnd.z ) ) / divider;
		
				glm::vec3 hit = glm::vec3( intersectionX, 0.0f, intersectionZ );
		
				float alongLine = glm::dot( hit - lineStart, (lineEnd - lineStart) * current->corners[p].inverseLength );
				if( alongLine > 0 && alongLine < current->corners[p].length )
				{
					glm::vec3 hitPos = lineStart + alongLine * (lineEnd - lineStart) * current->corners[p].inverseLength;
					float t = glm::distance( hitPos, start );

					if( ( result.t > t ||  result.t < 0 ) && t > 0.01f && glm::dot( goal - start, hitPos - start ) > 0 && t < distanceToTarget - 0.01f )
					{
						result.node = nodeList[head];
						result.edge = p;
						result.t = t;
					}
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
	}

	bool PathFinder::CheckLineVsNavMesh( glm::vec3 from, glm::vec3 to )
	{
		Core::NavigationMesh* instance = Core::GetNavigationMesh();
		if( !instance )
			return false;

		// get starting node...
		int startNode = -1;
		for( int i = 0; i < instance->nrNodes; i++ )
		{
			if( instance->CheckPointInsideNode( from, i ) )
			{
				startNode = i;
				break;
			}
		}

		// start is outside...
		if( startNode < 0 )
			return false;

		LineCheckReturnStruct result;
		CheckLine( from, to, instance, startNode, result );

		if( result.t < NO_LINE_COLLISION )
			return true;
		return false;
	}

	bool PathFinder::CheckLineVsNavMesh( glm::vec3 from, glm::vec3 to, int startNode )
	{
		Core::NavigationMesh* instance = Core::GetNavigationMesh();
		if( !instance || startNode < 0 )
			return false;

		if( instance->CheckPointInsideNode( from, startNode ) )
		{
			LineCheckReturnStruct result;
			CheckLine( from, to, instance, startNode, result );
			if( result.t < NO_LINE_COLLISION )
				return true;
		}


		return false;
	}







	/*

	//struct PathComponent
	//{
	//	int m_nrNodes;
	//	float m_nodes[ 2 * 50 ];
	//};

	//struct PathElement
	//{
	//	PathElement() {}
	//
	//	PathElement( glm::vec3 start, glm::vec3 goal, int startNode ) 
	//		: origin( start ), end( goal ), startNode( startNode )
	//	{
	//	}
	//
	//	glm::vec3 origin;
	//	glm::vec3 end;
	//	int startNode;
	//};



#define SHOWDEBUG

#ifdef SHOWDEBUG
#define DEB
#endif

	PathComponent PathFinder::GetPath( glm::vec3 start, glm::vec3 goal )
	{
		PathComponent path;
		path.m_nrNodes = 0;

		Core::NavigationMesh* instance = Core::GetNavigationMesh();
		if( !instance )
			return path;

		// get starting node...
		int startNode = -1;
		for( int i = 0; i < instance->m_nrNodes; i++ )
		{
			if( instance->CheckPointInsideNode( start, i ) )
			{
				startNode = i;
				break;
			}
		}

		// start is outside...
		if( startNode < 0 )
			return path;

#ifdef DEB
		GFX::Debug::DrawSphere( goal, 5.0f, GFXColor( 0, 1, 1, 1 ), false );
		GFX::Debug::DrawSphere( start, 2.0f, GFXColor( 1, 1, 1, 1 ), false );
#endif
		// TO DO : check goal node, and if none calculate closes position on navMesh to goal

		#define MAX_WAYPOINTS_IN_LIST 500

		PathElement* wayPoints = (PathElement*)Core::world.m_frameHeap.NewPODArray< char >( MAX_WAYPOINTS_IN_LIST * sizeof( PathElement ) );
		
		// rig first node...
		wayPoints[0] = PathElement( start, goal, startNode );
		int head = 1;
		int tail = -1;

		int debugCounter = 0;

		while( head > tail )
		{
			tail++;

#ifdef DEB
			debugCounter++;
			if( debugCounter == 40 )
				//break;
				int o = 0;
#endif

			LineCheckReturnByReference result;
			CheckLine( wayPoints[tail].origin, wayPoints[tail].end, instance, wayPoints[tail].startNode, result );

#ifdef DEB
			GFX::Debug::DrawLine( 
					wayPoints[tail].origin,
					wayPoints[tail].origin + glm::normalize( wayPoints[tail].end - wayPoints[tail].origin ) * result.t, GFXColor( 1, 0, 1, 1 ), false );
			
			GFX::Debug::DrawSphere( wayPoints[tail].origin + glm::normalize( wayPoints[tail].end - wayPoints[tail].origin ) * result.t, 2.0f, GFXColor( 1, 1, 0, 1 ), false );

			int node = 8;
			int size = instance->m_nodes[node].corners[3].length < 0 ? 3 : 4;
			//for( int oo = 0; oo < size; oo++ )
			//{
			//	glm::vec3 point = glm::vec3(
			//	instance->m_nodes[node].points[oo * 2], 0, instance->m_nodes[node].points[oo * 2 + 1] ); 
			//
			//	GFX::Debug::DrawSphere( point, 4.0f, GFXColor( 1, 1, 0.5, 1 ), false );
			//}
#endif

			if( result.t >= 0 )
			{

				// if closest hit is backside of edge, we are shooting from a corner.
				// walk sideways along the walls and shoot from the next node...
				float* normal = instance->m_nodes[result.node].corners[result.edge].normal;
				if( glm::dot( wayPoints[tail].end - wayPoints[tail].origin, glm::vec3( normal[0], normal[1], normal[2] ) ) > 0 )
				{
					glm::vec2 currentCorner = glm::vec2( wayPoints[tail].origin.x, wayPoints[tail].origin.z );

					// find current corner in current node...
					int nrCorners = 4;
					if( instance->m_nodes[wayPoints[tail].startNode].corners[3].length < 0 )
						nrCorners = 3;

					for( int v = 0; v < nrCorners; ++v )
					{						
						int otherCurrent = v * 2;
						glm::vec2 cornerPos = glm::vec2( 
							instance->m_nodes[wayPoints[tail].startNode].points[ otherCurrent ], 
							instance->m_nodes[wayPoints[tail].startNode].points[ otherCurrent + 1 ] );
						
						int tempNextCornerNode = instance->m_nodes[wayPoints[tail].startNode].corners[v].cornerConnectsToNode;
						if( tempNextCornerNode >= 0 )
						{
							//glm::vec3 point = glm::vec3(
							//instance->m_nodes[tempNextCornerNode].points[0], 0, instance->m_nodes[tempNextCornerNode].points[1] ); 
							//
							//GFX::Debug::DrawSphere( point, 4.0f, GFXColor( 1, 0, 0, 1 ), false );
						}


						if( glm::dot( cornerPos - currentCorner, cornerPos - currentCorner ) < 0.05f )
						{
							//glm::vec3 point = glm::vec3(
							//instance->m_nodes[wayPoints[tail].startNode].points[0], 0, instance->m_nodes[wayPoints[tail].startNode].points[1] ); 

							//GFX::Debug::DrawSphere( glm::vec3( cornerPos.x, 0.0f, cornerPos.y ), 4.0f, GFXColor( 1, 0, 0, 1 ), false );

							//#define NAVMESH_NO_CONNECTING_CORNERS -1
							//#define NAVMESH_CONCAVE_CORNER_NODE -2

							// starting node for new ray
							int nextCornerNode = instance->m_nodes[wayPoints[tail].startNode].corners[v].cornerConnectsToNode;
							if( nextCornerNode == NAVMESH_CONCAVE_CORNER_NODE )
							{
								int prevCorner = (v + nrCorners - 1) % nrCorners;
								int nextCorner = (v+1) % nrCorners;
								
								glm::vec3 corner;
								
								corner = glm::vec3( 
									instance->m_nodes[ wayPoints[tail].startNode ].points[ prevCorner * 2 ], 
									0.0f, 
									instance->m_nodes[ wayPoints[tail].startNode ].points[ prevCorner * 2 + 1 ]);
								wayPoints[head++] = PathElement( corner, wayPoints[tail].end, nextCornerNode );

								
								wayPoints[head++] = PathElement( nextCornerPos, wayPoints[tail].end, nextCornerNode );

								continue;
							}
#ifdef DEB
							else if( nextCornerNode == NAVMESH_NO_CONNECTING_CORNERS )
							{
								// this should never happen?
								int breakpoint = 0;
							}
#endif


							// get start position for new ray...
							int corner = instance->m_nodes[wayPoints[tail].startNode].corners[v].cornerConnectsToCorner;
							glm::vec3 nextCornerPos = glm::vec3( 
								instance->m_nodes[nextCornerNode].points[ corner * 2 ],
								0.0f, 
								instance->m_nodes[nextCornerNode].points[ corner * 2 + 1 ] );
						
							GFX::Debug::DrawLine( nextCornerPos, wayPoints[tail].origin, GFXColor( 1, 1, 0, 1 ), false );
							

							wayPoints[head++] = PathElement( nextCornerPos, wayPoints[tail].end, nextCornerNode );
							
							break;
						}
					}

					continue;
				}

				//GFX::Debug::DrawLine( 
				//	list[0].origin,
				//	list[0].origin + glm::normalize( list[0].end - list[0].origin ) * result.t, GFXColor( 1, 0, 1, 1 ), false );

				// corners of hit edge...
				int ii = result.edge * 2;
				int oo = ( ii + 2 ) % 8;
				glm::vec3 lineStart = glm::vec3( instance->m_nodes[result.node].points[ ii ], 0, instance->m_nodes[result.node].points[ ii + 1 ] );
				glm::vec3 lineEnd	= glm::vec3( instance->m_nodes[result.node].points[ oo ], 0, instance->m_nodes[result.node].points[ oo + 1 ] );
				
				LineCheckReturnByReference resultA;
				CheckLine( lineStart, wayPoints[tail].origin, instance, result.node, resultA );
				if( resultA.t > 0.0f )
				{	
					//GFX::Debug::DrawSphere( lineStart, 4.0f, GFXColor( 1, 0, 0, 1 ), false );
				}
				else
				{
#ifdef DEB
					GFX::Debug::DrawSphere( lineStart, 5.0f, GFXColor( 0, 1, 0, 1 ), false );
					
					GFX::Debug::DrawLine( 
						lineStart,
						wayPoints[tail].origin, GFXColor( 0, 0, 1, 1 ), false );
#endif

					wayPoints[head++] = PathElement( lineStart, wayPoints[tail].end, resultA.node );
				}

				LineCheckReturnByReference resultB;
				CheckLine( lineEnd, wayPoints[tail].origin, instance, result.node, resultB );
				if( resultB.t > 0.0f )
				{
					//GFX::Debug::DrawSphere( lineEnd, 3.0f, GFXColor( 1, 0, 0, 1 ), false );
				}
				else
				{
#ifdef DEB
					GFX::Debug::DrawSphere( lineEnd, 5.0f, GFXColor( 0, 1, 0, 1 ), false );
					
					GFX::Debug::DrawLine( 
						lineEnd,
						wayPoints[tail].origin, GFXColor( 0, 0, 1, 1 ), false );
#endif

					wayPoints[head++] = PathElement( lineEnd, wayPoints[tail].end, resultB.node );
				}


							
				//GFX::Debug::DrawSphere( lineStart, 5.0f, GFXColor( 0, 1, 0, 1 ), false );
				//GFX::Debug::DrawSphere( lineEnd, 5.0f, GFXColor( 1, 0, 0, 1 ), false );

			
				

			}
			else
			{
#ifdef DEB
				GFX::Debug::DrawLine( 
						wayPoints[tail].end,
						wayPoints[tail].origin, GFXColor( 0.25f, 0.5f, 1, 1 ), false );
#endif
				break;
			}

			//list.erase( list.begin() );

		}



		// cast ray
		// get corners
		// recursivly check direct paths to corners each corner


		// lägg till memcpy för tail > 100...
		// lägg till sortering...
		// lägg till rekursions-checken...
		// fixa datastruktur så att man kan få ut alla noderna sen...
		// lägg till corner-smoothing...

		//LOG_DEBUG << tail << std::endl;


		return path;

	}
}

*/
}

//glm::vec3 lineStart = glm::vec3( instance->m_nodes[result.node].points[ result.edge * 2 ], 0, instance->m_nodes[result.node].points[ result.edge * 2 + 1 ] );
//glm::vec3 lineEnd = glm::vec3( instance->m_nodes[result.node].points[ result.edge * 2 + 2 ], 0, instance->m_nodes[result.node].points[ result.edge * 2 + 3 ] );
//
//GFX::Debug::DrawLine( 
//	start,
//	start + glm::normalize( goal - start ) * result.t, GFXColor( 1, 0, 1, 1 ), false  );



// working line intersection test...

//// for every node
//for( int i = 0; i < instance->m_nrNodes; i++ )
//{
//	// for every line
//	for( int p = 0; p < 4; p++ )
//	{
//		// check if triangle or non-collidable line...
//		if( instance->m_nodes[i].corners[p].length < 0 || instance->m_nodes[i].corners[p].linksTo >= 0 )
//			continue;
//
//		int ii = p * 2;
//		int oo = ( ii + 2 ) % 8;
//		glm::vec3 lineStart = glm::vec3( instance->m_nodes[i].points[ ii ], 0, instance->m_nodes[i].points[ ii + 1 ] );
//		glm::vec3 lineEnd	= glm::vec3( instance->m_nodes[i].points[ oo ], 0, instance->m_nodes[i].points[ oo + 1 ] );
//
//		float A = glm::determinant( glm::mat2x2( start.x, start.z, goal.x, goal.z ) );
//		float B = glm::determinant( glm::mat2x2( lineStart.x, lineStart.z, lineEnd.x, lineEnd.z ) );
//		float divider = glm::determinant( glm::mat2x2( start.x - goal.x, start.z - goal.z, lineStart.x - lineEnd.x, lineStart.z - lineEnd.z  ) );
//
//		float intersectionX = glm::determinant( glm::mat2x2( A, start.x - goal.x , B, lineStart.x - lineEnd.x ) ) / divider;
//		float intersectionZ = glm::determinant( glm::mat2x2( A, start.z - goal.z , B, lineStart.z - lineEnd.z ) ) / divider;
//
//		glm::vec3 hit = glm::vec3( intersectionX, 0.0f, intersectionZ );
//
//		float alongLine = glm::dot( hit - lineStart, (lineEnd - lineStart) * instance->m_nodes[i].corners[p].inverseLength );
//		if( alongLine > 0 && alongLine < instance->m_nodes[i].corners[p].length )
//		{
//
//			glm::vec3 hitPos = lineStart + alongLine * (lineEnd - lineStart) * instance->m_nodes[i].corners[p].inverseLength;
//			float t = glm::distance( hitPos, start );
//
//			if( result.t > t || result.t < 0 )
//			{
//				result.node = i;
//				result.edge = p;
//				result.t = t;
//			}
//		}
//
//		//float B = glm::determinant( glm::mat2x2( start.x, 1.0f, goal.x, 1.0f ) ); 
//		//float D = glm::determinant( glm::mat2x2( lineStart.x, 1.0f, lineEnd.x, 1.0f ) ); 
//
//		//glm::vec2 line = lineEnd - lineStart;
//	}
//}
