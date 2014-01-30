return function(asm, dirX, dirY, dirZ, r, g, b, intens)
	r = r or 1
	g = g or 1
	b = b or 1
	intens = intens or 1
	asm:loadAssembly(
		{
			{
				type = core.componentType.LightComponent,
				data =  { 
							color = { r, g, b },
							intensity = intens,
							type = core.gfx.objectTypes.Light,
							lighttype = core.gfx.lightTypes.Dir,
							speccolor = { r, g, b }
						},
						ignoreHard = true
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
				data = { rotation = { dirX, dirY, dirZ, 0 } } -- Lights uses rotation component as a direction vector, not a quaternion
			}
		} 
	)
end
