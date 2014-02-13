local function CreateTrack(position, tracktype, scen)
	local path
	local killzone
	if tracktype == 2 then
		path = "assets/minecart/track_destroyed_01.bgnome"
		killzone = {-3*10, -0.3*10}
	elseif tracktype == 3 then
		path = "assets/minecart/track_destroyed_02.bgnome"
		killzone = {0.3*10, 2.5*10}
	else 
		path = "assets/minecart/track_regular.bgnome"
		killzone = {10000, 10000}
	end
	
	local rot = {0, 0, 0, 1}
	-- rotate a track around for extra randomness
	if math.random(0,1) == 1 then
		rot = { 0, math.sin( math.pi/2 ), 0, math.cos(math.pi/2) }
		
		killzone[1], killzone[2] = -killzone[2], -killzone[1]
		
		--local kz1 = killzone[1]
		--local kz2 = killzone[2]
		--killzone[1] = -kz2
		--killzone[2] = -kz1
	end
	return scen:loadAssembly( 
	{
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = position }
		},
		{
			type = core.componentType.GraphicsComponent,
			data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1},render = true },
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
			data = { rotation = rot }
		}
	}
	), killzone
end

local interface = { 
	CreateTrack = CreateTrack
 }
return interface