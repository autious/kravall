local scenario = require "scenario"
local input = require "input" 
local scen = scenario.new()
local entities = dofile"scripts/scenarios/minecart/my_entities.lua"
local util = dofile"scripts/scenarios/minecart/my_utils.lua"
local track = dofile"scripts/scenarios/minecart/my_tracks.lua"

local mouse = core.input.mouse
local keyboard = core.input.keyboard
local key = keyboard.key
local camera = require "rts_camera".new()
camera:lookAt( core.glm.vec3.new( 0, 30, 50 ), core.glm.vec3.new( 0, 0, -50 ) )


-- Game Variables
local currentLane = 2
local lanePos = { -17, 0, 17} -- Precalculated positions for the lanes positions
local currentTileIndex = 0

local numTracks = 8 -- Number of tracks to draw in each lane
local trackCO = 40 -- The position at which tracks reset (behind camera)
local trackLength = 100 -- Length of each track piece

local numCaveSegs = 4 -- Number of tracks to draw in each lane
local caveSegLength = 200 -- Length in units for a cave segment

local cartResetSpeed = 75.0
local cartMaxSpeed = 275.0
local cartSpeed = cartResetSpeed -- Speed of the minecart

local score = 0



-- Ambient light
local ambientlight = entities.CreateAmbientlight(scen)

-- Plane
--local plane = entities.CreateBaseplane(scen)

-- Debug light
local dblight = entities.CreatePointlight({0, 10, 0}, {1, 0, 0}, 100, 100, scen)
util.SetLightIntensity(0, dblight)

-- Create the minecart
local minecart = {
	cart = entities.CreateMinecart(scen),
	spotlight = entities.CreateHeadlight(scen),
	goldlight1 = entities.CreatePointlight({1, 9, -1}, {1, 1, 0}, 6, 10, scen),
	goldlight2 = entities.CreatePointlight({0, 8, 0}, {1, 1, 0}, 6, 10, scen),
	goldlight3 = entities.CreatePointlight({-1, 7, 1}, {1, 1, 0}, 6, 10, scen),
	moodlight = entities.CreatePointlight({0, 15, -7}, {255/255, 90/255, 0/255}, 75, 2, scen),
	position = {0, 0, 0}
	
}

local function UpdateCartPosition(delta)
	util.SetPosition(
		{
			minecart.position[1] + 0.0, 
			minecart.position[2] + 0.0, 
			minecart.position[3] + 0.0
		}, minecart.cart)
		
	util.SetPosition(
		{
			minecart.position[1] + 0.0, 
			minecart.position[2] + 10.0, 
			minecart.position[3] - 5.0
		}, minecart.spotlight)
		
	util.SetPosition(
		{
			minecart.position[1] + 1.0, 
			minecart.position[2] + 9.0, 
			minecart.position[3] - 1.0
		}, minecart.goldlight1)
		
	util.SetPosition(
		{
			minecart.position[1] + 0.0, 
			minecart.position[2] + 8.0, 
			minecart.position[3] + 0.0
		}, minecart.goldlight2)
		
	util.SetPosition(
		{
			minecart.position[1] + -1.0, 
			minecart.position[2] + 7.0, 
			minecart.position[3] + 1.0
		}, minecart.goldlight3)
		
	util.SetPosition(
		{
			minecart.position[1] + 0, 
			minecart.position[2] + 15, 
			minecart.position[3] - 7
		}, minecart.moodlight)
end

local function Overlap(a, b, c)
	local a1 = a[1]
	local a2 = a[2]
	local b1 = b[1]
	local b2 = b[2]
	local c1 = c[1]
	local c2 = c[2]
	
	-- a overlap b
	if 		a1 <= b2 and b1 <= a2 
	-- a overlap c
	and 	a1 <= c2 and c1 <= a2 
	-- b overlap c
	and 	b1 <= c2 and b1 <= c2 
	then return true 
	else return false end
end

local function RandomFunc()
	local fac = 15
	local rmax = fac * (500 / (score + 500))
	local rnd = math.random(1,3+rmax)
	return rnd
end

-- Function for randomizing tracks
local function RandomTrack(i)
	local e1, k1 = track.CreateTrack({lanePos[1], 0, trackCO-i*trackLength}, RandomFunc(), scen)
	local e2, k2 = track.CreateTrack({lanePos[2], 0, trackCO-i*trackLength}, RandomFunc(), scen)
	local e3, k3 = track.CreateTrack({lanePos[3], 0, trackCO-i*trackLength}, RandomFunc(), scen)
	
	local left	= {entity = e1, killzone = k1}
	local mid	= {entity = e2, killzone = k2}
	local right	= {entity = e3, killzone = k3}
	
	-- Check so that killzones are not overlapping
	if Overlap(left.killzone, mid.killzone, right.killzone) then 
		local id = math.random(1,3)
		if id == 1 then
			left.entity:destroy()
			left.entity, left.killzone = track.CreateTrack({lanePos[1], 0, trackCO-i*trackLength}, 1, scen)
		elseif id == 2 then
			mid.entity:destroy()
			mid.entity, mid.killzone = track.CreateTrack({lanePos[2], 0, trackCO-i*trackLength}, 1, scen)
		else
			right.entity:destroy()
			right.entity, right.killzone = track.CreateTrack({lanePos[3], 0, trackCO-i*trackLength}, 1, scen)
		end
	end
	
	return 	left, mid, right
end

-- Create tracks
local tracks = {}

-- Create a few tracks to begin with
-- Left track
for i=1,numTracks do
	left, mid, right = RandomTrack(i)
	tracks[i] = {
					left = left,
					mid = mid,
					right = right,
					offset = trackCO-i*trackLength
				}
end

local function UpdateTracks(delta)
	local position = {0, 0, 0}
	for i=1,numTracks do
		-- Update left track
		tracks[i].offset = tracks[i].offset + delta * cartSpeed
		
		if tracks[i].offset > trackLength then 
			tracks[i].left.entity:destroy()
			tracks[i].mid.entity:destroy()
			tracks[i].right.entity:destroy()
			
			tracks[i].offset = tracks[i].offset -numTracks * trackLength 
			tracks[i].left, tracks[i].mid, tracks[i].right = RandomTrack(i)
		end
		
		-- Left
		position = util.GetPosition(tracks[i].left.entity)
		position[3] = tracks[i].offset
		util.SetPosition(position, tracks[i].left.entity)
		
		-- Mid
		position = util.GetPosition(tracks[i].mid.entity)
		position[3] = tracks[i].offset
		util.SetPosition(position, tracks[i].mid.entity)
		
		-- Right
		position = util.GetPosition(tracks[i].right.entity)
		position[3] = tracks[i].offset
		util.SetPosition(position, tracks[i].right.entity)
		
		if tracks[i].offset >= -trackLength and tracks[i].offset < 0  then
			currentTileIndex = i
		end
		
	end
end

-- Create cave
local cave = {}

for i=1,numCaveSegs do
	cave[i] = {
	entity = entities.CreateCavePiece({0, 0, caveSegLength-i*caveSegLength}, math.floor(math.random()*4)+1, scen), 
	offset = caveSegLength-i*caveSegLength}
end

local function UpdateCave(delta)
	for i=1,#cave do
		local position = util.GetPosition(cave[i].entity)
		cave[i].offset = cave[i].offset + delta * cartSpeed
		if cave[i].offset > caveSegLength then 
			cave[i].offset = cave[i].offset -numCaveSegs * caveSegLength 
			cave[i].entity:destroy()
			cave[i].entity = entities.CreateCavePiece({0, 0, caveSegLength-i*caveSegLength}, math.floor(math.random()*4)+1, scen)
		end
		position[3] = cave[i].offset
		util.SetPosition(position, cave[i].entity)
	end
end

-- Function for detecting if the minecart is in a killzone or not
local function IsKillzoned()
	-- Get the current tile for the current lane
	local trk
	local i = currentTileIndex
	
	if currentLane == 1 then -- left
		trk = tracks[i].left
	elseif currentLane == 2 then -- mid
		trk = tracks[i].mid
	elseif currentLane == 3 then -- right
		trk = tracks[i].right
	else
		return false
	end
	
	if trk.killzone[1] >= 0 and trk.killzone[2] >= 0 then -- killzone is not nil
		if trackLength + tracks[i].offset < trk.killzone[2]  and trackLength + tracks[i].offset > trk.killzone[1] then
			return true
		end
	end
	return false
end






-- Init function
local function Init()
	core.camera.gameCamera:setView( camera:getView( ) )
	core.camera.gameCamera:setProjection( camera:getProjection( ) )
end
scen:registerInitCallback( Init )

-- Update function
local alive = true
local invincitime = 3
local prevKeysPressed 	= {false, false, false, false }
local keysPressed 		= {false, false, false, false }
local switchingLane = false
local switchingTimeConstant = 0.2
local switchingTime = 0
local pendingLane = 2
local function Update(delta)
	-- Logic for keyboard input
	keysPressed[1]	= keyboard.isKeyDown( key.Up )
	keysPressed[2]	= keyboard.isKeyDown( key.Left )
	keysPressed[3] 	= keyboard.isKeyDown( key.Right )
	keysPressed[4] 	= keyboard.isKeyDown( key.Enter )
	
	for i=1,#keysPressed do
		if keysPressed[i] == true then
			if prevKeysPressed[i] == true then keysPressed[i] = false end
			prevKeysPressed[i] = true
		else prevKeysPressed[i] = false end
	end
	
	-- Logic for updating tracks and cave
	UpdateTracks(delta)
	UpdateCave(delta)
	
	-- Do stuff if dead and whatnot
	if alive == true then
	
		-- Logic for switching lanes and setting minecart position
		if keyboard.isKeyDown( key.Left ) == true and switchingLane == false then 
			pendingLane = currentLane - 1 
			if pendingLane < 1 then 
				pendingLane = 1 
			else
				switchingLane = true
				switchingTime = switchingTimeConstant
			end
		end
		if keyboard.isKeyDown( key.Right ) == true and switchingLane == false then 
			pendingLane = currentLane + 1 
			if pendingLane > 3 then 
				pendingLane = 3 
			else
				switchingLane = true
				switchingTime = switchingTimeConstant
			end 
		end
		
		if switchingLane == true then
			switchingTime = switchingTime - delta
			if switchingTime > 0 then
				local targetPos = lanePos[pendingLane]
				local factor = switchingTime / switchingTimeConstant
				minecart.position[1] =   factor * lanePos[currentLane] +   (1 - factor) * targetPos
				minecart.position[2] = 15 * math.sin(math.pi * factor )
				if pendingLane < currentLane then
					util.SetRotationZ(-math.pi*2 * factor, minecart.cart)
				else
					util.SetRotationZ(math.pi*2 * factor, minecart.cart)
				end
			else
				currentLane = pendingLane
				minecart.position[1] = lanePos[currentLane]
				minecart.position[2] = 0
				util.SetRotationZ(0, minecart.cart)
				switchingTime = 0
				switchingLane = false
			end
		end
		
		UpdateCartPosition(delta)
		score = score + cartSpeed * delta / 20
		
		core.draw.drawText( 300, 35, "METERS TRAVELLED: "..math.floor(score) )
		
		-- Die and stuff
		if IsKillzoned() and invincitime < 0 and pendingLane == currentLane then 
			util.SetLightIntensity(100, dblight) 
			alive = false
		else 
			util.SetLightIntensity(0, dblight) 
		end
		
		cartSpeed = cartSpeed + 0.05
		
		if cartSpeed > cartMaxSpeed then cartSpeed = cartMaxSpeed end

		invincitime = invincitime - delta
		if invincitime < 0.0 then 
			invincitime = -1 
		elseif 10*invincitime - math.floor(10*invincitime) > 0.5 then
			util.SetPosition(
			{
				minecart.position[1] + 0.0, 
				minecart.position[2] + 0.0, 
				1000
			}, minecart.cart)
		else
			util.SetPosition(
			{
				minecart.position[1] + 0.0, 
				minecart.position[2] + 0.0, 
				0
			}, minecart.cart)
		end
	
	else
		minecart.position[3] = minecart.position[3] + cartSpeed * delta
		UpdateCartPosition(delta)
		cartSpeed = cartSpeed - 20 * delta
		if cartSpeed < 0.1 then cartSpeed = 0 end
		
		core.draw.drawText( 605, 300, "YOU DED!" )
		core.draw.drawText( 540, 340, "FINAL SCORE: "..math.floor(score).." METERS!" )
		core.draw.drawText( 590, 380, "PRESS ENTER!" )
		
		-- Logic for restarting game
		if keysPressed[4] == true then 
			alive = true 
			minecart.position[3] = 0
			invincitime = 3
			cartSpeed = cartResetSpeed
			score = 0
			currentLane = 2
			pendingLane = 2
			minecart.position[1] = lanePos[currentLane]
			minecart.position[2] = 0
			util.SetRotationZ(0, minecart.cart)
			switchingTime = 0
			switchingLane = false
		end
		
	end
	camera:update(delta)
end
scen:registerUpdateCallback( Update )

return scen;
