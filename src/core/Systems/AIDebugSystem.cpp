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


void CheckIfPointIsInsideNavMesh()
{
	if( !Core::GetNavigationMesh() )
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


	glm::vec3 temp = ((Core::PickingSystem*)Core::world.m_systemHandler.GetSystem( pickingSystem ))->GetGroundHit( Core::GetInput().GetXPos(), Core::GetInput().GetYPos() );

	if( Core::GetNavigationMesh()->CheckPointInsideNode( temp, 0 ) )
	{
		GFX::Debug::DrawSphere( temp, 5.0f, GFXColor( 0.0f, 0.7f, 1.0f, 1.0f ), false );
	}


}

void CheckNavMeshCalculation()
{
	if( !Core::GetInput().IsMouseButtonPressed(1) )
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


	glm::vec3 temp = ((Core::PickingSystem*)Core::world.m_systemHandler.GetSystem( pickingSystem ))->GetGroundHit( Core::GetInput().GetXPos(), Core::GetInput().GetYPos() );

	Core::GetNavigationMesh()->CalculateFlowfieldForGroup( temp );
}


void Core::AIDebugSystem::Update( float delta )
{
	//CheckPickingSystemVsGround();	

	//CheckPickingSystemVsRioters();
	MarkClickedObject();

	//CheckIfPointIsInsideNavMesh();

	CheckNavMeshCalculation();


	if( Core::GetNavigationMesh() )
	{
		for( int i = 0; i < Core::GetNavigationMesh()->nrNodes; i++ )
		{
			//GFX::Debug::DrawSphere( Core::GetNavigationMesh()->flowfields[0].list[i], 5.0f, GFXColor( 0.0f, 0.7f, 1.0f, 1.0f ), false );
		}
	}


}


