local function CreateTrack(position, tracktype, scen)
	local path
	local killzone
	if tracktype == 2 then
		path = "assets/minecart/track_destroyed_01.bgnome"
		killzone = {2.3*10, 5*10}
	elseif tracktype == 3 then
		path = "assets/minecart/track_destroyed_02.bgnome"
		killzone = {5*10, 8*10}
	else 
		path = "assets/minecart/track_regular.bgnome"
		killzone = {10000, 10000}
	end
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
						mesh = { core.loaders.GnomeLoader, path, false },
						material = { core.loaders.MaterialLoader, "assets/minecart/material/track.material", false }
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
	), killzone
end

local interface = { 
	CreateTrack = CreateTrack
 }
return interface