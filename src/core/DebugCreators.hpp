
static void CreateLightsAndTea()
{
    unsigned int meshID; 
    unsigned int materialID;

    Core::world.m_contentManager.Load<Core::GnomeLoader>("assets/cube.bgnome", [&meshID](Core::BaseAssetLoader* baseLoader, Core::AssetHandle handle)
            {
                Core::GnomeLoader* gnomeLoader = dynamic_cast<Core::GnomeLoader*>(baseLoader);
                const Core::ModelData* data = gnomeLoader->getData(handle);
				meshID = data->meshID;
            });

    Core::world.m_contentManager.Load<Core::MaterialLoader>("assets/material/test-material.material", [&materialID](Core::BaseAssetLoader* baseLoader, Core::AssetHandle handle)
            {
                Core::MaterialData* data = static_cast<Core::MaterialData*>(handle);
                materialID = (unsigned int)data->materialId;
            }, false);

	for (int i = -10; i < 10; i++)
	{
		for (int j = -10; j < 10; j++)
		{
			Core::Entity e2 = Core::world.m_entityHandler.CreateEntity<Core::GraphicsComponent, Core::WorldPositionComponent, Core::RotationComponent, Core::ScaleComponent>
				(Core::GraphicsComponent(), Core::WorldPositionComponent(), Core::RotationComponent(), Core::ScaleComponent());
	
			Core::GraphicsComponent* gc = WGETC<Core::GraphicsComponent>(e2);
			
			GFX::SetBitmaskValue(gc->bitmask, GFX::BITMASK::TYPE, GFX::OBJECT_TYPES::OPAQUE_GEOMETRY);
			GFX::SetBitmaskValue(gc->bitmask, GFX::BITMASK::MESH_ID, meshID);
			
			Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(e2);
			wpc->position[0] = (float)(i * 20);
			wpc->position[1] = (float)(j * 20);
	
			Core::ScaleComponent* sc = WGETC<Core::ScaleComponent>(e2);
			sc->scale[0] = .10f;
			sc->scale[1] = .10f;
			sc->scale[2] = .10f;
	
			Core::RotationComponent* rc = WGETC<Core::RotationComponent>(e2);
			rc->rotation[2] = sin(3.14f);
			rc->rotation[3] = cos(3.14f / 2.0f);
		}
	}
	for (int i = 0; i < 4096; i++)
	{
		Core::Entity light = Core::world.m_entityHandler.CreateEntity<Core::LightComponent, Core::WorldPositionComponent, Core::RotationComponent, Core::ScaleComponent>
				(Core::LightComponent(), Core::WorldPositionComponent(), Core::RotationComponent(), Core::ScaleComponent());
	
			Core::LightComponent* lc = WGETC<Core::LightComponent>(light);
	
			lc->color[0] = (rand()&1000)/1000.0f;
            lc->color[1] = (rand()&1000)/1000.0f;
            lc->color[2] = (rand()&1000)/1000.0f;

			lc->intensity = 1.3f;
	
			// Set the the light component to new point light 
			GFX::SetBitmaskValue(lc->bitmask, GFX::BITMASK::TYPE, GFX::OBJECT_TYPES::LIGHT);
			GFX::SetBitmaskValue(lc->bitmask, GFX::BITMASK::LIGHT_TYPE, GFX::LIGHT_TYPES::POINT);
	
			Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(light);
			wpc->position[0] = (float)(-25.0f + 50.0f * (rand()&1000)/1000.0f);
			//wpc->position[1] = (float)(-400.0f + 600.0f * (rand()&1000)/1000.0f);
			wpc->position[2] = (float)(-250.0f + 500.0f * (rand() & 1000) / 1000.0f);
			Core::ScaleComponent* sc = WGETC<Core::ScaleComponent>(light);
			sc->scale[0] = sc->scale[1] = sc->scale[2] = 10.0f +5.0f * (rand() & 1000) / 1000.0f;
	
			Core::RotationComponent* rc = WGETC<Core::RotationComponent>(light);
	}
}
