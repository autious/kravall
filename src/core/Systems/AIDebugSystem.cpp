#include "AIDebugSystem.hpp"
#include "World.hpp"

#include <gfx/GFXInterface.hpp>
#include <string>
#include <logger/Logger.hpp>
#include <GLFWInput.hpp>
#include <Camera/Camera.hpp>
#include <limits>
#include <GameUtility/NavigationMesh.hpp>

void CheckPickingSystemVsGround()
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

	if( !Core::GetInput().IsMouseButtonPressed(0) )
		return;

	glm::vec3 hit = ((Core::PickingSystem*)Core::world.m_systemHandler.GetSystem( pickingSystem ))->GetGroundHit( Core::GetInput().GetXPos(), Core::GetInput().GetYPos() );
	GFX::Debug::DrawSphere( hit, 3.0f, GFXColor( 1.0f, 0.7f, 0.0f, 1.0f ) );
}



void CheckPickingSystemVsRioters()
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

	if( !Core::GetInput().IsMouseButtonPressed(0) )
		return;
	
	Core::Entity ent;
	if( (ent = ((Core::PickingSystem*)Core::world.m_systemHandler.GetSystem( pickingSystem ))->GetHitEntity( Core::GetInput().GetXPos(), Core::GetInput().GetYPos() )) != std::numeric_limits<Core::Entity>::max() )
		return;

}

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
		GFX::Debug::DrawSphere( wpc->GetVec3(wpc->position) + *sphere->GetVec3( sphere->offset ), sphere->radius, GFXColor( 1.0f, 0.7f, 0.0f, 1.0f ) );
	}
}



void Core::AIDebugSystem::Update( float delta )
{
	//CheckPickingSystemVsGround();	
	
	//CheckPickingSystemVsRioters();
	MarkClickedObject();
	

	NavigationMesh* ms = GetNavigationMesh();

	ms = (NavigationMesh*)1;

	NavigationMesh* ms2 = GetNavigationMesh();

	int o = 0;

	


}