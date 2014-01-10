#include "NavigationMesh.hpp"
#include <World.hpp>

#include <fstream>
#include <logger/Logger.hpp>

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

	void NavigationMesh::InitFlowfieldInstances( int nrFlowfieldInstances )
	{
		int nrInstances = nrFlowfieldInstances;
		if( !nrInstances )
			nrInstances = Core::world.m_config.GetDouble( "defaultNrFlowfields", 20.0f );
		
		// resize list of flowfields...
		flowfields = Core::world.m_levelHeap.NewObjectArray<Core::NavigationMesh::Flowfield>( nrInstances );
		maxFlowfields = nrInstances;
		nrUsedFlowfields = 0;

		// resize the size of each flowfield to hold the same number of vectors as there are nodes.
		for( int i = 0; i < nrInstances; i++ )
		{
			flowfields[i].edges = Core::world.m_levelHeap.NewPODArray<int>( nrNodes );
			flowfields[i].list = (glm::vec3*)Core::world.m_levelHeap.NewPODArray<float>( nrNodes * 3 );
			
			std::memset( flowfields[i].edges, 0.0f, nrNodes * sizeof(int) );
			std::memset( flowfields[i].list, 0.0f, nrNodes * sizeof(glm::vec3) );
		}
	}


	void NavigationMesh::CalculateLinks()
	{
		// for all nodes
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
		ff >> node.corners[i].linksTo;

		int current = i * 2;
		int following = (current + 2) % 8; 
		float deltaX = node.points[ following ] - node.points[ current ];
		float deltaY = node.points[ following + 1] - node.points[current + 1];
		
		// calc length, set to negative if the node is a triangle...
		if( node.corners[i].linksTo < -1.5f )
			node.corners[i].length = -1.0f;
		else
		{
			node.corners[i].length = std::sqrt( deltaX * deltaX + deltaY * deltaY );
			node.corners[i].inverseLength = 1.0f / node.corners[i].length;
		}


		glm::vec3 lineStart = glm::vec3( node.points[ current ], 0.0f, node.points[ current + 1 ] );
		glm::vec3 lineEnd	= glm::vec3( node.points[ following ], 0.0f, node.points[ following + 1 ] );

		// calc normal
		glm::vec3 cross = glm::normalize( glm::cross( (lineEnd - lineStart), glm::vec3( 0.0f, 1.0f, 0.0f ) ) );
		*reinterpret_cast<glm::vec3*>(node.corners[i].normal) = cross;

	}

	return ff;
}


