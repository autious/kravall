#include "AIDebugSystem.hpp"
#include "World.hpp"

#include <gfx/GFXInterface.hpp>
#include <string>
#include <logger/Logger.hpp>
#include <Camera/Camera.hpp>
#include <limits>
#include <GameUtility/NavigationMesh.hpp>
#include <Input/InputManager.hpp>

#include <GameUtility/PathfindingUtility.hpp>


Core::AIDebugSystem::AIDebugSystem()  
		: BaseSystem( EntityHandler::GenerateAspect< WorldPositionComponent, BoundingVolumeComponent >(), 0ULL )
{
}


void GetPlanePoint()
{
	if( !Core::GetInputManager().GetMouseState().IsButtonDown(2) )
		return;

	int systemMax = Core::world.m_systemHandler.GetSystemCount();
	int pickingSystem = -1;
	for( int i = 0; i < systemMax; i++ )
	{
		if( std::string(Core::world.m_systemHandler.GetSystem( i )->GetHumanName()).compare( "PickingSystem" ) == 0 )
			pickingSystem = i;
	}

	if( pickingSystem < 0 )
		return;

	int x, y;
	Core::GetInputManager().GetMouseState().GetCursorPosition( x, y );
	glm::vec3 position = ((Core::PickingSystem*)Core::world.m_systemHandler.GetSystem( pickingSystem ))->GetGroundHit( x, y );

	LOG_WARNING << position.x << "  " << position.z << std::endl;

	int o = 0;
}


void TestPathFinding()
{

	//Core::PathComponent path = Core::PathFinder::GetPath( glm::vec3(0), glm::vec3( -105, 0, 38 ) );
	//Core::PathComponent path = Core::PathFinder::GetPath( glm::vec3(0), glm::vec3( -442, 0, 138 ) );
	//Core::PathComponent path = Core::PathFinder::GetPath( glm::vec3(0), glm::vec3( -712, 0, -510 ) );
	//Core::PathComponent path = Core::PathFinder::GetPath( glm::vec3( 707, 0, 501 ), glm::vec3( -712, 0, -510 ) );


	//for( int i = 0; i < path.nrNodes - 1; i++ )
	{
		//glm::vec3 temp  = glm::vec3( path.nodes[ i * 2 + 0 ], 0, path.nodes[ i * 2 + 1 ] );
		//glm::vec3 temp2 = glm::vec3( path.nodes[ i * 2 + 2 ], 0, path.nodes[ i * 2 + 3 ] );
		//
		//GFX::Debug::DrawLine( temp, temp2, GFXColor( 1, 1, 0, 1 ), false );
	}

	//if( Core::GetNavigationMesh() )
	//	Core::GetNavigationMesh()->CalculateFlowfieldForGroup( glm::vec3( 0,0,350 ), 1 );

	if( Core::GetNavigationMesh() )
		;//Core::GetNavigationMesh()->CalculatedShortPath( 0, glm::vec3( 0, 0, 0 ), 8, glm::vec3( -73,0,2 ) );


}


#include <GameUtility/GameData.hpp>

void Core::AIDebugSystem::Update( float delta )
{
	


	//GetPlanePoint();


	return; //////////////////////////////////////////


	if( Core::GetNavigationMesh() )
	{
		Core::NavigationMesh* instance = Core::GetNavigationMesh();

		for( int i = 0; i < instance->nrNodes; i++ )
		{

			float* points = instance->nodes[i].points;
			glm::vec3 temp = glm::vec3(0.0f);
			for( int g = 0; g < 4; g++ )
				temp += glm::vec3( points[ g * 2 ], 0.0f, points[ g * 2 + 1] );
			temp *= 0.25f;
			GFX::Debug::DrawSphere( temp, 5, GFXColor( 0.5f, 1.0f, 0.2f, 1.0f ), false );
			if( instance->flowfields[4].list != nullptr )
				GFX::Debug::DrawLine( temp, instance->flowfields[4].list[i], GFXColor( 0, 0, 1, 1 ), false  );

		}
	}


}
