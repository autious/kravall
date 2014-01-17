local r = 0.94
local g = 0.89
local b = 0.69
local posY = 5
local size = 15

return function(asm, posX, posZ, strength)
	asm:loadAssembly( 
		{
			{
				type = core.componentType.LightComponent,
				data =  { 
							color = { r, g, b },
							intensity = strength,
							type = core.gfx.objectTypes.Light,
							lighttype = core.gfx.lightTypes.Point,
                            speccolor = { r,g,b }
						}
			},
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { posX, posY, posZ } }
			},
			{
				type = core.componentType.ScaleComponent,
				data = { scale = 10.0 }
			},
			{
				type = core.componentType.RotationComponent,
				data = { rotation = { 0, 0, 0, 0 } }
			}
		} 
    )
end
