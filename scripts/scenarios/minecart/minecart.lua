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

local cartSpeed = 75.0 -- Speed of the minecart

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

-- Function for randomizing tracks
local function RandomTrack(i, lane)
	return track.CreateTrack({lanePos[lane], 0, trackCO-i*trackLength}, math.floor(math.random()*10)+1, scen)
end

-- Create tracks
local l_tracks = {}
local m_tracks = {}
local r_tracks = {}
-- Create a few tracks to begin with
-- Left track
for i=1,numTracks do
	ent, kz = RandomTrack(i, 1)
	l_tracks[i] = {entity = ent, killzone = kz, offset = trackCO-i*trackLength}
end
-- Middle track
for i=1,numTracks do
	ent, kz = RandomTrack(i, 2)
	m_tracks[i] = {entity = ent, killzone = kz, offset = trackCO-i*trackLength}
end
-- Right track
for i=1,numTracks do
	ent, kz = RandomTrack(i, 3)
	r_tracks[i] = {entity = ent, killzone = kz, offset = trackCO-i*trackLength}
end

local function UpdateTracks(delta)
	for i=1,numTracks do
		-- Update left track
		local position = util.GetPosition(l_tracks[i].entity)
		l_tracks[i].offset = l_tracks[i].offset + delta * cartSpeed
		if l_tracks[i].offset > trackLength then 
			l_tracks[i].offset = l_tracks[i].offset -numTracks * trackLength 
			l_tracks[i].entity:destroy()
			l_tracks[i].entity, l_tracks[i].killzone = RandomTrack(i, 1)
		end
		position[3] = l_tracks[i].offset
		util.SetPosition(position, l_tracks[i].entity)
		
		-- Update middle track
		local position = util.GetPosition(m_tracks[i].entity)
		m_tracks[i].offset = m_tracks[i].offset + delta * cartSpeed
		if m_tracks[i].offset > trackLength then 
			m_tracks[i].offset = m_tracks[i].offset -numTracks * trackLength 
			m_tracks[i].entity:destroy()
			m_tracks[i].entity, m_tracks[i].killzone = RandomTrack(i, 2)
		end
		position[3] = m_tracks[i].offset
		util.SetPosition(position, m_tracks[i].entity)
		
		-- Update right track
		local position = util.GetPosition(r_tracks[i].entity)
		r_tracks[i].offset = r_tracks[i].offset + delta * cartSpeed
		local curOffset = r_tracks[i].offset
		if r_tracks[i].offset > trackLength then 
			r_tracks[i].offset = r_tracks[i].offset -numTracks * trackLength 
			r_tracks[i].entity:destroy()
			r_tracks[i].entity, r_tracks[i].killzone = RandomTrack(i, 3)
		end
		position[3] = r_tracks[i].offset
		util.SetPosition(position, r_tracks[i].entity)
		
		if curOffset >= -trackLength and curOffset < 0  then
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
		if cave[i].offset > caveSegLength then cave[i].offset = cave[i].offset -numCaveSegs * caveSegLength end
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
		trk = l_tracks[i]
	elseif currentLane == 2 then -- mid
		trk = m_tracks[i]
	elseif currentLane == 3 then -- right
		trk = r_tracks[i]
	else
		return false
	end
	
	if trk.killzone[1] >= 0 and trk.killzone[2] >= 0 then -- killzone is not nil
		if trackLength + trk.offset < trk.killzone[2]  and trackLength + trk.offset > trk.killzone[1] then
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
		if keysPressed[2] == true and switchingLane == false then 
			pendingLane = currentLane - 1 
			if pendingLane < 1 then 
				pendingLane = 1 
			else
				switchingLane = true
				switchingTime = switchingTimeConstant
			end
		end
		if keysPressed[3] == true and switchingLane == false then 
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
		
		
		if IsKillzoned() and invincitime < 0 then 
			util.SetLightIntensity(100, dblight) 
			alive = false
		else 
			util.SetLightIntensity(0, dblight) 
		end
		
		cartSpeed = cartSpeed + 0.05

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
			cartSpeed = 75.0
			score = 0
			currentLane = 2
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
