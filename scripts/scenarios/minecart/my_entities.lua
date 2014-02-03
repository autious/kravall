local function CreateMinecart(scen)
	return scen:loadAssembly( 
	{
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = {0, 0, 0} }
		},
		{
			type = core.componentType.GraphicsComponent,
			data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
			load = { 
						mesh = { core.loaders.GnomeLoader, "assets/minecart/minecart.bgnome", false },
						material = { core.loaders.MaterialLoader, "assets/minecart/material/minecart.material", false }
				   }
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = 10 }
		},
		{
			type = core.componentType.RotationComponent,
			--data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
			data = { rotation = { 0, 0, 0, 1 } }
		}
	}
	)
end

local function CreateCavePiece(position, cavetype, scen)
	local ct = math.max(1, math.min(4, cavetype))
	local model = "assets/minecart/cave_0"..ct..".bgnome"
	return scen:loadAssembly( 
	{
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = position }
		},
		{
			type = core.componentType.GraphicsComponent,
			data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
			load = { 
						mesh = { core.loaders.GnomeLoader, model, false },
						material = { core.loaders.MaterialLoader, "assets/minecart/material/cave.material", false }
				   }
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = 10 }
		},
		{
			type = core.componentType.RotationComponent,
			--data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
			data = { rotation = { 0, 0, 0, 1 } }
		}
	}
	)
end

local function CreateHeadlight(scen)
	return scen:loadAssembly( 
	{
		{
			type = core.componentType.LightComponent,
			data =  { 
						color = {255/255, 90/255, 0/255},
						speccolor = {255/255, 90/255, 0/255},
						intensity = 150,
						spotangle = math.pi/4,
						spotpenumbra = 0.3,
						type = core.gfx.objectTypes.Light,
						lighttype = core.gfx.lightTypes.Spot
					}
		},
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = {0, 10, -5} }
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = 550 } 
		},
		{
			type = core.componentType.RotationComponent,
			data = { rotation = { 0, 0, -1, 0 } } -- Lights uses rotation component as a direction vector, not a quaternion
		}
	} 
	)
end

local function CreatePointlight(position, color, size, intensity, scen)
	return scen:loadAssembly( 
	{
		{
			type = core.componentType.LightComponent,
			data =  { 
						color = color,
						speccolor = color,
						intensity = intensity,
						spotangle = 0,
						spotpenumbra = 0,
						type = core.gfx.objectTypes.Light,
						lighttype = core.gfx.lightTypes.Point
					}
		},
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = position }
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = size } 
		},
		{
			type = core.componentType.RotationComponent,
			data = { rotation = { 0, 0, -1, 0 } } -- Lights uses rotation component as a direction vector, not a quaternion
		}
	} 
	)
end

local function CreateAmbientlight(scen)
	return scen:loadAssembly( 
	{
		{
			type = core.componentType.LightComponent,
			data =  { 
						color = { 1.0, 1.0, 1.0 },
						speccolor = { 1.0, 1.0, 1.0 },
						intensity = 0.0,
						spotangle = 0.0,
						spotpenumbra = 0.0,
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

local function CreateBaseplane(scen)
	return scen:loadAssembly( 
	{
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = { 0, -2, 0 } }
		},
		{
			type = core.componentType.GraphicsComponent,
			data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
			load = { 
						mesh = { core.loaders.GnomeLoader, "assets/plane.bgnome", false },
						material = { core.loaders.MaterialLoader, "assets/minecart/material/cave.material", false }
				   }
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = 1350.0 }
		},
		{
			type = core.componentType.RotationComponent,
			data = { rotation = { -math.sin(math.pi/4.0), 0, 0, math.cos(math.pi/4.0) } }
		}
	}
	)
end

local interface = { 
	CreateMinecart = CreateMinecart,
	CreateHeadlight = CreateHeadlight,
	CreatePointlight = CreatePointlight,
	CreateAmbientlight = CreateAmbientlight,
	CreateBaseplane = CreateBaseplane,
	CreateCavePiece = CreateCavePiece
 }
return interface