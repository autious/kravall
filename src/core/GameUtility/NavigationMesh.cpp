#include "NavigationMesh.hpp"
#include <World.hpp>

#include <fstream>
#include <logger/Logger.hpp>
#include <limits>
static bool drawNavigationMesh = false;

namespace Core
{
	NavigationMesh* mesh;
	struct NavigationMeshInitializer
	{
		NavigationMeshInitializer()
		{
			mesh = nullptr;
		}

	} navigationMeshInitializer;


	NavigationMesh* GetNavigationMesh()
	{
		return mesh;
	}

	void SetNavigationMesh( Core::NavigationMesh* newMesh )
	{
		mesh = newMesh;
	}

    int NavigationMesh::CreateGroup( int groupTeam )
    {
        if(nrUsedFlowfields >= maxFlowfields)
        {
            LOG_FATAL << "Flowfield array is out of indices" << std::endl;
        }

		flowfields[nrUsedFlowfields].blocked = Core::world.m_levelHeap.NewPODArray<float>( nrNodes * 4 );
        flowfields[nrUsedFlowfields].edges = Core::world.m_levelHeap.NewPODArray<int>( nrNodes );
		flowfields[nrUsedFlowfields].list = (glm::vec3*)(Core::world.m_levelHeap.NewPODArray<float>( 3 * nrNodes ));
		flowfields[nrUsedFlowfields].distanceToGoal = Core::world.m_levelHeap.NewPODArray<float>( nrNodes );
		flowfields[nrUsedFlowfields].deadNodes = Core::world.m_levelHeap.NewPODArray<bool>( nrNodes );

		std::memset( flowfields[nrUsedFlowfields].blocked, 0, nrNodes * sizeof(float) * 4 );
		std::memset( flowfields[nrUsedFlowfields].edges, 0, nrNodes * sizeof(int) );
		std::memset( flowfields[nrUsedFlowfields].list, 0, nrNodes * sizeof(glm::vec3) );
		std::memset( flowfields[nrUsedFlowfields].distanceToGoal, 0, nrNodes * sizeof(float) );
		std::memset( flowfields[nrUsedFlowfields].deadNodes, 0, nrNodes * sizeof(bool) );

		flowfields[nrUsedFlowfields].goal[ 0 ] = std::numeric_limits<float>::max();
		flowfields[nrUsedFlowfields].goal[ 1 ] = std::numeric_limits<float>::max();

		flowfields[ nrUsedFlowfields ].stuckTimer = 0.0f;
		flowfields[ nrUsedFlowfields ].timeSinceLastCheck = -1.0f;
				    				 
		flowfields[ nrUsedFlowfields ].recordedPosition[0] = 0.0f;
		flowfields[ nrUsedFlowfields ].recordedPosition[1] = 0.0f;

		flowfields[ nrUsedFlowfields ].team = groupTeam;

        return nrUsedFlowfields++;
    }

	void NavigationMesh::InitFlowfieldInstances()
	{
		maxFlowfields = Core::world.m_config.GetInt( "maxNumberOfFlowfields", MAX_NUMBER_OF_FLOWFIELDS);

		// resize list of m_flowfields...
		flowfields = Core::world.m_levelHeap.NewPODArray<Core::NavigationMesh::Flowfield>( maxFlowfields );
		nrUsedFlowfields = 0;
	}


	void NavigationMesh::CalculateLinks()
	{
		// for all m_nodes
		for( int i = 0; i < nrNodes; i++ )
		{
			// for every edge in the node
			for( int p = 0; p < 4; p++ )
			{
				if( nodes[i].corners[p].linksTo >= 0 )
				{
					// for all edges in the linked-to node
					for( int q = 0; q < 4; q++ )
					{
						if( nodes[nodes[i].corners[p].linksTo].corners[q].linksTo == i )
							nodes[i].corners[p].linksToEdge = q;
					}
				}
			}
		}
			
		// conncet corners...
		for( int i = 0; i < nrNodes; i++ )
		{
			// for every edge in the node
			for( int p = 0; p < 4; p++ )
			{
				// check if valid corner
				if( nodes[i].corners[p].length < 0 )
					continue;

				int current = p * 2;
				glm::vec2 cornerPos = glm::vec2( nodes[i].points[current], nodes[i].points[ current + 1 ] );

				// calculate corner connections...
				for( int q = 0; q < nrNodes; ++q )
				{
					// for every edge in the node
					int nrCorners = 4;
					if( nodes[q].corners[3].length < 0 )
						nrCorners = 3;


					for( int v = 0; v < nrCorners; ++v ) 
					{						
						int otherCurrent = v * 2;
						glm::vec2 otherCornerPos = glm::vec2( nodes[q].points[otherCurrent], nodes[q].points[ otherCurrent + 1 ] );

						// check square distance to other corner
						float dist = glm::distance( cornerPos, otherCornerPos );
						if( dist < 0.05f )
						{
							// check which edge the corner connects to...
							int prevIndex = v-1;
							if( prevIndex < 0 )
								prevIndex = nrCorners - 1;

							int linkIndex = nodes[i].corners[p].cornerConnectsToNode[0] < 0 ? 0 : 1;

							// set connecting corner
							if( nodes[q].corners[prevIndex].linksTo < 0 )
							{
								if( q != i )
								{
									nodes[i].corners[p].cornerConnectsToNode[linkIndex] = q;								
									nodes[i].corners[p].cornerConnectsToCorner[linkIndex] = prevIndex;
								}
							}
							else if( nodes[q].corners[v].linksTo < 0 && q != i )
							{
								if( q != i )
								{
									nodes[i].corners[p].cornerConnectsToNode[linkIndex] = q;
									nodes[i].corners[p].cornerConnectsToCorner[linkIndex] = v;
								}
							}
							else if( q == i && nodes[q].corners[v].linksTo < 0 )
							{
								//nodes[i].corners[p].cornerConnectsToNode[linkIndex] = i;
								//nodes[i].corners[p].cornerConnectsToCorner[linkIndex] = p;
							}
						}
					}
				}
			}

			int nrEdges = nodes[i].corners[3].length < 0 ? 3 : 4;
			for( int k = 0; k < nrEdges; k++ )
			{
				for( int l = 0; l < 2; l++ )
				{
					if( nodes[i].corners[k].cornerConnectsToNode[l] < 0 )
					{
						nodes[i].corners[k].cornerConnectsToNode[l] = i;
						nodes[i].corners[k].cornerConnectsToCorner[l] = k;
					}
				}
			}
		}
	}

	void ToggleDrawOfNavigationMesh(clop::ArgList args)
	{
		drawNavigationMesh = !drawNavigationMesh;
	}

	void DrawToggledNavigationMesh()
	{
		if( Core::GetNavigationMesh() && drawNavigationMesh )
			Core::GetNavigationMesh()->DrawDebug();
	}
}


//template < class T >
std::fstream& operator>> ( std::fstream& ff, Core::NavigationMesh::Node& node )
{
	// read points...
	for( int i = 0; i < 8; i++ )
		ff >> node.points[i];

	// handle meta...
	for( int i = 0; i < 4; i++ )
	{
		node.corners[i].cornerConnectsToNode[0] = NAVMESH_NO_CONNECTING_CORNERS;
		node.corners[i].cornerConnectsToNode[1] = NAVMESH_NO_CONNECTING_CORNERS;

		ff >> node.corners[i].linksTo;

		int current = i * 2;
		int following = (current + 2) % 8; 
		float deltaX = node.points[ following ] - node.points[ current ];
		float deltaY = node.points[ following + 1] - node.points[current + 1];
		
		// calc length, set to negative if the node is a triangle...
		if( node.corners[i].linksTo < -1.5f )
		{
			node.corners[i].length = -1.0f;
			continue;
		}
		else
		{
			node.corners[i].length = std::sqrt( deltaX * deltaX + deltaY * deltaY );
			node.corners[i].inverseLength = 1.0f / node.corners[i].length;
		}


		glm::vec3 lineStart = glm::vec3( node.points[ current ], 0.0f, node.points[ current + 1 ] );
		glm::vec3 lineEnd	= glm::vec3( node.points[ following ], 0.0f, node.points[ following + 1 ] );

		// calc normal
		glm::vec3 cross = glm::normalize( glm::cross( (lineEnd - lineStart), glm::vec3( 0.0f, 1.0f, 0.0f ) ) );
		node.corners[i].normal[0] = cross.x;
		node.corners[i].normal[1] = cross.z;

	}

	return ff;
}


