return function(scen, x, y, z, r, g, b, radius, intens, name)
	local components = {
		{
			type = core.componentType.LightComponent,
			data =  { 
						color = { r, g, b },
						intensity = intens,
						type = core.gfx.objectTypes.Light,
						lighttype = core.gfx.lightTypes.Point,
                        speccolor = { r, g, b }
					},
					ignoreHard = true
		},
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = { x, y, z } }
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = radius }
		},
		{
			type = core.componentType.RotationComponent,
			data = { rotation = { 0, 0, 0, 0 } }
		}
	}

	if name then
        components[ #components + 1 ] =
        {
            type = core.componentType.NameComponent,
            data = { name = name }
        }
    end

	return scen:loadAssembly( components )
end
