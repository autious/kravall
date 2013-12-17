#include "FlowfieldSystem.hpp"
#include "World.hpp"
//#include <gfx/GFXInterface.hpp>
//#include "Camera/Camera.hpp"
//#include <limits>
#include <logger/Logger.hpp>

Core::FlowfieldSystem::FlowfieldSystem()
	: BaseSystem( EntityHandler::GenerateAspect< WorldPositionComponent, BoundingVolumeComponent >(), 0ULL )
{
}


void Core::FlowfieldSystem::Update( float delta )
{
	
}