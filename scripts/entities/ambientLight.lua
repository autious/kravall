return function(asm, r, g, b, intensity)
	asm:loadAssembly( 
		{
			{
				type = core.componentType.LightComponent,
				data =  { 
							--color = { 1.0, 1.0, 1.0 },
							--intensity = 0.05,
							color = { r, g, b },
							intensity = intensity,
							type = core.gfx.objectTypes.Light,
							lighttype = core.gfx.lightTypes.Ambient
						}
			},
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { 0, 0, 0 } }
			},
			{
				type = core.componentType.ScaleComponent,
				data = { scale = 1.0 }
			},
			{
				type = core.componentType.RotationComponent,
				data = { rotation = { 0,0,0,0 } }
			}
		} 
	)
end
