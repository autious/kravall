local entities = require "entities"
local scenario = require "scenario"
local input = require "input" 
local scen = scenario.new()

local GUI = require "gui/GUI"
local OneButton = require "scenarios/gamejam/cs_one"
local TwoButton = require "scenarios/gamejam/cs_two"
local ThreeButton = require "scenarios/gamejam/cs_three"
local FourButton = require "scenarios/gamejam/cs_four"
local gui = GUI:new()
local cs_oneButton = OneButton:new({x=0,y=0})

local cs_twoButton = TwoButton:new({x = 0, y = 0})
local cs_threeButton = ThreeButton:new({x = 0, y = 0})
local cs_fourButton = FourButton:new({x = 0, y = 0})


gui:addComponent(cs_oneButton)

scen.gui = gui
scen:registerDestroyCallback( function() scen.gui:destroy() end )

local mouse = core.input.mouse
local keyboard = core.input.keyboard
local key = keyboard.key

local camera = require "rts_camera".new()
camera:lookAt( core.glm.vec3.new( 0, 200, 0 ), core.glm.vec3.new( 0, 0, 0 ) )
core.camera.gameCamera:setView( camera:getView( ) )
core.camera.gameCamera:setProjection( camera:getProjection( ) )

local floorGrid = {}
local wallGrid = {}
local player = {}
local treasure = {}
local enemies = {}
local traps = {}
local goal = {}
local restartMap = false
local treasureMax
local trapMax
local enemyMax
local gSizeX
local gSizeY
local start = true
local dead = false
local gSeed = 0
local shotLastFrame = false
local highScore = 0

local roamingShots = {}

local cutScene = true
local frameOne = true
local frameTwo = false
local frameThree = false
local frameFour = false

function randomFloat(lower, greater)
    return lower + math.random()  * (greater - lower);
end

function CreateEnemy(x, y, z)

	local outVal = {}
	outVal.entity = scen:loadAssembly(
		{
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { x, y, z}}
			},
			{
				type = core.componentType.GraphicsComponent,
				data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
				load = { 
					mesh = { core.loaders.GnomeLoader, "assets/cube.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/gamejam/redpixel.material", false }
					}
			},
			{
				type = core.componentType.ScaleComponent,
				data = { scale = 2.0 }
			},
			{
				type = core.componentType.RotationComponent,
				data = { rotation = { 0, 0, 0, 1 } }
			}
		})

	outVal.spot = CreateSpotLightFOO(x - 9, y, z, 0.5, 0, 0.5, 15, 3.14/6.0, 0.1 , 20)
	outVal.point = CreatePointlightFOO(x, 5, z, 0.5, 0, 0.5, 5, 2)
	outVal.speed = 10
	outVal.dir = {}
	outVal.dir.x = 0
	outVal.dir.y = 0
	outVal.shots = {}
	outVal.shotTimer = 0
	outVal.shot = false
	outVal.shotLimit = randomFloat(1, 3)
	outVal.posY = math.random(0, 100)
	outVal.dead = false
	return outVal

end

function CreateEnemyShot(x, y, z)
	local outVal = {}

	outVal.pointlight = CreatePointlightFOO(x, y, z, 0.5, 0.0, 0.5, 1.0, 20.0)
	outVal.entity = scen:loadAssembly(
		{
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { x, y, z}}
			},
			{
				type = core.componentType.GraphicsComponent,
				data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
				load = { 
					mesh = { core.loaders.GnomeLoader, "assets/sphere.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/gamejam/whitePixel.material", false }
					}
			},
			{
				type = core.componentType.ScaleComponent,
				data = { scale = 1.0 }
			},
			{
				type = core.componentType.RotationComponent,
				data = { rotation = { 0, 0, 0, 1 } }
			}
		})
	outVal.speed = 10
	outVal.duration = 4
	outVal.timer = 0
	outVal.dir = {}
	outVal.dir.x = 0
	outVal.dir.y = 0
	outVal.hit = false
	return outVal

end

function CreateShot(x, y, z)
	local outVal = {}

	outVal.pointlight = CreatePointlightFOO(x, y, z, 0.0, 0.5, 0.0, 1.0, 20.0)
	outVal.entity = scen:loadAssembly(
		{
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { x, y, z}}
			},
			{
				type = core.componentType.GraphicsComponent,
				data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
				load = { 
					mesh = { core.loaders.GnomeLoader, "assets/sphere.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/gamejam/greenpixel.material", false }
					}
			},
			{
				type = core.componentType.ScaleComponent,
				data = { scale = 1.0 }
			},
			{
				type = core.componentType.RotationComponent,
				data = { rotation = { 0, 0, 0, 1 } }
			}
		})
	outVal.speed = 50
	outVal.duration = 1
	outVal.timer = 0
	outVal.hit = false
	return outVal
end

function CreateTreasure(x, y, z)
	local outVal = {}
	outVal.value = math.random(1000, 2000)
	outVal.treasure = scen:loadAssembly(
		{
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { x, y, z}}
			},
			{
				type = core.componentType.GraphicsComponent,
				data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
				load = { 
					mesh = { core.loaders.GnomeLoader, "assets/minecart.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/minecart.material", false }
					}
			},
			{
				type = core.componentType.ScaleComponent,
				data = { scale = 5.0}
			},
			{
				type = core.componentType.RotationComponent,
				data = { rotation = { 0, 0, 0, 1 } }
			}
		})
		
	outVal.pointlight = CreatePointlightFOO(x, y + 5, z, 0.5, 0.5, 0.0, 1.0, 10.0)
	
	return outVal
end

function CreateGoal(sizeX, sizeY)
	
	local found = false
	local posX = 0
	local posY = 0
	while not found do
		posX = math.random(1, sizeX)
		posY = math.random(1, sizeY)

		if not wallGrid[posX][posY] then
			found = true
		end
	end

	local outVal = CreatePointlightFOO(posX * 10, 10, posY * 10, 0, 0, 1, 10, 50)
	return outVal
end

function CreateTrap(x, y, z)
	local outVal = {}
	
	local hurfdurf = math.random(1, 2)
	local intensity = 0
	if hurfdurf < 2 then
		outVal.active = false
	else
		outVal.active = true
		intensity = 1
	end

	outVal.trap = CreatePointlightFOO(x, y, z, 1, 0, 0, intensity, 20)
	outVal.timer = math.random(4, 12) / 4
	outVal.counter = 0
	return outVal
end

function CreateSpotLight(x, y, z)
	local spot = scen:loadAssembly( 
	{
		{
			type = core.componentType.LightComponent,
			data =  { 
						color = { 0.2, 0.7, 0.8 },
						speccolor = { 0.2, 0.7, 0.8 },
						intensity = 15.0,
						spotangle = 3.14/3.0,
						spotpenumbra = 0.5,
						type = core.gfx.objectTypes.Light,
						lighttype = core.gfx.lightTypes.Spot
					}
		},
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = { x, y, z } }
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = 40.0 }
		},
		{
			type = core.componentType.RotationComponent,
			data = { rotation = { 1,0,0,0 } }
		}
	} 
	)

	return spot
end

function CreateSpotLightFOO(x, y, z, cx, cy, cz, intens, spota, spotp, radis)
	
	local spot = scen:loadAssembly( 
	{
		{
			type = core.componentType.LightComponent,
			data =  { 
						color = { cx, cy, cz },
						speccolor = { cx, cy, cz },
						intensity = intens,
						spotangle = spota,
						spotpenumbra = spotp,
						type = core.gfx.objectTypes.Light,
						lighttype = core.gfx.lightTypes.Spot
					}
		},
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = { x, y, z } }
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = radis }
		},
		{
			type = core.componentType.RotationComponent,
			data = { rotation = { 1,0,0,0 } }
		}
	} 
	)

	return spot
end

function CreatePointlight(x, y, z)
	local spot = scen:loadAssembly( 
	{
		{
			type = core.componentType.LightComponent,
			data =  { 
						color = { 0.2, 0.7, 0.8 },
						speccolor = { 0.2, 0.7, 0.8 },
						intensity = 2.0,
						spotangle = 3.14/4.0,
						spotpenumbra = 0.1,
						type = core.gfx.objectTypes.Light,
						lighttype = core.gfx.lightTypes.Point
					}
		},
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = { x, y, z } }
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = 30.0 }
		},
		{
			type = core.componentType.RotationComponent,
			data = { rotation = { 1,0,0,0 } }
		}
	} 
	)

	return spot
end

function CreatePointlightFOO(x, y, z, cx, cy, cz, intasdasd, asdasfasdasd)
	local spot = scen:loadAssembly( 
	{
		{
			type = core.componentType.LightComponent,
			data =  { 
						color = { cx, cy, cz },
						speccolor = { cx, cy, cz },
						intensity = intasdasd,
						spotangle = 3.14/4.0,
						spotpenumbra = 0.1,
						type = core.gfx.objectTypes.Light,
						lighttype = core.gfx.lightTypes.Point
					}
		},
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = { x, y, z } }
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = asdasfasdasd }
		},
		{
			type = core.componentType.RotationComponent,
			data = { rotation = { 1,0,0,0 } }
		}
	} 
	)

	return spot
end

function CreatePlayer(sizeX, sizeY)
	
	local found = false
	local posX = 0
	local posY = 0
	while not found do
		posX = math.random(1, sizeX)
		posY = math.random(1, sizeY)

		if not wallGrid[posX][posY] then
			found = true
		end
	end

	local outVal = scen:loadAssembly(
		{
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { posX * 10, 10, posY * 10}}
			},
			{
				type = core.componentType.GraphicsComponent,
				data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
				load = { 
					mesh = { core.loaders.GnomeLoader, "assets/cube.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/blackPixel.material", false }
					}
			},
			{
				type = core.componentType.ScaleComponent,
				data = { scale = 2.0 }
			},
			{
				type = core.componentType.RotationComponent,
				data = { rotation = { 0, 0, 0, 1 } }
			}
		})
	return outVal
end

function CreateTile(x, y, z, lives)
	local tile = {}

	if lives == 3 then
	tile.entity = scen:loadAssembly(
		{
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { x, y, z}}
			},
			{
				
				type = core.componentType.GraphicsComponent,
				data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
				load = { 
					mesh = { core.loaders.GnomeLoader, "assets/cube.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/gamejam/whitePixel.material", false }
					}
			},
			{
				type = core.componentType.ScaleComponent,
				data = { scale = 10.0 }
			},
			{
				type = core.componentType.RotationComponent,
				data = { rotation = { 0, 0, 0, 1 } }
			}
		})
	end

	if lives == 2 then
	tile.entity = scen:loadAssembly(
		{
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { x, y, z}}
			},
			{
				
				type = core.componentType.GraphicsComponent,
				data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
				load = { 
					mesh = { core.loaders.GnomeLoader, "assets/cube.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/gamejam/greenPixel.material", false }
					}
			},
			{
				type = core.componentType.ScaleComponent,
				data = { scale = 10.0 }
			},
			{
				type = core.componentType.RotationComponent,
				data = { rotation = { 0, 0, 0, 1 } }
			}
		})
	end

	if lives == 1 then
	tile.entity = scen:loadAssembly(
		{
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { x, y, z}}
			},
			{
				
				type = core.componentType.GraphicsComponent,
				data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
				load = { 
					mesh = { core.loaders.GnomeLoader, "assets/cube.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/gamejam/redPixel.material", false }
					}
			},
			{
				type = core.componentType.ScaleComponent,
				data = { scale = 10.0 }
			},
			{
				type = core.componentType.RotationComponent,
				data = { rotation = { 0, 0, 0, 1 } }
			}
		})
	end



	tile.lives = lives
	return tile
end

function SetWall(percent)
	local random = math.random(0, 100)

	if random < percent then
		return 1
	end
	return 0
end

function AdjacentWallCount(x, y, scopeX, scopeY, sizeX, sizeY)
	local wallCount = 0

	local startX = x - scopeX;
	local startY = y - scopeY;
	local endX = x + scopeX;
	local endY = y + scopeY;


	if x == sizeX then
		return 8
	elseif x == 1 then
		return 8
	end

	if y == sizeY then
		return 8
	elseif y == 1 then
		return 8
	end
 
	local iX = startX;
	local iY = startY;

 
	for iY = startY, endY do
		for iX = startX,endX do
			if not (iX == x and iY==y) then
				if wallGrid[iX][iY] then
					wallCount = wallCount + 1;
				end
			end
		end
	end

	return wallCount
end

function RandomFill(sizeX, sizeY, percent)
	for x = 1, sizeX do
			wallGrid[x] = {}
			for y = 1, sizeY do
				wallGrid[x][y] = nil
				if x == 1 then
					wallGrid[x][y] = CreateTile(x * 10, 10, y * 10, 3)
				elseif x == sizeX then
					wallGrid[x][y] = CreateTile(x * 10, 10, y * 10, 3)
				elseif y == 1 then
					wallGrid[x][y] = CreateTile(x * 10, 10, y * 10, 3)
				elseif y == sizeY then
					wallGrid[x][y] = CreateTile(x * 10, 10, y * 10, 3)
				else
					local halfHeight = sizeY / 2
					if y == halfHeight then
						wallGrid[x][y] = nil
					else
						local isWall = SetWall(percent)
						if isWall == 1 then
							wallGrid[x][y] = CreateTile(x * 10, 10, y * 10, 3)
						else
							wallGrid[x][y] = nil
						end
					end
				end
		end
	end
end

function CellularAutomata(sizeX, sizeY)

	for x = 1, sizeX do
		for y = 1, sizeY do
			local wallCount = AdjacentWallCount(x, y, 1, 1, sizeX, sizeY)

			if wallGrid[x][y] then
				if wallCount >= 4 then
					--do nothing
				elseif wallCount < 2 then
					wallGrid[x][y].entity:destroy()
					wallGrid[x][y] = nil
				end
			else
				if wallCount >=5 then
					wallGrid[x][y] = CreateTile(x * 10, 10, y * 10, 3)
				end
			end
		end
	end
	
end

function GenerateWalls(sizeX, sizeY, percent)
	
	RandomFill(sizeX, sizeY, percent)
	CellularAutomata(sizeX, sizeY)
end

function InitMap(sizeX, sizeY, percent)

	--init floor
	for x = 1, sizeX do
		floorGrid[x] = {}
		for y = 1, sizeY do
			floorGrid[x][y] = CreateTile(x * 10, 0, y * 10, 3)
		end
	end

	GenerateWalls(sizeX, sizeY, percent)

end

function PositionToCell(x, y)
	local cellPos = {}

	local valX = x / 10
	local valY = y / 10

	cellPos.x = math.floor(valX)
	cellPos.y = math.floor(valY)

	return cellPos
end

function PositionToCellSHOT(x, y)
	local cellPos = {}

	local valX = x / 10
	local valY = y / 10

	cellPos.x = math.floor(valX)
	cellPos.y = math.floor(valY)

	return cellPos
end

function CheckCollision(x1,y1,w1,h1, x2,y2,w2,h2)
  return x1 < x2+w2 and
         x2 < x1+w1 and
         y1 < y2+h2 and
         y2 < y1+h1
end

function CollidePlayer(cellPos, dir, wpc)

	if dir.x > 0 then
		
		if wallGrid[cellPos.x + 1][cellPos.y] then
			local tempCP = {}
			tempCP = PositionToCell(wpc.position[1] + 5, wpc.position[3])

			if wallGrid[tempCP.x][tempCP.y] then
				dir.x = 0
			end
		end
	elseif dir.x < 0 then
		
		if wallGrid[cellPos.x][cellPos.y] then
			local tempCP = {}
			tempCP = PositionToCell(wpc.position[1] + 5, wpc.position[3])
			if wallGrid[tempCP.x][tempCP.y] then
				dir.x = 0
			end
		end
	end
	if dir.y > 0 then
		local tempCP = {}
		tempCP = PositionToCell(wpc.position[1] + 5, wpc.position[3] + 5)
		
		if wallGrid[tempCP.x][tempCP.y] then
			dir.y = 0
		end
	elseif dir.y < 0 then

		local tempCP = {}
		tempCP = PositionToCell(wpc.position[1] + 5, wpc.position[3] - 5)
		if wallGrid[tempCP.x][tempCP.y] then
			dir.y = 0
		end
	end
end

function CollideShot(cellPos, dir, wpc, shot)

	if dir.x > 0 then
			local tempCP = {}
			tempCP = PositionToCellSHOT(wpc.position[1], wpc.position[3])
			if tempCP.x >= gSizeX then
				shot.hit = true
				return
			end
			if wallGrid[tempCP.x][tempCP.y] then
				if CheckCollision(wpc.position[1], wpc.position[3], 4, 4, tempCP.x * 10, tempCP.y * 10,10,10) then
					wallGrid[tempCP.x][tempCP.y].lives = wallGrid[tempCP.x][tempCP.y].lives - 1

					if wallGrid[tempCP.x][tempCP.y].lives <= 0 then
						wallGrid[tempCP.x][tempCP.y].entity:destroy()
						wallGrid[tempCP.x][tempCP.y] = nil
					else
						local lives = wallGrid[tempCP.x][tempCP.y].lives
						wallGrid[tempCP.x][tempCP.y].entity:destroy()
						wallGrid[tempCP.x][tempCP.y] = nil
						wallGrid[tempCP.x][tempCP.y]  = CreateTile(tempCP.x * 10, 10, tempCP.y * 10, lives)
					end

					shot.hit = true
				end
			end
		--end
	elseif dir.x < 0 then
		local tempCP = {}
		tempCP = PositionToCellSHOT(wpc.position[1], wpc.position[3])
		if tempCP.x <= 1 then
			shot.hit = true
			return
		end

		if wallGrid[tempCP.x][tempCP.y] then
			if CheckCollision(wpc.position[1], wpc.position[3], 4, 4, tempCP.x * 10, tempCP.y * 10,10,10) then
				wallGrid[tempCP.x][tempCP.y].lives = wallGrid[tempCP.x][tempCP.y].lives - 1

					if wallGrid[tempCP.x][tempCP.y].lives <= 0 then
						wallGrid[tempCP.x][tempCP.y].entity:destroy()
						wallGrid[tempCP.x][tempCP.y] = nil
					else
						local lives = wallGrid[tempCP.x][tempCP.y].lives
						wallGrid[tempCP.x][tempCP.y].entity:destroy()
						wallGrid[tempCP.x][tempCP.y] = nil
						wallGrid[tempCP.x][tempCP.y]  = CreateTile(tempCP.x * 10, 10, tempCP.y * 10, lives)
					end

					shot.hit = true
			end
		end
	elseif dir.y > 0 then
		local tempCP = {}
		tempCP = PositionToCellSHOT(wpc.position[1] + 5, wpc.position[3])
		if tempCP.y >= gSizeY then
			shot.hit = true
			return
		end

		if wallGrid[tempCP.x][tempCP.y] then
			if CheckCollision(wpc.position[1], wpc.position[3], 4, 4, tempCP.x * 10, tempCP.y * 10,10,10) then
				
				wallGrid[tempCP.x][tempCP.y].lives = wallGrid[tempCP.x][tempCP.y].lives - 1

					if wallGrid[tempCP.x][tempCP.y].lives <= 0 then
						wallGrid[tempCP.x][tempCP.y].entity:destroy()
						wallGrid[tempCP.x][tempCP.y] = nil
					else
						local lives = wallGrid[tempCP.x][tempCP.y].lives
						wallGrid[tempCP.x][tempCP.y].entity:destroy()
						wallGrid[tempCP.x][tempCP.y] = nil
						wallGrid[tempCP.x][tempCP.y]  = CreateTile(tempCP.x * 10, 10, tempCP.y * 10, lives)
					end

					shot.hit = true
			end
		end
	elseif dir.y < 0 then
		local tempCP = {}
		tempCP = PositionToCellSHOT(wpc.position[1] + 5, wpc.position[3])
		if tempCP.y <= 1 then
			shot.hit = true
			return
		end
		if wallGrid[tempCP.x][tempCP.y] then
			if CheckCollision(wpc.position[1], wpc.position[3], 4, 4, tempCP.x * 10, tempCP.y * 10,10,10) then
				wallGrid[tempCP.x][tempCP.y].lives = wallGrid[tempCP.x][tempCP.y].lives - 1

					if wallGrid[tempCP.x][tempCP.y].lives <= 0 then
						wallGrid[tempCP.x][tempCP.y].entity:destroy()
						wallGrid[tempCP.x][tempCP.y] = nil
					else
						local lives = wallGrid[tempCP.x][tempCP.y].lives
						wallGrid[tempCP.x][tempCP.y].entity:destroy()
						wallGrid[tempCP.x][tempCP.y] = nil
						wallGrid[tempCP.x][tempCP.y]  = CreateTile(tempCP.x * 10, 10, tempCP.y * 10, lives)
					end

					shot.hit = true
			end
		end
	end
end

function Populate(treasureCount, trapCount, enemyCount)

	--Generate some treasure
	while #treasure < treasureCount do
		local foundPos = false
		local x 
		local y

		while not foundPos do
			x = math.random(1, gSizeX)
			y = math.random(1, gSizeY)

			if not wallGrid[x][y] then
				foundPos = true
			end
		end

		table.insert(treasure, CreateTreasure(x * 10, 5, y * 10 + 10))
	end

	--Generate some traps
	while #traps < trapCount do
		local foundPos = false
		local x 
		local y

		while not foundPos do
			x = math.random(1, gSizeX)
			y = math.random(1, gSizeY)

			if not wallGrid[x][y] then
				foundPos = true
			end
		end

		table.insert(traps, CreateTrap(x * 10, 10, y * 10))
	end
	
	--generate some enemies
	while #enemies < enemyCount do
		local foundPos = false
		local x 
		local y

		while not foundPos do
			x = math.random(1, gSizeX)
			y = math.random(1, gSizeY)

			if not wallGrid[x][y] then
				foundPos = true
			end
		end

		table.insert(enemies, CreateEnemy(x * 10, 10, y * 10))
	end
	
	
	
	--win gamejam
end

function ControlPlayer(dt)
	--get player position
	local wpc
	local oldPos
	wpc = player.entity:get(core.componentType.WorldPositionComponent)

	oldPos = wpc

	local dir = {}
	dir.x = 0
	dir.y = 0

	if keyboard.isKeyDown( key.Left ) then
		dir.x = -1
		player.dir = dir
	elseif keyboard.isKeyDown( key.Right ) then
		dir.x = 1
		player.dir = dir
	elseif keyboard.isKeyDown (key.Up) then
		dir.y = -1
		player.dir = dir
	elseif keyboard.isKeyDown (key.Down) then
		dir.y = 1
		player.dir = dir
	end

	local cellPos = PositionToCell(wpc.position[1], wpc.position[3])

	CollidePlayer(cellPos, dir, wpc)
	
	--set the camera
	camera:lookAt( core.glm.vec3.new( wpc.position[1], wpc.position[2] + 100, wpc.position[3] + 40), core.glm.vec3.new( wpc.position[1], wpc.position[2], wpc.position[3] ) )
	player.y = player.y + 5 * dt
	--set  the position
	wpc.position[1] =  wpc.position[1] + dir.x *  player.speed  * dt
	wpc.position[2] = wpc.position[2] + 0.05 * math.sin(player.y)
	wpc.position[3] =  wpc.position[3] + dir.y * player.speed  * dt

	local spotWPC
	spotWPC = wpc

	player.entity:set(core.componentType.WorldPositionComponent, wpc)
	player.point:set(core.componentType.WorldPositionComponent, wpc)

	local spotRot
	spotRot = player.spot:get(core.componentType.RotationComponent, spotRot)
	
	if dir.x > 0 then
		spotRot.rotation[1] = 1
		spotRot.rotation[2] = 0
		spotRot.rotation[3] = 0
		spotRot.rotation[4] = 0
		spotWPC.position[1] = spotWPC.position[1] - 2
		
		player.spot:set(core.componentType.WorldPositionComponent, spotWPC)
	elseif dir.x < 0 then
		spotRot.rotation[1] = math.cos(math.pi)
		spotRot.rotation[2] = 0
		spotRot.rotation[3] = math.sin(math.pi)
		spotRot.rotation[4] = 0
		spotWPC.position[1] = spotWPC.position[1] + 2
		
		player.spot:set(core.componentType.WorldPositionComponent, spotWPC)
		
	elseif dir.y > 0 then
		spotRot.rotation[1] = math.cos(math.pi / 2)
		spotRot.rotation[2] = 0
		spotRot.rotation[3] = math.sin(math.pi / 2)
		spotRot.rotation[4] = 0
		spotWPC.position[3] = spotWPC.position[3] - 2
		
		player.spot:set(core.componentType.WorldPositionComponent, spotWPC)
	elseif dir.y < 0 then
		spotRot.rotation[1] = math.cos(math.pi / -2)
		spotRot.rotation[2] = 0
		spotRot.rotation[3] = math.sin(math.pi / -2)
		spotRot.rotation[4] = 0
		spotWPC.position[3] = spotWPC.position[3] + 2
		
		player.spot:set(core.componentType.WorldPositionComponent, spotWPC)
	end

	player.spot:set(core.componentType.RotationComponent, spotRot)


	Shoot(dt)
end

function TreasureHandling(dt)
	
	local i = 1
	local playerWPC = player.entity:get(core.componentType.WorldPositionComponent)

	while i <= #treasure do
		
		local tWPC = treasure[i].treasure:get(core.componentType.WorldPositionComponent)

		if (CheckCollision(playerWPC.position[1], playerWPC.position[3], 4, 4, tWPC.position[1],tWPC.position[3],10,10)) then
			player.score = player.score + treasure[i].value
			player.treasureCount = player.treasureCount + 1
			treasure[i].treasure:destroy()
			treasure[i].pointlight:destroy()
			table.remove(treasure, i)
		else
			i = i + 1
		end
	end

	core.draw.drawText( 1050, 20, "TREASURE FOUND: " ..  player.treasureCount .. " / " .. treasureMax)
	core.draw.drawText( 1050, 40, "TREASURE VALUE: " .. player.score )
	core.draw.drawText( 1050, 60, "TREASURE DESTROYED: " .. treasureMax -  player.treasureCount  -  #treasure)
	core.draw.drawText( 1050, 80, "TREASURE LEFT: " .. #treasure)
end

function TrapHandler(dt)
	local playerWPC = player.entity:get(core.componentType.WorldPositionComponent)
	for i,v in ipairs(traps) do
		v.counter = v.counter + dt
		
		local trapPos = v.trap:get(core.componentType.WorldPositionComponent)
		if (v.active and CheckCollision(playerWPC.position[1], playerWPC.position[3], 4, 4, trapPos.position[1], trapPos.position[3],5,5)) then
			dead = true
		end

		if v.counter > v.timer then
			v.active = not v.active
			v.counter = 0

			local lc = v.trap:get(core.componentType.LightComponent)

			if v.active then
				lc.intensity = 1
			else
				lc.intensity = 0
			end
			v.trap:set(core.componentType.LightComponent, lc)
		end
	end
end

function Shoot(dt)

	local wpc
	wpc = player.entity:get(core.componentType.WorldPositionComponent)

	if keyboard.isKeyDown( key.Space ) and not shotLastFrame and not (player.dir.x == 0 and player.dir.y == 0) then
		table.insert(player.shots, CreateShot(wpc.position[1], wpc.position[2], wpc.position[3]))
		player.shots[#player.shots].dir = player.dir
		shotLastFrame = true
	elseif not keyboard.isKeyDown (key.Space) then
		shotLastFrame = false
	end
	
end

function distance(x1, y1, x2, y2)
	return math.sqrt(math.pow((x2 - x1), 2) + math.pow((y2 - y1), 2))
end

function UpdateEnemies(dt)

	local playerWPC = player.entity:get(core.componentType.WorldPositionComponent)

	local rmIndex = {}
	local i = 1
	while i <= #enemies do
		
		local eWPC = enemies[i].entity:get(core.componentType.WorldPositionComponent)
		local eROT = enemies[i].entity:get(core.componentType.RotationComponent)

		enemies[i].posY = enemies[i].posY + 5 * dt
		--set  the position
		eWPC.position[2] = eWPC.position[2] + 0.05 * math.sin(enemies[i].posY)
		enemies[i].entity:set(core.componentType.WorldPositionComponent, eWPC)

		local sWPC = enemies[i].spot:get(core.componentType.WorldPositionComponent)
		sWPC.position[2] = eWPC.position[2]

		local pWPC =  enemies[i].point:get(core.componentType.WorldPositionComponent)
		pWPC = eWPC
		pWPC.position[2] = pWPC.position[2] - 3
		
		enemies[i].point:set(core.componentType.WorldPositionComponent, pWPC)

		if enemies[i].shot then
			enemies[i].shotTimer = 	enemies[i].shotTimer + dt

			if enemies[i].shotTimer > enemies[i].shotLimit then
				enemies[i].shot = false
			end

		end

		local length = distance(playerWPC.position[1], playerWPC.position[3], eWPC.position[1], eWPC.position[3])
	
		if length < 35  and length > 1 then
			

			local x = (eWPC.position[1] - playerWPC.position[1] ) / length
			local y = (eWPC.position[3] - playerWPC.position[3]) / length
			local angle = math.atan2(x, y)

			eROT.rotation[1] = math.cos(-angle - 3.141592 * 0.5)
			eROT.rotation[2] = 0
			eROT.rotation[3] = math.sin(-angle - 3.141592 * 0.5)
			eROT.rotation[4] = 0

			enemies[i].entity:set(core.componentType.RotationComponent, eROT)
			enemies[i].spot:set(core.componentType.RotationComponent, eROT)

			sWPC.position[3] = sWPC.position[3]
			sWPC.position[1] = sWPC.position[1]

			enemies[i].spot:set(core.componentType.WorldPositionComponent, eWPC)

			if not enemies[i].shot then 
				enemies[i].shot = true
				enemies[i].shotTimer = 0
				table.insert(enemies[i].shots, CreateEnemyShot(eWPC.position[1], eWPC.position[2], eWPC.position[3]))
				enemies[i].shots[#enemies[i].shots].dir.x = -x
				enemies[i].shots[#enemies[i].shots].dir.y = -y
			end
		end
		
		local j = 1
		while j < #enemies[i].shots do
			local tWPC = enemies[i].shots[j].entity:get(core.componentType.WorldPositionComponent)

			tWPC.position[1] = tWPC.position[1] + enemies[i].shots[j].speed * dt *  enemies[i].shots[j].dir.x
			tWPC.position[3] = tWPC.position[3] + enemies[i].shots[j].speed * dt * enemies[i].shots[j].dir.y
			enemies[i].shots[j].entity:set(core.componentType.WorldPositionComponent, tWPC)
			enemies[i].shots[j].pointlight:set(core.componentType.WorldPositionComponent, tWPC)

			enemies[i].shots[j].timer = enemies[i].shots[j].timer + dt

			if enemies[i].shots[j].timer >  enemies[i].shots[j].duration then
				enemies[i].shots[j].hit = true
			end


			local cellPos = PositionToCell(tWPC.position[1], tWPC.position[3])

			CollideShot(cellPos, enemies[i].shots[j].dir, tWPC, enemies[i].shots[j])

			
			local asdf = 1
			while asdf <= #treasure do
				local woopeecee = treasure[asdf].treasure:get(core.componentType.WorldPositionComponent)
				if CheckCollision(tWPC.position[1], tWPC.position[3], 4, 4, woopeecee.position[1], woopeecee.position[3],10,10) then
					treasure[asdf].treasure:destroy()
					treasure[asdf].pointlight:destroy()
					table.remove(treasure, asdf)
					enemies[i].shots[j].hit = true
				else
					asdf = asdf + 1
				end
			end

			local erferferf = 1
			while erferferf <= #enemies do
				local woopeecee = enemies[erferferf].entity:get(core.componentType.WorldPositionComponent)
			
				if erferferf == i then
					erferferf = erferferf + 1
				elseif CheckCollision(tWPC.position[1], tWPC.position[3], 2, 2, woopeecee.position[1], woopeecee.position[3],2,2) then
					enemies[i].shots[j].hit = true
					enemies[erferferf].dead = true
					erferferf = erferferf + 1
				else
					erferferf = erferferf + 1
				end
				
			end


			if CheckCollision(tWPC.position[1], tWPC.position[3], 2, 2, playerWPC.position[1], playerWPC.position[3],2,2) then
				enemies[i].shots[j].entity:destroy()
				enemies[i].shots[j].pointlight:destroy()
				table.remove(enemies[i].shots, j)
				dead = true
			elseif enemies[i].shots[j].hit then
				enemies[i].shots[j].entity:destroy()
				enemies[i].shots[j].pointlight:destroy()
				table.remove(enemies[i].shots, j)
			else
				j = j + 1
			end
		end
		i = i + 1
	end

	local hurr = 1
	while hurr <= #enemies do

		if enemies[hurr].dead then	
			enemies[hurr].entity:destroy()
			enemies[hurr].point:destroy()
			enemies[hurr].spot:destroy()
			table.insert(roamingShots, enemies[hurr].shots)
			table.remove(enemies, hurr)
		else
			hurr = hurr + 1
		end
		
	end
end

function RoamingShots(dt)
	local playerWPC = player.entity:get(core.componentType.WorldPositionComponent)
	local i = 1
	if roamingShots then
		while i <= #roamingShots do
			local j = 1
			while j <= #roamingShots[i] do
				local tWPC = roamingShots[i][j].entity:get(core.componentType.WorldPositionComponent)
			
				tWPC.position[1] = tWPC.position[1] + roamingShots[i][j].speed * dt *  roamingShots[i][j].dir.x
				tWPC.position[3] = tWPC.position[3] + roamingShots[i][j].speed * dt * roamingShots[i][j].dir.y
				roamingShots[i][j].entity:set(core.componentType.WorldPositionComponent, tWPC)
				roamingShots[i][j].pointlight:set(core.componentType.WorldPositionComponent, tWPC)

				local cellPos = PositionToCell(tWPC.position[1], tWPC.position[3])

				roamingShots[i][j].timer = roamingShots[i][j].timer + dt

				if roamingShots[i][j].timer >  roamingShots[i][j].duration then
					roamingShots[i][j].hit = true
				end

				local asdf = 1
				while asdf <= #treasure do
					local woopeecee = treasure[asdf].treasure:get(core.componentType.WorldPositionComponent)
					if CheckCollision(tWPC.position[1], tWPC.position[3], 4, 4, woopeecee.position[1], woopeecee.position[3],10,10) then
						treasure[asdf].treasure:destroy()
						treasure[asdf].pointlight:destroy()
						table.remove(treasure, asdf)
						roamingShots[i][j].hit = true
					else
						asdf = asdf + 1
					end
				end

				asdf = 1
				while asdf <= #enemies do
				
					local woopeecee = enemies[asdf].entity:get(core.componentType.WorldPositionComponent)
					if CheckCollision(tWPC.position[1], tWPC.position[3], 2, 2, woopeecee.position[1], woopeecee.position[3],2,2) then
						enemies[asdf].entity:destroy()
						enemies[asdf].point:destroy()
						enemies[asdf].spot:destroy()
						roamingShots[i][j].hit = true
						table.insert(roamingShots, enemies[asdf].shots)
						table.remove(enemies, asdf)
					else
						asdf = asdf + 1
					end
				end

				CollideShot(cellPos, roamingShots[i][j].dir, tWPC, roamingShots[i][j])
				
				if CheckCollision(tWPC.position[1], tWPC.position[3], 2, 2, playerWPC.position[1], playerWPC.position[3],2,2) then
					roamingShots[i][j].entity:destroy()
					roamingShots[i][j].pointlight:destroy()
					table.remove(roamingShots[i], j)
					dead = true
				elseif roamingShots[i][j].hit then
					roamingShots[i][j].entity:destroy()
					roamingShots[i][j].pointlight:destroy()
					table.remove(roamingShots[i], j)
				else
					j = j + 1
				end
			end
			i = i +1
		end
	end
end

function UpdateShots(dt)
	
	local i = 1
	while i <= #player.shots do
		local shotWPC
		shotWPC = player.shots[i].pointlight:get(core.componentType.WorldPositionComponent)

		shotWPC.position[1] = shotWPC.position[1] + player.shots[i].speed * player.shots[i].dir.x * dt
		shotWPC.position[3] = shotWPC.position[3] + player.shots[i].speed * player.shots[i].dir.y * dt
		
		player.shots[i].pointlight:set(core.componentType.WorldPositionComponent, shotWPC)
		player.shots[i].entity:set(core.componentType.WorldPositionComponent, shotWPC)
		local cellPos = PositionToCell(shotWPC.position[1], shotWPC.position[3])

		CollideShot(cellPos, player.shots[i].dir, shotWPC, player.shots[i])

		player.shots[i].timer = player.shots[i].timer + dt

		if player.shots[i].timer >  player.shots[i].duration then
			player.shots[i].hit = true
		end

		local j = 1
		while j <= #treasure do

			local tWPC = treasure[j].treasure:get(core.componentType.WorldPositionComponent)
			if CheckCollision(shotWPC.position[1], shotWPC.position[3], 4, 4, tWPC.position[1], tWPC.position[3],10,10) then
				treasure[j].treasure:destroy()
				treasure[j].pointlight:destroy()
				table.remove(treasure, j)
				player.shots[i].hit = true
			else
				j = j + 1
			end
		end

		j = 1
		while j <= #enemies do

			local tWPC = enemies[j].entity:get(core.componentType.WorldPositionComponent)
			if CheckCollision(shotWPC.position[1], shotWPC.position[3], 2, 2, tWPC.position[1], tWPC.position[3],2,2) then
				enemies[j].entity:destroy()
				enemies[j].point:destroy()
				enemies[j].spot:destroy()

				table.insert(roamingShots, enemies[j].shots)
				player.score = player.score + 1000
				table.remove(enemies, j)
				player.shots[i].hit = true
			else
				j = j + 1
			end
		end

		if player.shots[i].hit then
			player.shots[i].pointlight:destroy()
			player.shots[i].entity:destroy()
			table.remove(player.shots, i)
		else
			i = i + 1
		end
	end
end

function ReloadMap()
	for i = 1, gSizeX do
		for j = 1, gSizeY do
			if wallGrid[i][j] then
				wallGrid[i][j].entity:destroy()
				wallGrid[i][j] = nil
			end

			floorGrid[i][j].entity:destroy()
		end
	end

	local i = 1
	while i <= #player.shots do
		player.shots[i].entity:destroy()
		player.shots[i].pointlight:destroy()
		table.remove(player.shots, i)
	end

	player.entity:destroy()
	player.spot:destroy()
	player.point:destroy()

	i = 1
	while i <= #treasure do
		treasure[i].treasure:destroy()
		treasure[i].pointlight:destroy()
		table.remove(treasure, i)
	end
	 
	i = 1
	while i <= #traps do
		traps[i].trap:destroy()
		table.remove(traps, i)
	end

	i = 1
	while i <= #enemies do
		enemies[i].entity:destroy()
		enemies[i].point:destroy()
		enemies[i].spot:destroy()

		local asdf = 1
		while asdf <= #enemies[i].shots do
			enemies[i].shots[asdf].entity:destroy()
			enemies[i].shots[asdf].pointlight:destroy()
			table.remove(enemies[i].shots, asdf)
		end


		table.remove(enemies, i)
	end

	i = 1
	while i <= #roamingShots do
		local j = 1
		while j <= #roamingShots[i] do
			roamingShots[i][j].entity:destroy()
			roamingShots[i][j].pointlight:destroy()
			table.remove(roamingShots[i], j)
		end
		table.remove(roamingShots, i)
	end



	start = true

	if dead then
		restartMap = true
	else
		restartMap = false
	end

	dead = false
end

function GoalHandler(dt)
	local pWPC = player.entity:get(core.componentType.WorldPositionComponent)
	local gWPC = goal.entity:get(core.componentType.WorldPositionComponent)
	
	if #treasure <= 0 then 
		if CheckCollision(pWPC.position[1], pWPC.position[3], 4, 4, gWPC.position[1], gWPC.position[3],	20,20) then
			goal.found = true
		end
	end
end

function LoadLevel(seed)
	gSeed = gSeed + seed
	math.randomseed(gSeed)

	gSizeX = math.random(20, 100)
	gSizeY = math.random(20, 100)

	InitMap(gSizeX, gSizeY, math.random(20, 45))
	player.entity = CreatePlayer(gSizeX, gSizeY)

	local wpc
	wpc = player.entity:get(core.componentType.WorldPositionComponent)

	player.spot = CreateSpotLight(wpc.position[1], wpc.position[2], wpc.position[3])
	player.point = CreatePointlight(wpc.position[1], wpc.position[2], wpc.position[3])
	player.speed = 35

	if goal.found then
		highScore = highScore + player.score
	end

	player.score = 0
	player.treasureCount = 0
	player.y = 0
	local dir = {}
	dir.x = 0
	dir.y = 0
	player.dir = dir
	player.shots = {}
	start = false

	goal.entity = CreateGoal(gSizeX, gSizeY)
	goal.found = false
	enemyMax = math.random(10, gSizeX)
	treasureMax = math.random(10, gSizeX)
	trapMax = math.random(10, gSizeY)
	Populate(treasureMax, trapMax, enemyMax)
end

local prevFrame = false
local timer = 0
local tOne = 5
local tTwo = 5
local tThree = 5
local tFour = 5


function Update(dt)
	
	if cutScene then

		if frameOne then
			timer = timer + dt

			if timer > tOne then
				cs_oneButton:destroy()
				gui:addComponent(cs_twoButton)
				frameTwo = true
				frameOne = false
				timer = 0
			end

		elseif frameTwo then

			timer = timer + dt

			if timer > tTwo then
				cs_twoButton:destroy()
				gui:addComponent(cs_threeButton)
				frameTwo = false
				frameThree = true
				timer = 0
			end

		elseif frameThree then

			timer = timer + dt

			if timer > tThree then
				cs_threeButton:destroy()
				gui:addComponent(cs_fourButton)
				frameThree = false
				frameFour = true
				timer = 0
			end
		elseif frameFour then
			timer = timer + dt

			if timer > tFour then
				cs_fourButton:destroy()
				frameFour = false
				cutScene = false
			end

		end

		if core.input.mouse.isButtonUp(core.input.mouse.button.Left) then
			prevFrame = false
		end
	else
		if start then
			if keyboard.isKeyDown( key.Enter ) and not restartMap then
				LoadLevel(1)
			elseif keyboard.isKeyDown( key.Enter ) and restartMap then
				LoadLevel(0)
			end

			if not goal.found then
				core.draw.drawText( 520, 260, "QUANTITY BEFORE QUALITY")
				core.draw.drawText( 540, 280, "PRES BUTAN TO ENTER")
			end
		end

		if dead then
			core.draw.drawText( 580, 360, "U DED NIGGA")
			core.draw.drawText(540, 380, "PRES BUTAN TO RETRY")
			if keyboard.isKeyDown( key.Enter ) then
				ReloadMap()
			end
		elseif goal.found then
			core.draw.drawText( 580, 360, "U WIN NIGGA")
			core.draw.drawText(540, 380, "PRES BUTAN TO CONTINUE")
			if keyboard.isKeyDown( key.Enter ) then
				ReloadMap()
			end
		elseif not dead  and not start then
			ControlPlayer(dt)
			TreasureHandling(dt)
			TrapHandler(dt)
			UpdateShots(dt)
			UpdateEnemies(dt)
			GoalHandler(dt)
			RoamingShots(dt)
			core.draw.drawText( 540, 20, "HIGHSCORE: " .. highScore)
		end
	--camera:update(dt)
	end


end

scen:registerUpdateCallback( Update )

return scen;