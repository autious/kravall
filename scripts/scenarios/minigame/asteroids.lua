local entities = require "entities"
local scenario = require "scenario"
local input = require "input" 
local scen = scenario.new()

local mouse = core.input.mouse
local keyboard = core.input.keyboard
local key = keyboard.key
local camera = require "rts_camera".new()
camera:lookAt( core.glm.vec3.new( 0, 200, 0 ), core.glm.vec3.new( 0, 0, 0 ) )
core.camera.gameCamera:setView( camera:getView( ) )
core.camera.gameCamera:setProjection( camera:getProjection( ) )


--Spaceship
local spaceship = scen:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 30, 10, -10 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/asteroidship.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/asteroids/roids_white.material", false }
			   }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 1.5 }
	},
	{
		type = core.componentType.RotationComponent,
		--data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
		data = { rotation = { 0, 1*math.sin( -3.14/4 ), 0, math.cos( -3.14/4 ) } }
	}
}
)
-- Directional light
--scen:loadAssembly( 
--{
--	{
--		type = core.componentType.LightComponent,
--		data =  { 
--					color = { 0.3, 0.3, 0.3 },
--					speccolor = { 0.1, 0.3, 1.0 },
--					intensity = 0.5,
--					type = core.gfx.objectTypes.Light,
--					lighttype = core.gfx.lightTypes.Dir
--				}
--	},
--	{
--		type = core.componentType.WorldPositionComponent,
--		data = { position = { 0, 0, 0 } }
--	},
--	{
--		type = core.componentType.ScaleComponent,
--		data = { scale = 1.0 } 
--	},
--	{
--		type = core.componentType.RotationComponent,
--		data = { rotation = { 2,-3,-1,0 } } -- Lights uses rotation component as a direction vector, not a quaternion
--	}
--} 
--)
-- Ambient light
--scen:loadAssembly( 
--{
--	{
--		type = core.componentType.LightComponent,
--		data =  { 
--					color = { 1.0, 1.0, 1.0 },
--					intensity = 0.01,
--					type = core.gfx.objectTypes.Light,
--					lighttype = core.gfx.lightTypes.Ambient
--				}
--	},
--	{
--		type = core.componentType.WorldPositionComponent,
--		data = { position = { 0, 0, 0 } }
--	},
--	{
--		type = core.componentType.ScaleComponent,
--		data = { scale = 1.0 }
--	},
--	{
--		type = core.componentType.RotationComponent,
--		data = { rotation = { 0,0,0,0 } }
--	}
--} 
--)
--Plane
scen:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, 10, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/plane.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/asteroids/roids_black.material", false }
			   }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 150.0 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { -math.sin(math.pi/4.0), 0, 0, math.cos(math.pi/4.0) } }
	}
}
)--Background Plane
scen:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, 5, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/plane.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/asteroids/roids_white.material", false }
			   }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 160.0 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { -math.sin(math.pi/4.0), 0, 0, math.cos(math.pi/4.0) } }
	}
}
)

local thrustLight = scen:loadAssembly( 
{
	{
		type = core.componentType.LightComponent,
		data =  { 
					color = {1.0, 0.4, 0.0},
					speccolor = {1.0, 0.4, 0.0},
					intensity = 15.0,
					spotangle = 3.14/4.0,
					spotpenumbra = 0.1,
					type = core.gfx.objectTypes.Light,
					lighttype = core.gfx.lightTypes.Spot
				}
	},
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, -1000, 0 } }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 25.0 } 
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { 1, -1, 0, 0 } } -- Lights uses rotation component as a direction vector, not a quaternion
	}
} 
)

local function CreateBullet()
	return scen:loadAssembly( 
	{
		{
			type = core.componentType.LightComponent,
			data =  { 
						color = {0.3 + math.random()*0.7, 0.3 + math.random()*0.7,  0.3 + math.random()*0.7},
						speccolor = {0.3 + math.random()*0.7, 0.3 + math.random()*0.7,  0.3 + math.random()*0.7},
						intensity = 0.5,
						spotangle = 0.0,
						spotpenumbra = 0.0,
						type = core.gfx.objectTypes.Light,
						lighttype = core.gfx.lightTypes.Point
					}
		},
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = { 0, 0, 0 } }
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = 10.0 } 
		},
		{
			type = core.componentType.RotationComponent,
			data = { rotation = { 1, 0, 0, 0 } } -- Lights uses rotation component as a direction vector, not a quaternion
		}
	} 
	)
end

local function CreateAsteroid(scale, position)
	return scen:loadAssembly( 
	{
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = { position[1], 0, position[2] } }
		},
		{
			type = core.componentType.GraphicsComponent,
			data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
			load = { 
						mesh = { core.loaders.GnomeLoader, "assets/sphere.bgnome", false },
						material = { core.loaders.MaterialLoader, "assets/material/asteroids/roids_white.material", false }
				   }
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = scale }
		},
		{
			type = core.componentType.RotationComponent,
			--data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
			data = { rotation = { 1*math.sin( -3.14/4 ), 0, 0, math.cos( -3.14/4 ) } }
		}
	}
	)
end



local showTutorial = true
local enterWasDown = true
local started = false
local level = 0
local difficulty = 3
local score = 0
local maxLives = 3
local lives = maxLives
local alive = true
local t = 0
local playerX = 0
local playerY = 0
local velocity = {0,0}
local playerDirection = 0
local shootTimer = 0

local acc = 40.7
local decc = 0.01
local turnSpeed = 4.5
local topSpeed = 120
local boundX = 75
local boundY = 75
local shootInterval = 0.2
local bulletSpeed = 120
local asteroidSpeed = 5

local bullets = {}
local asteroids = {}

local function InitAsteroids(n)
	local size = 3
	local minSpawn = 45
	local maxSpawn = (boundX + boundY) / 2 - minSpawn
	for i=1,n do
		local randVec = {1 - 2 * math.random(), 1 - 2 * math.random()}
		local randDist = minSpawn + math.random() * maxSpawn
		
		
		if randVec[1] == 0 and randVec[2] == 0 then
			randVec[1] = 1
		end
		
		-- normalize
		local length = math.sqrt(randVec[1]*randVec[1] + randVec[2]*randVec[2])
		randVec[1] = randVec[1] / length
		randVec[2] = randVec[2] / length
		
		randVec[1] = randVec[1] * randDist
		randVec[2] = randVec[2] * randDist
		
		
		table.insert(asteroids, {entity = CreateAsteroid(5*size, randVec), direction = math.pi * 2 * math.random(), size = size})
	end
end

InitAsteroids(difficulty)

local function RestartGame(diff)
	-- Clear tables
	for i=1,#asteroids do
		asteroids[i].entity:destroy()
		asteroids[i] = nil
	end
	for i=1,#bullets do
		bullets[i].entity:destroy()
		bullets[i] = nil
	end
	
	-- Reset variables
	alive = true
	t = 0
	playerX = 0
	playerY = 0
	velocity = {0,0}
	playerDirection = 0
	shootTimer = 0
	difficulty = diff
	
	--Create asteroids
	InitAsteroids(difficulty)
end


local function Shoot(direction, position)
	table.insert(bullets, { entity = CreateBullet(), direction = direction })
	local wpc;
	wpc = bullets[#bullets].entity:get(core.componentType.WorldPositionComponent)
	wpc.position = position
	bullets[#bullets].entity:set(core.componentType.WorldPositionComponent, wpc)
end

local function bulletHitAsteroid(bulletID)
	local i = 1
	wasHit = false
	bulletPos = bullets[bulletID].entity:get(core.componentType.WorldPositionComponent).position
	while i <= #asteroids do
		wpc = asteroids[i].entity:get(core.componentType.WorldPositionComponent)
		position = {wpc.position[1] - bulletPos[1], wpc.position[3] - bulletPos[3]}
		distance = math.sqrt(position[1]*position[1] + position[2]*position[2])
		if distance < 5 then
			asteroids[i].entity:destroy()
			size = asteroids[i].size
			table.remove(asteroids, i)
			if size > 1 then
				size = size-1
				table.insert(asteroids, {entity = CreateAsteroid(5*size, {bulletPos[1], bulletPos[3]}), direction = math.pi * 2 * math.random(), size = size})
				table.insert(asteroids, {entity = CreateAsteroid(5*size, {bulletPos[1], bulletPos[3]}), direction = math.pi * 2 * math.random(), size = size})
			end
			wasHit = true
			score = score + 10
			break
		else
			i = i + 1
		end
	end
	return wasHit
end

local function UpdateBullets(delta)
	local i = 1
	while i <= #bullets do
	
		local wpc
		wpc = bullets[i].entity:get(core.componentType.WorldPositionComponent)
		position = wpc.position
		
		if position[1] < -boundX or position[1] > boundX 
		or position[2] < -boundY or position[2] > boundY
		or bulletHitAsteroid(i) then
			-- Remove bullet outside of bounds
			bullets[i].entity:destroy()
			table.remove(bullets, i)
		else
			wpc.position[1] = wpc.position[1] + bulletSpeed * math.cos(bullets[i].direction) * delta
			wpc.position[2] = 10.5
			wpc.position[3] = wpc.position[3] + bulletSpeed * math.sin(bullets[i].direction) * delta
			bullets[i].entity:set(core.componentType.WorldPositionComponent, wpc)
			i = i + 1
		end
	end
end


local function UpdateAsteroids(delta)
	local i = 1
	spaceshipPos = spaceship:get(core.componentType.WorldPositionComponent).position
	while i <= #asteroids do
	
		local wpc
		wpc = asteroids[i].entity:get(core.componentType.WorldPositionComponent)
		position = wpc.position
		
		collidePos = {position[1] - spaceshipPos[1], position[3] - spaceshipPos[3]}
		distance = math.sqrt(collidePos[1]*collidePos[1] + collidePos[2]*collidePos[2])
		if distance < asteroids[i].size * 2.5 then
			alive = false
		end
		
		if position[1] < -boundX then
			position[1] = boundX
		elseif position[1] > boundX then
			position[1] = -boundX
		end 
		if position[3] < -boundY then
			position[3] = boundY
		elseif position[3] > boundY then
			position[3] = -boundY
		end
		
		wpc.position[1] = position[1] + (8-2*asteroids[i].size) * asteroidSpeed * math.cos(asteroids[i].direction) * delta
		wpc.position[2] = 11.0
		wpc.position[3] = position[3] + (8-2*asteroids[i].size) * asteroidSpeed * math.sin(asteroids[i].direction) * delta
		asteroids[i].entity:set(core.componentType.WorldPositionComponent, wpc)
		
		i = i + 1
	end
end

local function Move(delta)
	t = (t + delta * 0.5)
	
	-- Thrusting
	local thrusting = keyboard.isKeyDown( key.Up )
	local leftTurn = keyboard.isKeyDown( key.Left )
	local rightTurn = keyboard.isKeyDown( key.Right )
	local shooting = keyboard.isKeyDown( key.Left_shift )
	
	local enterIsDown = false
	if keyboard.isKeyDown( key.Enter ) then
		if not enterWasDown then
			enterIsDown = true
		end
		enterWasDown = true
	else
		enterWasDown = false
	end
	
	UpdateAsteroids(delta)
	
	-- Insert coin
	if not started then
		local wpc;
		wpc = spaceship:get(core.componentType.WorldPositionComponent)
		wpc.position[1] = playerX
		wpc.position[2] = -1000.0
		wpc.position[3] = playerY
		spaceship:set(core.componentType.WorldPositionComponent, wpc)
		core.draw.drawText( 580, 340, "INSERT COIN")
		if enterIsDown then
			started = true
			enterIsDown = false
			showTutorial = true
		else
			return
		end
	end
	
	-- Draw tutorial
	if showTutorial then
		core.draw.drawText( 605, 300, "CONTROLS:" )
		core.draw.drawText( 598, 340, "ARROWS FLY" )
		core.draw.drawText( 568, 380, "LSHIFT SHOOT LASER" )
		if enterIsDown then
			showTutorial = false
			RestartGame(difficulty)
		else
			return
		end
	end
	
	-- You are dead!
	if not alive then
		local wpc;
		wpc = spaceship:get(core.componentType.WorldPositionComponent)
		wpc.position[1] = playerX
		wpc.position[2] = -1000.0
		wpc.position[3] = playerY
		spaceship:set(core.componentType.WorldPositionComponent, wpc)
		print("ded")
		
		thrusting = false
		leftTurn = false
		rightTurn = false
		shooting = false
		if lives == 0 then
			core.draw.drawText( 605, 300, "YOU DED!" )
			core.draw.drawText( 580, 340, "FINAL SCORE: "..score )
			core.draw.drawText( 590, 380, "PRESS ENTER!" )
			if enterIsDown then
				lives = maxLives
				difficulty = 3
				score = 0
				lives = maxLives
				level = 0
				started = false
				RestartGame(difficulty)
				return
			end
		else
			if lives > 1 then
				core.draw.drawText( 540, 300, "TRY AGAIN..."..lives.." LIVES LEFT" )
			else
				core.draw.drawText( 545, 300, "TRY AGAIN..."..lives.." LIFE LEFT" )
			end
			core.draw.drawText( 590, 340, "PRESS ENTER!" )
			if enterIsDown then
				lives = lives - 1
				alive = true
				RestartGame(difficulty)
				return
			end
		end
	end
	
	shootTimer = shootTimer - delta
	if shootTimer < 0 then shootTimer = 0 end
	if shooting and shootTimer == 0 then 
		shootTimer = shootInterval
	else 
		shooting = false 
	end
	
	local flamePos = thrustLight:get(core.componentType.WorldPositionComponent)
	local flameRot = thrustLight:get(core.componentType.RotationComponent)
	
	if thrusting then
		velocity[1] = velocity[1] + math.cos(playerDirection) * acc * delta
		velocity[2] = velocity[2] + math.sin(playerDirection) * acc * delta
		
		flamePos.position[1] = playerX
		flamePos.position[2] = 12.5
		flamePos.position[3] = playerY
		thrustLight:set(core.componentType.WorldPositionComponent, flamePos)
		
	else
		velocity[1] = velocity[1] * 0.998
		velocity[2] = velocity[2] * 0.998
		
		flamePos.position[1] = playerX
		flamePos.position[2] = -1000.0
		flamePos.position[3] = playerY
		thrustLight:set(core.componentType.WorldPositionComponent, flamePos)
	end
	
	local speed = math.sqrt(velocity[1] * velocity[1] + velocity[2] * velocity[2])
	if speed > topSpeed then
		velocity[1] = topSpeed * (velocity[1] / speed)
		velocity[2] = topSpeed * (velocity[2] / speed)
	elseif speed < 0.001 then
		velocity[1] = 0
		velocity[2] = 0
	end
	local flameLight = thrustLight:get(core.componentType.LightComponent)
	-- Flicker light
	
	flameLight.spotangle = (1.5-(speed/topSpeed))*(0.5 + 0.5 * math.random()) * 3.14/8.0
	thrustLight:set(core.componentType.LightComponent, flameLight)
	
	-- Turning
	
	if leftTurn then
		playerDirection = playerDirection - turnSpeed * delta
	end
	
	if rightTurn then
		playerDirection = playerDirection + turnSpeed * delta
	end
	
	flameRot.rotation[1] = math.cos(math.pi + playerDirection)
	flameRot.rotation[2] = -0.5
	flameRot.rotation[3] = math.sin(math.pi + playerDirection)
	flameRot.rotation[4] = 0
	thrustLight:set(core.componentType.RotationComponent, flameRot)
	
	if playerDirection < 0 then
		playerDirection = 2 * math.pi
	elseif playerDirection > 2 * math.pi then
		playerDirection = 0
	end
	
	-- Bounds
	if playerX > boundX then
		playerX = -boundX
	elseif playerX < -boundX then
		playerX = boundX
	end
	
	if playerY > boundY then
		playerY = -boundY
	elseif playerY < -boundY then
		playerY = boundY
	end
	
	
	playerX = playerX + velocity[1] * delta
	playerY = playerY + velocity[2] * delta
	
	
	if shooting then
		Shoot(playerDirection, {playerX, 12.5, playerY})
	end
	
	UpdateBullets(delta)
	
	
	local wpc;
    wpc = spaceship:get(core.componentType.WorldPositionComponent)
	wpc.position[1] = playerX
	wpc.position[2] = 10.0
	wpc.position[3] = playerY
	spaceship:set(core.componentType.WorldPositionComponent, wpc)
	
    local rc = spaceship:get(core.componentType.RotationComponent)
	rc.rotation[1] = 0
	rc.rotation[2] = math.sin( (math.pi/2 - playerDirection)/2 )
	rc.rotation[3] = 0
	rc.rotation[4] = math.cos( (math.pi/2 - playerDirection)/2 )
	spaceship:set(core.componentType.RotationComponent, rc)
	
	
	
	if #asteroids == 0 then
		core.draw.drawText( 540, 300, "CONGRADULAZION, YOU WIN!" )
		core.draw.drawText( 590, 340, "PRESS ENTER!" )
		if enterIsDown then
			difficulty = difficulty + 1
			level = level + 1
			RestartGame(difficulty)
			return
		end
	end
	
	core.draw.drawText( 300, 35, "LEVEL: "..level )
	core.draw.drawText( 300, 55, "SCORE: "..score )
	core.draw.drawText( 300, 75, "EXTRA LIFE: "..lives )
	
	
	
end

scen:registerUpdateCallback( Move )


return scen;
