#include "PickingSystem.hpp"
#include "World.hpp"
#include <gfx/GFXInterface.hpp>
#include "Camera/Camera.hpp"
#include <limits>
#include <logger/Logger.hpp>
#include <Input/InputManager.hpp>

Core::PickingSystem::PickingSystem()
: BaseSystem(EntityHandler::GenerateAspect< WorldPositionComponent, BoundingVolumeComponent >(), 0ULL), 
m_lastSelectedEntity(INVALID_ENTITY), m_currentGroundHit(glm::vec3(0.0f)) 
{ }


void Core::PickingSystem::Update( float delta )
{
    int x,y;
    Core::GetInputManager().GetMouseState().GetCursorPosition(x,y);
	m_currentGroundHit = GetGroundHit(x,y);
	
	GetHitEntity( x,y );
}




Core::Entity Core::PickingSystem::GetHitEntity(int mouseX, int mouseY, Core::Aspect aspectMask)
{
	glm::vec3 rayDir = GetRayFromCamera( mouseX, mouseY );
	glm::vec3 rayOrigin = Core::gameCamera.GetPosition();

	float closestHit = FLT_MAX;
	Entity objectHit;
	bool somethingHit = false;

	for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
	{
		Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
		Core::BoundingVolumeComponent* bvc = WGETC<Core::BoundingVolumeComponent>(*it);

		if ((Core::world.m_entityHandler.GetEntityAspect(*it) & aspectMask) != aspectMask)
			continue;
		
		if( bvc->type == Core::BoundingVolumeType::SphereBoundingType )
		{
		    Core::BoundingSphere* sphere = (Core::BoundingSphere*)bvc->data;

			glm::vec3 toSphere = wpc->GetVec3(*wpc) + glm::vec3(sphere->offset[0], sphere->offset[1], sphere->offset[2]) - rayOrigin;

			float squareLength = glm::dot( toSphere, toSphere );
			float projectedDistanceToSphere = glm::dot( toSphere, rayDir );

			// ray is inside the sphere, ignore...
			if( squareLength < sphere->radius * sphere->radius ) 
			{
				continue;
			}

			// sphere is behind the ray and ray is not inside it, ignore...
			if( projectedDistanceToSphere < 0 ) 
			{
				continue;
			}

			// the sphere is too far from the line, ignore...
			if( squareLength - projectedDistanceToSphere * projectedDistanceToSphere > sphere->radius * sphere->radius ) 
            {
				continue;
			}

			// does the ray intersect with the sphere...
			float sphereIntersectionDelta = sqrt( sphere->radius * sphere->radius - (squareLength - projectedDistanceToSphere * projectedDistanceToSphere) );

			// record closest hit sphere...
			if( projectedDistanceToSphere - sphereIntersectionDelta < closestHit )
			{
				somethingHit = true;
				objectHit = *it;
				closestHit = projectedDistanceToSphere - sphereIntersectionDelta;
			}
		}

		if( bvc->type == Core::BoundingVolumeType::AABBBoundingType )
		{
			LOG_INFO << "AABB picking is yet to be implemented" << std::endl;
		}
		
	}

	if( somethingHit )
	{
		m_lastSelectedEntity = objectHit;
		return objectHit;
	}

	m_lastSelectedEntity = INVALID_ENTITY; 
	return INVALID_ENTITY;
}



glm::vec3 Core::PickingSystem::GetGroundHit( int mouseX, int mouseY )
{
	glm::vec3 rayDir = GetRayFromCamera( mouseX, mouseY );
	glm::vec3 origin = Core::gameCamera.GetPosition();

	float tValue = ( -glm::dot( glm::vec3( 0,1,0 ), origin )) / glm::dot( glm::vec3( 0,1,0 ), rayDir );
	glm::vec3 targetPosition = origin + rayDir * tValue;

	return targetPosition;
}



glm::vec3 Core::PickingSystem::GetRayFromCamera( int mouseX, int mouseY )
{
	glm::vec2 mousePos;
	mousePos.x = ( 2.f * (float)mouseX ) / (float)GFX::GetScreenWidth() - 1.f;
	mousePos.y = 1.f - ( 2.f * (float)mouseY ) / (float)GFX::GetScreenHeight();

	glm::vec4 rayClipSpace = glm::vec4( mousePos.x, mousePos.y, -1, 1 );
	glm::vec4 rayEyeSpace = glm::inverse( Core::gameCamera.GetProjectionMatrix() ) * rayClipSpace;
	rayEyeSpace.z = -1;
	rayEyeSpace.w = 0;

	glm::vec4 ray = glm::normalize( glm::inverse( Core::gameCamera.GetViewMatrix() ) * rayEyeSpace );

	return glm::vec3( ray );
}

