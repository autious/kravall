return function(asm, posX, posY, posZ, group)
	asm:loadAssembly( 
		{
			{
				type = core.componentType.GraphicsComponent,
				data = { mesh = 2000, material = 2000, type = core.gfx.objectTypes.OpaqueGeometry },
				load = { 
							mesh = { core.loaders.GnomeLoader, "assets/model/animated/police/cop/cop-light_00.bgnome" },
							material = { core.loaders.MaterialLoader, "assets/material/rioter.material" }
					   }
			},
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { posX, posY, posZ} }
			},
			{
				type = core.componentType.RotationComponent,
				data = { rotation = { 0, 0, 0, 0 } }
			},
			{
				type = core.componentType.ScaleComponent,
				data = { scale = 1.0 }
			},
			{
				type = core.componentType.UnitTypeComponent,
				data = { unitType = core.UnitType.Rioter }
			},
			{
				type = core.componentType.MovementComponent,
				data = { direction = { 0, 0, 0 }, newDirection = { 0, 0, 0 }, speed = 5.8, 
				disiredSpeed = 5.8, goal = false }
			},
			{
				type = core.componentType.AttributeComponent,
				data = { health = 0, stamina = 0, morale = 0, 
					   alignment = core.RioterAlignment.Anarchist, rage = 0, pressure = 0, groupID = group }
			},
			{
				type = core.componentType.BoundingVolumeComponent,
				data = { sphereOffset = { 0, 0, 0 }, sphereRadius = 0.7, 
						collisionModel = core.BoundingVolumeCollisionModel.DynamicResolution, 
						type = core.BoundingVolumeType.SphereBoundingType }
			},
			{
				type = core.componentType.FlowfieldComponent,
				data = { node = -1 }
			}
			
		}
	)
end
