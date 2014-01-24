return function(asm, r, g, b, intensity)
    print( "in" )
	asm:loadAssembly( 
		{
			{
				type = core.componentType.LightComponent,
				data =  { 
							color = { r, g, b },
							intensity = intensity,
							type = core.gfx.objectTypes.Light,
							lighttype = core.gfx.lightTypes.Ambient
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
				data = { rotation = { 0,0,0,0 } }
			}
		} 
	)
    print( "out" )
end
