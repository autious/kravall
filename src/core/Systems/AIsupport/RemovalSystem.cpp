#include "RemovalSystem.hpp"
#include <World.hpp>

#include <Animation/AnimationManager.hpp>

namespace Core
{

	RemovalSystem::RemovalSystem() :BaseSystem(EntityHandler::GenerateAspect<UnitTypeComponent, AttributeComponent, TargetingComponent>(), 0ULL)
	{
		m_foundAssets = false;
	}


	void RemovalSystem::Update(float delta)
	{
		if (!m_foundAssets)
		{
			m_bloodBitmask = 0;
			
			Core::world.m_contentManager.Load<Core::GnomeLoader>("assets/model/dev/cube.bgnome", [this](Core::BaseAssetLoader* baseLoader, Core::AssetHandle handle)
			{
				Core::ModelData data = *static_cast<Core::ModelData*>(handle);
				this->m_mesh = data.meshID;
			});

			Core::world.m_contentManager.Load<Core::MaterialLoader>("assets/texture/decal/blood-decal.material", [this](Core::BaseAssetLoader* baseLoader, Core::AssetHandle handle)
			{
				Core::MaterialData data = *static_cast<Core::MaterialData*>(handle);
				this->m_material = data.materialId;
			});

			GFX::SetBitmaskValue(m_bloodBitmask, GFX::BITMASK::MESH_ID, m_mesh);
			GFX::SetBitmaskValue(m_bloodBitmask, GFX::BITMASK::MATERIAL_ID, m_material);
			GFX::SetBitmaskValue(m_bloodBitmask, GFX::BITMASK::LAYER, GFX::LAYER_TYPES::MESH_LAYER);
			GFX::SetBitmaskValue(m_bloodBitmask, GFX::BITMASK::TYPE, GFX::OBJECT_TYPES::DECAL_GEOMETRY);

			m_foundAssets = true;
		}
		else
		{

			Entity* rioterEntities = Core::world.m_frameHeap.NewPODArray<Entity>(m_entities.size());
			Entity* policeEntities = Core::world.m_frameHeap.NewPODArray<Entity>(m_entities.size());
			int rioterCount = 0;
			int policeCount = 0;

			for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
			{

				UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);

				if (utc->type == UnitType::Rioter)
				{
					AttributeComponent* atc = WGETC<AttributeComponent>(*it);
					//Leave graphics, animation, WPC, scale and rotation so we can keep rendering the object
					if (atc->health <= 0)
					{
						WorldPositionComponent* wpc = WGETC<WorldPositionComponent>(*it);
						MovementComponent* mc = WGETC<MovementComponent>(*it);
					
						CreateBloodDecal(WorldPositionComponent::GetVec3(*wpc), mc->newDirection[0], mc->newDirection[3], mc->newDirection[2]);
						rioterEntities[rioterCount++] = *it;
					}
					else if (atc->stamina <= 0)//Some kind of formula shiet here
					{
						rioterEntities[rioterCount++] = *it;
					}
				}
				else if (utc->type == UnitType::Police)
				{
					AttributeComponent* atc = WGETC<AttributeComponent>(*it);
					if (atc->health <= 0)
					{
						WorldPositionComponent* wpc = WGETC<WorldPositionComponent>(*it);
						MovementComponent* mc = WGETC<MovementComponent>(*it);

						CreateBloodDecal(WorldPositionComponent::GetVec3(*wpc), mc->newDirection[0], mc->newDirection[3], mc->newDirection[2]);
						policeEntities[policeCount++] = *it;
					}
				}
			}

			for (int i = 0; i < rioterCount; i++)
			{
				// remove this entity form its attackers...
				Core::TargetingComponent* tc = WGETC<Core::TargetingComponent>(rioterEntities[i]);
				Core::TargetingComponent* attackerTc = WGETC<Core::TargetingComponent>(tc->target);
				if (attackerTc)
					Core::TargetingComponent::StopAttacking(rioterEntities[i], *attackerTc);

				//for( int p = 0; p < tc->numberOfAttackers; p++ )
				//{
				//	Core::TargetingComponent* attackerTcomp = WGETC<Core::TargetingComponent>(tc->attackers[p]);
				//	if( attackerTcomp )
				//		attackerTcomp->target = INVALID_ENTITY;
				//}

				Core::GraphicsComponent* gfxc = WGETC<Core::GraphicsComponent>(rioterEntities[i]);
				if (gfxc)
					GFX::SetBitmaskValue(gfxc->bitmask, GFX::BITMASK::LAYER, GFX::LAYER_TYPES::MESH_LAYER);

				Core::AnimationComponent* animc = WGETC<Core::AnimationComponent>(rioterEntities[i]);
				if (animc)
					Core::AnimationManager::PlayAnimation(rioterEntities[i], "die");

				world.m_entityHandler.RemoveComponents<UnitTypeComponent, MovementComponent, AttributeComponent, TargetingComponent, BoundingVolumeComponent, FlowfieldComponent>(rioterEntities[i]);
			}


			for (int i = 0; i < policeCount; i++)
			{

				// remove this entity form its attackers...
				Core::TargetingComponent* tc = WGETC<Core::TargetingComponent>(policeEntities[i]);
				Core::TargetingComponent* attackerTc = WGETC<Core::TargetingComponent>(tc->target);
				if (attackerTc)
					Core::TargetingComponent::StopAttacking(policeEntities[i], *attackerTc);

				//LOG_WARNING << "police " << policeEntities[i] << " died, having " << tc->attackers << " number of attackers" << std::endl;
				//std::cout << "police " << policeEntities[i] << " died, attacked by " << tc->attackers[0] << "  " << tc->attackers[1] << "  " << tc->attackers[2] << "  " << tc->attackers[3] << "  " << tc->attackers[4] << ", attacking " << tc->target << std::endl;

				//for( int p = 0; p < tc->numberOfAttackers; p++ )
				//{
				//	Core::TargetingComponent* attackerTcomp = WGETC<Core::TargetingComponent>(tc->attackers[p]);
				//	if( attackerTcomp )
				//		attackerTcomp->target = INVALID_ENTITY;
				//}

				Core::GraphicsComponent* gfxc = WGETC<Core::GraphicsComponent>(policeEntities[i]);
				if (gfxc)
					GFX::SetBitmaskValue(gfxc->bitmask, GFX::BITMASK::LAYER, GFX::LAYER_TYPES::MESH_LAYER);


				Core::AnimationComponent* animc = WGETC<Core::AnimationComponent>(policeEntities[i]);
				if (animc)
					Core::AnimationManager::PlayAnimation(policeEntities[i], "die");

				world.m_entityHandler.RemoveComponents<UnitTypeComponent, MovementComponent, AttributeComponent, TargetingComponent, BoundingVolumeComponent, FlowfieldComponent>(policeEntities[i]);
			}
		}
	}

	void RemovalSystem::CreateBloodDecal(glm::vec3 position, float dirX, float dirY, float dirZ)
	{

		GraphicsComponent gc;
		gc.bitmask = m_bloodBitmask;
		gc.outlineColor[0] = 0.0f;
		gc.outlineColor[1] = 1.0f;
		gc.outlineColor[2] = 0.0f;
		gc.outlineColor[3] = 0.0f;

		WorldPositionComponent wpc;
		wpc.position[0] = position.x;
		wpc.position[1] = 0.0f;
		wpc.position[2] = position.z;

		RotationComponent rc; 
		rc = RotationComponent::GetComponentRotateY(rand_FloatRange(0, 3.14f));

		ScaleComponent sc;
		BloodComponent bc;

		bc.scaleSpeed[0] = 0.4f;
		bc.scaleSpeed[1] = 0.4f;
		bc.maxSize = rand_FloatRange(0, 3.14f);

		Core::world.m_entityHandler.CreateEntity(gc, wpc, rc, sc, bc);
	}


}
