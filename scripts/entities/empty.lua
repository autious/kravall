return function(scen, name, posX, posY, posZ, rotX, rotY, rotZ, rotW, scaleX, scaleY, scaleZ)
	posX = posX or 0
	posY = posY or 0
	posZ = posZ or 0
	rotX = rotX or 0
	rotY = rotY or 0
	rotZ = rotZ or 0
	scaleX = scaleX or 1
	scaleY = scaleY or 1
	scaleZ = scaleZ or 1
	
	components = {
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = { posX, posY, posZ} }
		},
		{
			type = core.componentType.RotationComponent,
			data = { rotation = { rotX, rotY, rotZ, rotW } }
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = { scaleX, scaleY, scaleZ } }
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
