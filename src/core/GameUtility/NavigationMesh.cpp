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


