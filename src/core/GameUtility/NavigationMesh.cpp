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

	void InitFlowfieldInstances( Core::NavigationMesh* mesh, int nrFlowfieldInstances )
	{
		int nrInstances = nrFlowfieldInstances;
		if( !nrInstances )
			nrInstances = Core::world.m_config.GetDouble( "defaultNrFlowfields", 20.0f );
		
		// resize list of flowfields...
		mesh->flowfields = Core::world.m_levelHeap.NewObjectArray<Core::NavigationMesh::Flowfield>( nrInstances );
		mesh->maxFlowfields = nrInstances;
		mesh->nrUsedFlowfields = 0;

		// resize the size of each flowfield to hold the same number of vectors as there are nodes.
		for( int i = 0; i < nrInstances; i++ )
			mesh->flowfields[i].list = Core::world.m_levelHeap.NewObjectArray<glm::vec3>( mesh->nrNodes, 0.0f, 0.0f, 0.0f );
	}
}


//template < class T >
std::fstream& operator>> ( std::fstream& ff, Core::NavigationMesh::Node& node )
{
	// read points...
	for( int i = 0; i < 8; i++ )
		ff >> node.points[i];
	
	// read corners...
	for( int i = 0; i < 4; i++ )
	{
		ff >> node.corners[i].linksTo;
		ff >> node.corners[i].length;
		ff >> node.corners[i].inverseLength;
	}

	return ff;
}


