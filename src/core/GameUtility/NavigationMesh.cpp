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
        if(m_nrUsedFlowfields >= m_maxFlowfields)
        {
            LOG_FATAL << "Flowfield array is out of indices" << std::endl;
        }

        m_flowfields[m_nrUsedFlowfields].edges = Core::world.m_levelHeap.NewPODArray<int>( m_nrNodes );
        m_flowfields[m_nrUsedFlowfields].list = reinterpret_cast<glm::vec3*>(Core::world.m_levelHeap.NewPODArray<unsigned char>(sizeof(glm::vec3) * m_nrNodes));
        m_flowfields[m_nrUsedFlowfields].goal[0] = std::numeric_limits<float>::max();
        m_flowfields[m_nrUsedFlowfields].goal[1] = std::numeric_limits<float>::max();

		std::memset( m_flowfields[m_nrUsedFlowfields].edges, 0, m_nrNodes * sizeof(int) );
		std::memset( m_flowfields[m_nrUsedFlowfields].list, 0, m_nrNodes * sizeof(glm::vec3) );

        return m_nrUsedFlowfields++;
    }

	void NavigationMesh::InitFlowfieldInstances()
	{
		m_maxFlowfields = Core::world.m_config.GetInt( "maxNumberOfFlowfields", MAX_NUMBER_OF_FLOWFIELDS);

		// resize list of m_flowfields...
		m_flowfields = Core::world.m_levelHeap.NewPODArray<Core::NavigationMesh::Flowfield>( m_maxFlowfields );
		m_nrUsedFlowfields = 0;
	}


	void NavigationMesh::CalculateLinks()
	{
		// for all m_nodes
		for( int i = 0; i < m_nrNodes; i++ )
		{
			// for every edge in the node
			for( int p = 0; p < 4; p++ )
			{
				if( m_nodes[i].corners[p].linksTo >= 0 )
				{
					// for all edges in the linked-to node
					for( int q = 0; q < 4; q++ )
					{
						if( m_nodes[m_nodes[i].corners[p].linksTo].corners[q].linksTo == i )
							m_nodes[i].corners[p].linksToEdge = q;
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


