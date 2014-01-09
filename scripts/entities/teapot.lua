local t = {}

function t.create(asm, posX, posY, posZ)
	asm:loadAssembly( 
		{
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { posX, posY, posZ } }
			},
			{
				type = core.componentType.GraphicsComponent,
				data = { mesh = 2000, material = 2000, type = core.gfx.objectTypes.OpaqueGeometry },
				load = { 
							mesh = { core.loaders.GnomeLoader, "assets/teapot.bgnome" },
							material = { core.loaders.MaterialLoader, "assets/material/test-material.material" }
					   }
			},
			{
				type = core.componentType.ScaleComponent,
				data = { scale = 1.0 }
			},
			{
				type = core.componentType.RotationComponent,
				data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
			},
			{
				type = core.componentType.UnitTypeComponent,
				data = { unitType = core.UnitType.Rioter }
			},
			{
				type = core.componentType.MovementComponent,
				--data = { direction = { -1, 0, -1 }, speed = 5.8, maxSpeed = 50 }
				data = { direction = { 0, 0, 0 }, speed = 5.8, maxSpeed = 50 }
			},
			{
				type = core.componentType.AttributeComponent,
				data = { health = 0, stamina = 0, morale = 0, 
					   alignment = core.RioterAlignment.Anarchist, rage = 0, pressure = 0, groupID = 0 }
			},
			{
				type = core.componentType.BoundingVolumeComponent,
				data = { sphereOffset = { 0, 0, 0 }, sphereRadius = 1.0, 
						collisionModel = core.BoundingVolumeCollisionModel.DynamicResolution, 
						type = core.BoundingVolumeType.SphereBoundingType }
			}
		}
	)
end


--function t.create(asm, posX, posY, posZ, dirx, dirz)
--	asm:loadAssembly( 
--		{
--			{
--				type = core.componentType.WorldPositionComponent,
--				data = { position = { posX, posY, posZ } }
--			},
--			{
--				type = core.componentType.GraphicsComponent,
--				data = { mesh = 2000, material = 2000, type = core.gfx.objectTypes.OpaqueGeometry },
--				load = { 
--							mesh = { core.loaders.GnomeLoader, "assets/teapot.bgnome" },
--							material = { core.loaders.MaterialLoader, "assets/material/test-material.material" }
--					   }
--			},
--			{
--				type = core.componentType.ScaleComponent,
--				data = { scale = 1.0 }
--			},
--			{
--				type = core.componentType.RotationComponent,
--				data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
--			},
--			{
--				type = core.componentType.UnitTypeComponent,
--				data = { unitType = core.UnitType.Rioter }
--			},
--			{
--				type = core.componentType.MovementComponent,
--				--data = { direction = { -1, 0, -1 }, speed = 5.8, maxSpeed = 50 }
--				data = { direction = { dirx, 0, dirz }, speed = 5.8, maxSpeed = 50 }
--			},
--			{
--				type = core.componentType.AttributeComponent,
--				data = { health = 0, stamina = 0, morale = 0, 
--					   alignment = core.RioterAlignment.Anarchist, rage = 0, pressure = 0, groupID = 0 }
--			},
--			{
--				type = core.componentType.BoundingVolumeComponent,
--				data = { sphereOffset = { 0, 0, 0 }, sphereRadius = 1.5, 
--						collisionModel = core.BoundingVolumeCollisionModel.DynamicResolution, 
--						type = core.BoundingVolumeType.SphereBoundingType }
--			}
--		}
--	)
--end

return t