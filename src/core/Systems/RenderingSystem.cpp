#include "RenderingSystem.hpp"
#include "World.hpp"
#include <gfx/GFXInterface.hpp>
namespace Core
{
	void RenderingSystem::Update(float delta)
	{
		for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
		{
			WorldPositionComponent* wpc = WGETC<WorldPositionComponent>(*it);
			RotationComponent* rc		= WGETC<RotationComponent>(*it);
			GraphicsComponent* gc		= WGETC<GraphicsComponent>(*it);
			ScaleComponent* sc			= WGETC<ScaleComponent>(*it);

			//Build the matrices needed for model matrix
			glm::quat rot = RotationComponent::GetQuat(rc->rotation);
			glm::mat4 rotationMatrix = glm::toMat4(rot);
			glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), WorldPositionComponent::GetVec3(wpc->position));
			glm::mat4 scaleMatrix = glm::scale(sc->scale, sc->scale, sc->scale);

			//Build modelMatrix using linear allocator (FIX THIS)
			glm::mat4* modelMatrix = new glm::mat4(translationMatrix * rotationMatrix * scaleMatrix);

			GFX::Draw(gc->ibo, gc->vao, gc->iboSize, gc->shader, gc->material, modelMatrix);

			//GFX::Draw(gc->ibo, gc->vao, gc->iboSize)

			//Get components
			//Pass to gfx
		}
	}
}