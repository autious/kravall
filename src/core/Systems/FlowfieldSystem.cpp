#include "FlowfieldSystem.hpp"
#include "World.hpp"
#include <logger/Logger.hpp>



Core::FlowfieldSystem::FlowfieldSystem()
	: BaseSystem( EntityHandler::GenerateAspect< WorldPositionComponent, BoundingVolumeComponent >(), 0ULL )
{

}


void Core::FlowfieldSystem::Update( float delta )
{
	

	










}