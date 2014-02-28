return function(scen, x, y, z, rx, ry, rz, r, g, b, size, penumbra, radius, intens, name )
	local components = {
		{
			type = core.componentType.LightComponent,
			data = 
			{ 
				color = { r, g, b },
                speccolor = { r, g, b },
				intensity = intens,
				spotangle = size,
				spotpenumbra = penumbra,
				type = core.gfx.objectTypes.Light,
				lighttype = core.gfx.lightTypes.Spot,
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
			data = { rotation = { rx, ry, rz, 0 } }
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
