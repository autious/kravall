#include "PickingSystem.hpp"
#include "World.hpp"
#include <gfx/GFXInterface.hpp>
#include "Camera/Camera.hpp"
#include <limits>
#include <logger/Logger.hpp>
#include <GLFWInput.hpp>

Core::PickingSystem::PickingSystem()
: BaseSystem(EntityHandler::GenerateAspect< WorldPositionComponent, BoundingVolumeComponent >(), 0ULL), 
m_lastSelectedEntity(std::numeric_limits<Entity>::max()), m_currentGroundHit(glm::vec3(0.0f)), m_entityTypeMask(1 << Priority::Police)
{ }


void Core::PickingSystem::Update( float delta )
{
	m_currentGroundHit = GetGroundHit(static_cast<int>(Core::GetInput().GetXPos()), 
									  static_cast<int>(Core::GetInput().GetYPos()));

	if( !Core::GetInput().IsMouseButtonPressed(0) )
		return;
	
	Entity lastEntity = m_lastSelectedEntity;
	GetHitEntity(static_cast<int>(Core::GetInput().GetXPos()), static_cast<int>(Core::GetInput().GetYPos()));
	
	if (m_lastSelectedEntity == std::numeric_limits<Entity>::max())
	{
		glm::vec3 groundHit = GetGroundHit(static_cast<int>(Core::GetInput().GetXPos()),
										   static_cast<int>(Core::GetInput().GetYPos()));

		LOG_DEBUG << "Ground hit: " << groundHit.x << ", " << groundHit.z << std::endl;

		Core::UnitTypeComponent* utc = WGETC<Core::UnitTypeComponent>(lastEntity);

		if (utc->type == Core::UnitType::Police)
		{
			Core::MovementComponent* mc = WGETC<Core::MovementComponent>(lastEntity);
			Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(lastEntity);

			mc->goal[0] = groundHit.x;
			mc->goal[1] = groundHit.y;
			mc->goal[2] = groundHit.z;

			//glm::vec3 newDirection = groundHit - glm::vec3(wpc->position[0], groundHit.y, wpc->position[2]);
			glm::vec3 newDirection = glm::vec3(groundHit.x - wpc->position[0], 0.0f, groundHit.z - wpc->position[2]);
			newDirection /= newDirection.length();

			LOG_DEBUG << "New Direction: " << newDirection.x << ", " << newDirection.y << ", " << newDirection.z << std::endl;

			mc->direction[0] = newDirection.x;
			mc->direction[1] = newDirection.y;
			mc->direction[2] = newDirection.z;

			
		}
	}
}




Core::Entity Core::PickingSystem::GetHitEntity(int mouseX, int mouseY, char entityTypeMask)
{
	if (entityTypeMask < 0)
		entityTypeMask = m_entityTypeMask;
	glm::vec3 rayDir = GetRayFromCamera( mouseX, mouseY );
	glm::vec3 rayOrigin = Core::gameCamera->GetPosition();

	float closestHit = FLT_MAX;
	Entity objectHit;
	Core::UnitType objectHitUnitType = UnitType::Count;
	bool somethingHit = false;

	for( std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++ )
	{
		Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
		Core::BoundingVolumeComponent* bvc = WGETC<Core::BoundingVolumeComponent>(*it);

		Core::UnitTypeComponent* utc = WGETC<Core::UnitTypeComponent>(*it);

		if ((GetEntityMask(utc->type) & entityTypeMask) == 0 ||
			GetEntityMask(utc->type) < GetEntityMask(objectHitUnitType))
			continue;
		
		Core::BoundingSphere* sphere = (Core::BoundingSphere*)bvc->data;
		Core::AABB* aabb = (Core::AABB*)bvc->data;

		if( bvc->type == Core::BoundingVolumeType::SphereBoundingType )
		{
			glm::vec3 toSphere = wpc->GetVec3(*wpc) + *reinterpret_cast<glm::vec3*>(sphere->offset) - rayOrigin;
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
			if( squareLength - projectedDistanceToSphere * projectedDistanceToSphere > sphere->radius * sphere->radius ) {
				continue;
			}

			// does the ray intersect with the sphere...
			float sphereIntersectionDelta = sqrt( sphere->radius * sphere->radius - (squareLength - projectedDistanceToSphere * projectedDistanceToSphere) );

			// record closest hit sphere...
			if( projectedDistanceToSphere - sphereIntersectionDelta < closestHit )
			{
				somethingHit = true;
				objectHit = *it;
				objectHitUnitType = utc->type;
				closestHit = projectedDistanceToSphere - sphereIntersectionDelta;
			}
		}

		if( bvc->type == Core::BoundingVolumeType::AABBBoundingType )
		{
			LOG_INFO << "AABB picking is jet to be implemented" << std::endl;
		}
		
	}

	if( somethingHit )
	{
		m_lastSelectedEntity = objectHit;
		return objectHit;
	}

	m_lastSelectedEntity = std::numeric_limits<Entity>::max();
	return std::numeric_limits<Entity>::max();
}



glm::vec3 Core::PickingSystem::GetGroundHit( int mouseX, int mouseY )
{
	glm::vec3 rayDir = GetRayFromCamera( mouseX, mouseY );
	glm::vec3 origin = Core::gameCamera->GetPosition();

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
	glm::vec4 rayEyeSpace = glm::inverse( Core::gameCamera->GetProjectionMatrix() ) * rayClipSpace;
	rayEyeSpace.z = -1;
	rayEyeSpace.w = 0;

	glm::vec4 ray = glm::normalize( glm::inverse( Core::gameCamera->GetViewMatrix() ) * rayEyeSpace );

	return glm::vec3( ray );
}

Core::Entity Core::PickingSystem::GetLastHitEntity( )
{
	return m_lastSelectedEntity;
}

char Core::PickingSystem::GetEntityMask(UnitType type)
{
	switch (type)
	{
		case UnitType::Rioter:
			return 1 << Priority::Rioter;
		case UnitType::Police:
			return 1 << Priority::Police;
		default:
			return 0;
	}
}