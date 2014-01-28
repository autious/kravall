local boxScale = 3

return function(asm, posX, posZ)
return	asm:loadAssembly( 
		{
			{
				type = core.componentType.GraphicsComponent,
				data = { mesh = 2000, material = 2000, type = core.gfx.objectTypes.OpaqueGeometry },
				load = { 
							mesh = { core.loaders.GnomeLoader, "assets/haus.bgnome" },
							material = { core.loaders.MaterialLoader, "assets/material/brick.material" }
					   }
			},
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { posX, 0, posZ} }
			},
			{
				type = core.componentType.RotationComponent,
				data = { rotation = { 0, 0, 0, 0 } }
			},
			{
				type = core.componentType.ScaleComponent,
				data = { scale = boxScale }
			},
			{
				type = core.componentType.UnitTypeComponent,
				data = { unitType = core.UnitType.Object }
			},
		}
	)

end
