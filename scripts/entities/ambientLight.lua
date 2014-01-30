return function(asm, r, g, b, intensity)
	r = r or 1
	g = g or 1
	b = b or 1
	intensity = intensity or 0.5
	return asm:loadAssembly( 
		{
			{
				type = core.componentType.LightComponent,
				data =  { 
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
