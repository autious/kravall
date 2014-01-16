#include "AIDebugSystem.hpp"
#include "World.hpp"

#include <gfx/GFXInterface.hpp>
#include <string>
#include <logger/Logger.hpp>
#include <Camera/Camera.hpp>
#include <limits>
#include <GameUtility/NavigationMesh.hpp>
#include <Input/InputManager.hpp>


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
		Core::GetNavigationMesh()->CalculateFlowfieldForGroup( glm::vec2(position.x, position.z), 0 );
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



void Core::AIDebugSystem::Update( float delta )
{
	MarkClickedObject();

	CheckNavMeshCalculation();




	//if( Core::GetNavigationMesh() )
	//	Core::GetNavigationMesh()->DrawDebug();
}
