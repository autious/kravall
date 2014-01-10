#include "LightSystem.hpp"
#include "World.hpp"
#include <gfx/GFXInterface.hpp>
#include <gfx/Lights.hpp>
namespace Core
{
	static double lel = 0;
	void LightSystem::Update(float delta)
	{
		glm::quat rot;
		glm::mat4 rotationMatrix;
		glm::mat4 translationMatrix;
		glm::mat4 scaleMatrix;
		int i = 0;
		lel += 0.001f;
		for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
		{
			WorldPositionComponent* wpc = WGETC<WorldPositionComponent>(*it);
			RotationComponent* rc		= WGETC<RotationComponent>(*it);
			LightComponent* lc			= WGETC<LightComponent>(*it);
			ScaleComponent* sc			= WGETC<ScaleComponent>(*it);

            // <3 the frame heap.
			GFX::LightData* data = Core::world.m_frameHeap.NewObject<GFX::LightData>();

			data->position = glm::vec3(0.0f);
			data->radius_length = 0.0f;
			data->color = glm::vec3(0.0f);
			data->intensity = 0.0f;
			data->pad = glm::vec3(0.0f);
			data->spot_angle = 0.0f;
			data->orientation = glm::vec4(0.0f);

            unsigned int lightType = LightComponent::GetLightType( *lc );
			
			i++;
			switch (lightType)
			{
				// Point light
				case GFX::LIGHT_TYPES::POINT_SHADOW:
				case GFX::LIGHT_TYPES::POINT:
				{
					data->color[0] = lc->color[0];
					data->color[1] = lc->color[1];
					data->color[2] = lc->color[2];
					data->intensity = lc->intensity;

					//wpc->position[0] = 45 * cos(lel + double(i));
					//wpc->position[2] = 45 * sin(double(i%16) * lel*0.1f + double(i));

					data->position = WorldPositionComponent::GetVec3(*wpc);
					data->radius_length = sc->scale;

					//GFX::Debug::DrawSphere(data->position, sc->scale, glm::vec4(data->color, 1.0f), false);
				}
				break;

				// Spot light
				case GFX::LIGHT_TYPES::SPOT_SHADOW:
				case GFX::LIGHT_TYPES::SPOT:
				{
					data->color[0] = lc->color[0];
					data->color[1] = lc->color[1];
					data->color[2] = lc->color[2];
					data->intensity = lc->intensity;
					data->spot_angle = lc->lightSpecific.spotLight.angle;

					data->orientation = glm::vec4(	RotationComponent::GetQuat(rc->rotation).x*cos(10.0f*lel+ double(i)),
													RotationComponent::GetQuat(rc->rotation).y,
													RotationComponent::GetQuat(rc->rotation).z*sin(10.0f*lel + double(i+3)),
													RotationComponent::GetQuat(rc->rotation).w);
					data->position = WorldPositionComponent::GetVec3(*wpc);
					data->radius_length = sc->scale;

					wpc->position[0] = 45 * cos(lel + double(i));
					wpc->position[2] = 45 * sin(double(i%16) * lel*0.1f + double(i));

					//GFX::Debug::DrawSphere(data->position, sc->scale, glm::vec4(data->color, 1.0f), false);
					//glm::vec3 dir = glm::normalize(glm::vec3(data->orientation)) * data->radius_length;
					//GFX::Debug::DrawLine(data->position, data->position + dir, glm::vec4(data->color, 1.0f), 1.0f, false);
				}
				break;

				// Directional light
				case GFX::LIGHT_TYPES::DIR_SHADOW:
				case GFX::LIGHT_TYPES::DIR:
				{
					data->color[0] = lc->color[0];
					data->color[1] = lc->color[1];
					data->color[2] = lc->color[2];
					data->intensity = lc->intensity;
					data->orientation = glm::vec4(	RotationComponent::GetQuat(rc->rotation).x,
													RotationComponent::GetQuat(rc->rotation).y,
													RotationComponent::GetQuat(rc->rotation).z,
													RotationComponent::GetQuat(rc->rotation).w);
				}
				break;

				// Ambient light
				case GFX::LIGHT_TYPES::AMBIENT:
				{
					data->color[0] = lc->color[0];
					data->color[1] = lc->color[1];
					data->color[2] = lc->color[2];
					data->intensity = lc->intensity;
				}
				break;
			}
			// Use linear allocator
			GFX::Draw(lc->bitmask, (void*)data);
		}
	}
}
