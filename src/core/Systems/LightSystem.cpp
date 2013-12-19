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

			GFX::LightData* data = Core::world.m_frameHeap.NewObject<GFX::LightData>();
			
			i++;
			switch (lc->type)
			{
				// Point light
				case GFX::LIGHT_TYPES::POINT_SHADOW:
				case GFX::LIGHT_TYPES::POINT:
				{
					GFX::PointLight* p = reinterpret_cast<GFX::PointLight*>(lc->LightData);
					data->color = p->color;
					data->intensity = p->intensity;

					wpc->position[0] = 300.0f * sin(lel + double(i));

					data->position = WorldPositionComponent::GetVec3(wpc->position);
					data->radius_length = sc->scale;

					//GFX::Debug::DrawSphere(data->position, sc->scale, glm::vec4(data->color, 1.0f), false);
				}
				break;

				// Spot light
				case GFX::LIGHT_TYPES::SPOT_SHADOW:
				case GFX::LIGHT_TYPES::SPOT:
				{
					GFX::SpotLight* p = reinterpret_cast<GFX::SpotLight*>(lc->LightData);
					data->color = p->color;
					data->orientation = glm::vec4(RotationComponent::GetQuat(rc->rotation)[0]);
					data->intensity = p->intensity;
					data->position = WorldPositionComponent::GetVec3(wpc->position);
					data->radius_length = sc->scale;
					data->spot_angle = p->angle;
				}
				break;

				// Directional light
				case GFX::LIGHT_TYPES::DIR_SHADOW:
				case GFX::LIGHT_TYPES::DIR:
				{
					GFX::DirectionalLight* p = reinterpret_cast<GFX::DirectionalLight*>(lc->LightData);
					data->color = p->color;
					data->orientation = glm::vec4(RotationComponent::GetQuat(rc->rotation)[0]);
					data->intensity = p->intensity;
				}
				break;

				// Ambient light
				case GFX::LIGHT_TYPES::AMBIENT:
				{
					GFX::AmbientLight* p = reinterpret_cast<GFX::AmbientLight*>(lc->LightData);
					data->color = p->color;
					data->intensity = p->intensity;
				}
				break;
			}

			// Use linear allocator
			GFX::Draw(lc->bitmask, (void*)data);
		}
	}
}
