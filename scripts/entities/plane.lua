local planeScale = 17
return function(asm, posX, posY, posZ, scale)
    scale = scale or 100
	asm:loadAssembly( 
		{
			{
				type = core.componentType.GraphicsComponent,
				data = { render = true,  mesh = 2000, material = 2000, type = core.gfx.objectTypes.OpaqueGeometry },
				load = { 
							mesh = { core.loaders.GnomeLoader, "assets/plane.bgnome" },
							material = { core.loaders.MaterialLoader, "assets/material/whitePixel.material" }
					   }
			},
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { posX, posY, posZ} }
			},
			{
				type = core.componentType.RotationComponent,
				data = { rotation = { -math.sin(math.pi/4.0), 0, 0, math.cos(math.pi/4.0) } }
			},
			{
				type = core.componentType.ScaleComponent,
				data = { scale = scale }
			},
			{
				type = core.componentType.UnitTypeComponent,
				data = { unitType = core.UnitType.Object }
			},
		}
	)

end
