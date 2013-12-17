#include "RenderingSystem.hpp"
#include "World.hpp"
#include <gfx/GFXInterface.hpp>
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
			translationMatrix = glm::translate(glm::mat4(1.0f), WorldPositionComponent::GetVec3(wpc->position));
			scaleMatrix = glm::scale(sc->scale, sc->scale, sc->scale);

			//Build modelMatrix using linear allocator (FIX THIS)
			glm::mat4* modelMatrix = Core::world.m_frameHeap.NewObject<glm::mat4>(translationMatrix * rotationMatrix * scaleMatrix);
			//glm::mat4* modelMatrix = new glm::mat4(translationMatrix * rotationMatrix * scaleMatrix);
			//delete modelMatrix;
			GFX::Draw(gc->bitmask, (void*)modelMatrix);
		}
	}
}
