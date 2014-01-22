#include "AIDebugSystem.hpp"
#include "World.hpp"

#include <gfx/GFXInterface.hpp>
#include <string>
#include <logger/Logger.hpp>
#include <Camera/Camera.hpp>
#include <limits>
#include <GameUtility/NavigationMesh.hpp>
#include <Input/InputManager.hpp>

#include <GameUtility/NavigationMeshAStar.hpp>

void MarkClickedObject()
{
	int systemMax = Core::world.m_systemHandler.GetSystemCount();
	int pickingSystem = -1;
	for( int i = 0; i < systemMax; i++ )
	{
		if( std::string(Core::world.m_systemHandler.GetSystem( i )->GetHumanName()).compare( "PickingSystem" ) == 0 )
			pickingSystem = i;
	}

	if( pickingSystem < 0 )
		return;

	Core::Entity ent;
	if( (ent = ((Core::PickingSystem*)Core::world.m_systemHandler.GetSystem( pickingSystem ))->GetLastHitEntity()) != std::numeric_limits<Core::Entity>::max() )
	{
		Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(ent);
		Core::BoundingVolumeComponent* bvc = WGETC<Core::BoundingVolumeComponent>(ent);
		Core::BoundingSphere* sphere = reinterpret_cast<Core::BoundingSphere*>(bvc->data);
		GFX::Debug::DrawSphere( wpc->GetVec3(*wpc) + *reinterpret_cast<glm::vec3*>( sphere->offset ), sphere->radius, GFXColor( 1.0f, 0.7f, 0.0f, 1.0f ), false );
	}
}


/* right click to set navMesh goal for group 0 */
void CheckNavMeshCalculation()
{
	if( !Core::GetInputManager().GetMouseState().IsButtonDown(1) )
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

	if( Core::GetNavigationMesh() )
		Core::GetNavigationMesh()->CalculateFlowfieldForGroup( position, 0 );
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




}


#include <GameUtility/GameData.hpp>

void Core::AIDebugSystem::Update( float delta )
{
	MarkClickedObject();

	CheckNavMeshCalculation();

	TestPathFinding();


	GetPlanePoint();



}
