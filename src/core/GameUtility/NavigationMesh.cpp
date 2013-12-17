#include "NavigationMesh.hpp"
#include <World.hpp>


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



	bool LoadNavigationMesh( const char* path )
	{












		return false;
	}
}


