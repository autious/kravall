#include "NavigationMesh.hpp"
#include <World.hpp>

#include <fstream>
#include <logger/Logger.hpp>
#include <limits>
static bool drawNavigationMesh = true;

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

    int NavigationMesh::CreateGroup()
    {
        if(nrUsedFlowfields >= maxFlowfields)
        {
            LOG_FATAL << "Flowfield array is out of indices" << std::endl;
        }

        flowfields[nrUsedFlowfields].edges = Core::world.m_levelHeap.NewPODArray<int>( nrNodes );
		flowfields[nrUsedFlowfields].list = reinterpret_cast<glm::vec3*>(Core::world.m_levelHeap.NewPODArray<float>( 3 * nrNodes ));


		std::memset( flowfields[nrUsedFlowfields].edges, 0, nrNodes * sizeof(int) );
		std::memset( flowfields[nrUsedFlowfields].list, 0, nrNodes * sizeof(glm::vec3) );

		flowfields[nrUsedFlowfields].goal[ 0 ] = std::numeric_limits<float>::max();
		flowfields[nrUsedFlowfields].goal[ 1 ] = std::numeric_limits<float>::max();

		flowfields[nrUsedFlowfields].timeSinceLastCheck = -1.0f;

		flowfields[nrUsedFlowfields].recordedPosition[0] = 0.0f;
		flowfields[nrUsedFlowfields].recordedPosition[1] = 0.0f;

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
					//if( q == i )
					//	continue;

					// for every edge in the node
					int nrCorners = 4;
					if( nodes[q].corners[3].length < 0 )
						nrCorners = 3;

					for( int v = 0; v < nrCorners; ++v ) 
					{						
						int otherCurrent = v * 2;
						glm::vec2 otherCornerPos = glm::vec2( nodes[q].points[otherCurrent], nodes[q].points[ otherCurrent + 1 ] );

						// check square distance to other corner
						if( glm::dot( cornerPos - otherCornerPos, cornerPos - otherCornerPos ) < 0.05f )
						{
							// check which edge the corner connects to...
							int prevIndex = v-1;
							if( prevIndex < 0 )
								prevIndex = nrCorners - 1;

							int nextIndex = (v+1) % nrCorners;

							// set connecting corner
							if( nodes[q].corners[prevIndex].linksTo < 0 )
							{
								if( q != i )
								{	
									nodes[i].corners[p].cornerConnectsToNode = q;								
									nodes[i].corners[p].cornerConnectsToCorner = prevIndex;
								}
							}
							else if( nodes[q].corners[v].linksTo < 0 && q != i )
							{
								if( q != i )
								{
									nodes[i].corners[p].cornerConnectsToNode = q;
									nodes[i].corners[p].cornerConnectsToCorner = nextIndex;
								}
							}
							else if( q == i )
							{
								nodes[i].corners[p].cornerConnectsToNode = NAVMESH_CONCAVE_CORNER_NODE;
								nodes[i].corners[p].cornerConnectsToCorner = NAVMESH_CONCAVE_CORNER_NODE;
							}
						}
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
		node.corners[i].cornerConnectsToNode = NAVMESH_NO_CONNECTING_CORNERS;
		node.blocked[i] = 0.0f;

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


