#include "AIDebugSystem.hpp"
#include "World.hpp"

#include <gfx/GFXInterface.hpp>
#include <string>
#include <logger/Logger.hpp>
#include <Camera/Camera.hpp>
#include <limits>
#include <GameUtility/NavigationMesh.hpp>
#include <Input/InputManager.hpp>

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

	if( !Core::GetInputManager().GetMouseState().IsButtonDown(0) )
		return;

    int x,y;
    Core::GetInputManager().GetMouseState().GetCursorPosition(x,y);

	glm::vec3 hit = ((Core::PickingSystem*)Core::world.m_systemHandler.GetSystem( pickingSystem ))->GetGroundHit( x, y );
	GFX::Debug::DrawSphere( hit, 3.0f, GFXColor( 1.0f, 0.7f, 0.0f, 1.0f ), false);
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

	if( !Core::GetInputManager().GetMouseState().IsButtonDown(0) )
		return;
	
	Core::Entity ent;
    int x,y;
    Core::GetInputManager().GetMouseState().GetCursorPosition(x,y);

	if( (ent = ((Core::PickingSystem*)Core::world.m_systemHandler.GetSystem( pickingSystem ))->GetHitEntity( x,y )) != std::numeric_limits<Core::Entity>::max() )
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
		GFX::Debug::DrawSphere( wpc->GetVec3(*wpc) + *reinterpret_cast<glm::vec3*>( sphere->offset ), sphere->radius, GFXColor( 1.0f, 0.7f, 0.0f, 1.0f ), false );
	}
}


void TestNavigationMesh()
{
	std::string path = "testNaveMesh.txt";
	std::fstream ff;
	ff.open( path, std::fstream::out | std::fstream::trunc );
	if( ff.is_open() )
		int o = 0;


	ff << 1 << " ";

	for( int i = 0; i < 8; i++ )
	{
		ff << (float)i << " ";
	}

	for( int i = 0; i < 4; i++ )
	{
		ff << 3 << " ";
		ff << 5.0f << " ";
		ff << 0.01f << " ";
	}

	ff.close();
}


void Core::AIDebugSystem::Update( float delta )
{
	//CheckPickingSystemVsGround();	
	
	//CheckPickingSystemVsRioters();
	MarkClickedObject();


	


}
