return function(asm, dirX, dirY, dirZ)
	asm:loadAssembly( 
		{
			{
				type = core.componentType.LightComponent,
				data =  { 
							color = { 0.5, 0.5, 0.5},
							intensity = 0.5,
							type = core.gfx.objectTypes.Light,
							lighttype = core.gfx.lightTypes.Dir,
                            speccolor = { r,g,b }
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
