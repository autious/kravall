#include "NavigationMeshLoader.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <logger/Logger.hpp>
#include <World.hpp>

namespace Core
{
    NavigationMeshLoader::NavigationMeshLoader()
    {

    }

    NavigationMeshLoader::~NavigationMeshLoader()
    {

    }

    Core::AssetHandle NavigationMeshLoader::LoadAsync(const char* assetName)
    {
		return LoadNavigationMeshFromFile( assetName );        
    }

    void NavigationMeshLoader::FinishLoadAsync(Core::AssetHandle& handle)
    {
		Core::SetNavigationMesh( reinterpret_cast<Core::NavigationMesh*>(handle) );
		handle = Core::GetNavigationMesh();
    }

    Core::AssetHandle NavigationMeshLoader::Load(const char* assetName)
    {        
		Core::SetNavigationMesh( LoadNavigationMeshFromFile( assetName ) );
		return Core::GetNavigationMesh();
    }

    void NavigationMeshLoader::Destroy(const Core::AssetHandle handle)
    {
		// memory is allocated on the level stack, no delete is required.

		// set world instance of navemesh to zero. 
		Core::SetNavigationMesh( nullptr );
    }

    const ModelData* NavigationMeshLoader::getData(const Core::AssetHandle handle) const
    {
        return static_cast<const Core::ModelData*>(handle);
    }



#define check( x, y ) if( x ) { LOG_ERROR << y << std::endl; return nullptr; }
	NavigationMesh* NavigationMeshLoader::LoadNavigationMeshFromFile( const char* path )
	{
		if( Core::GetNavigationMesh() )
		{
			LOG_ERROR << "NavMesh is already loaded!" << std::endl;
			return Core::GetNavigationMesh();
		}

		std::fstream ff;
		ff.open( path, std::fstream::in );
		check( !ff.is_open(), "Navmesh file could not be found! Trying to open: " << path );

		// read header...
		int nrNodes = 0;
		ff >> nrNodes;
		check( nrNodes <= 0, "Number of nodes in file cannot be less or equal to zero!" );

		NavigationMesh* instance = instance = Core::world.m_levelHeap.NewObject<Core::NavigationMesh>();

		// allocate nodes
		instance->nodes = Core::world.m_levelHeap.NewObjectArray<Core::NavigationMesh::Node>( nrNodes );

		// fill nodes
		for( int i = 0; i < nrNodes; i++ )
			ff >> instance->nodes[ i ];

		// set Core::world instance to the new instance
		instance->nrNodes = nrNodes;

		ff.close();

		// set the number of flowfield instances using the number specified in the config file.
		instance->InitFlowfieldInstances();

		// calculate the linksTo values
		instance->CalculateLinks();

		return instance;
	}
#undef check


}
