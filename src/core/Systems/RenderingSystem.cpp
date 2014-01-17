#include "RenderingSystem.hpp"
#include "World.hpp"
#include <gfx/GFXInterface.hpp>
#include <gfx/InstanceData.hpp>

namespace Core
{
	void RenderingSystem::Update(float delta)
	{
		glm::quat rot;
		glm::mat4 rotationMatrix;
		glm::mat4 translationMatrix;
		glm::mat4 scaleMatrix;

		for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
		{
			WorldPositionComponent* wpc = WGETC<WorldPositionComponent>(*it);
			RotationComponent* rc		= WGETC<RotationComponent>(*it);
			GraphicsComponent* gc		= WGETC<GraphicsComponent>(*it);
			ScaleComponent* sc			= WGETC<ScaleComponent>(*it);

			//Build the matrices needed for model matrix
			rot = RotationComponent::GetQuat(rc->rotation);
			rotationMatrix = glm::toMat4(rot);
			translationMatrix = glm::translate(glm::mat4(1.0f), WorldPositionComponent::GetVec3(*wpc));
			scaleMatrix = glm::scale(sc->scale[0], sc->scale[1], sc->scale[2]);

			//Send the data through a drawcall to GFX
			GFX::InstanceData* instanceData = Core::world.m_frameHeap.NewObject<GFX::InstanceData>();
			instanceData->modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
			instanceData->animationFrame = 0;
			instanceData->animationFrameOffset = 0;

			GFX::Draw(gc->bitmask, (void*)instanceData);
		}
	}
}
