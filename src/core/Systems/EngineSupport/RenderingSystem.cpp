#include "RenderingSystem.hpp"

#include <gfx/GFXInterface.hpp>
#include <gfx/InstanceData.hpp>
#include <gfx/BitmaskDefinitions.hpp>
#include <World.hpp>

namespace Core
{

    RenderingSystem::RenderingSystem() :BaseSystem(EntityHandler::GenerateAspect<GraphicsComponent, WorldPositionComponent, RotationComponent, ScaleComponent>(), 0ULL)
		{}


	void RenderingSystem::Update(float delta)
	{
		glm::quat rot;
		glm::mat4 rotationMatrix;
		glm::mat4 translationMatrix;
		glm::mat4 scaleMatrix;
		int i = 0;
		for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
		{
			WorldPositionComponent* wpc = WGETC<WorldPositionComponent>(*it);
			RotationComponent* rc		= WGETC<RotationComponent>(*it);
			GraphicsComponent* gc		= WGETC<GraphicsComponent>(*it);
			ScaleComponent* sc			= WGETC<ScaleComponent>(*it);
			AnimationComponent* ac		= WGETC<AnimationComponent>(*it);

            if( gc->render )
            {
                //Build the matrices needed for model matrix
                rot = RotationComponent::GetQuat(rc->rotation);
                rotationMatrix = glm::toMat4(rot);
                translationMatrix = glm::translate(glm::mat4(1.0f), WorldPositionComponent::GetVec3(*wpc));

				//ugly hack, outline color is used as scale for decals
				if (GFX::GetBitmaskValue(gc->bitmask, GFX::BITMASK::TYPE) == GFX::OBJECT_TYPES::DECAL_GEOMETRY)
					scaleMatrix = glm::scale(gc->outlineColor[0], gc->outlineColor[1], gc->outlineColor[2]);
				else
					scaleMatrix = glm::scale(sc->scale[0], sc->scale[1], sc->scale[2]);
				
                //Send the data through a drawcall to GFX
                GFX::InstanceData* instanceData = Core::world.m_frameHeap.NewObject<GFX::InstanceData>();
                instanceData->modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
                instanceData->rnd_seed = *it;
                instanceData->frameOffset = 0;
                instanceData->animationIndex = -1;

				instanceData->outlineColor[0] = gc->outlineColor[0];
				instanceData->outlineColor[1] = gc->outlineColor[1];
				instanceData->outlineColor[2] = gc->outlineColor[2];
				instanceData->outlineColor[3] = gc->outlineColor[3];

				
                if (ac != nullptr)
                {
                    instanceData->animationIndex = ac->animationID;
                    instanceData->frameOffset = ac->currentFrame;
                }
                
                GFX::Draw(gc->bitmask, (void*)instanceData);
            }
		}
	}
}
