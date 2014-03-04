local entities = require "entities"
local scenario = require "scenario"
local input = require "input" 
local decal = entities.get "decal"
local scen = scenario.new()

local mouse = core.input.mouse
local keyboard = core.input.keyboard
local key = keyboard.key


local camera = require "rts_camera".new()
camera:lookAt( core.glm.vec3.new( 0, 10, 0 ), core.glm.vec3.new( 0, 0, 0 ) )

local function Init()
	core.camera.gameCamera:setView( camera:getView( ) )
	core.camera.gameCamera:setProjection( camera:getProjection( ) )
end

scen:registerInitCallback( Init )

scen.asm:specific_content( core.contentmanager.load(
		core.loaders.NavigationMeshLoader, "extremeScenario.txt", function( value ) end, false ) )

	



local directional = entities.get "directionalLight"
directional( scen, -0.5950919985771179, -0.7691010236740112, 0.23312883079051971, 0.8, 1.0, 1.0, 0.1 )

local ambient = entities.get "ambientLight"
ambient( scen, 0.7, 1.0, 1.0, 0.001 )


local aim = decal( scen, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 2.5, 0.1, 2.5, "assets/texture/tankwars/aim-decal.material", "" )

--local ground = 
local n = 30
for i=0,n do
	for j=0,n do
		
	
		scen:loadAssembly( 
		{
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { (i - n / 2) * 15, 0, (j - n / 2) * 15 } }
			},
			{
				type = core.componentType.GraphicsComponent,
				data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
				load = { 
							mesh = { core.loaders.GnomeLoader, "assets/plane.bgnome", false },
							material = { core.loaders.MaterialLoader, "assets/texture/tankwars/snow/snow.material", false }
					   }
			},
			{
				type = core.componentType.ScaleComponent,
				data = { scale = 15.0 }
			},
			{
				type = core.componentType.RotationComponent,
				--data = { rotation = { 1*math.sin( 0 ), 1*math.sin( 0 ), 0, 1*math.cos( 0 ) } }
				data = { rotation = { 1*math.sin( -3.14/4 ), 0, 0, 1*math.cos( -3.14/4 ) } }
			}
		}
		)
	end
end
for kk=0,10 do
scen:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { kk * 5, 0, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/model/tankwars/m270-body.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/texture/tankwars/m270-body.material", false }
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
}
)

scen:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { kk * 5, 0, -1.2 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/model/tankwars/m270-turret.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/texture/tankwars/m270-turret.material", false }
			   }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 1.0 }
	},
	{
		type = core.componentType.RotationComponent,
		--data = { rotation = { 1*math.sin( 0 ), 1*math.sin( 0 ), 0, 1*math.cos( 0 ) } }
		data = { rotation = { 0, 1*math.sin( math.pi/4/2 ), 0, 1*math.cos( math.pi/4/2 ) } }
		
	}
}
)
end


local playerGroup = core.system.groups.createGroup();
local playerTank = scen:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, 0, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/model/tankwars/Leopard_Body.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/texture/tankwars/leopard-body.material", false }
			   }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 1.0 }
	},
	{
		type = core.componentType.UnitTypeComponent,
		data = { unitType = core.UnitType.Rioter }
	},
	{
		type = core.componentType.MovementComponent,
		data = { direction = { 0, 0, 0 }, newDirection = { 0, 0, 0 }, speed = 1.5, 
		desiredSpeed = 1.5, goal = false }
		,ignoreHard = true
	},
	{
		type = core.componentType.BoundingVolumeComponent,
		data = { sphereOffset = { 0, 0, 0 }, sphereRadius = 5.0, pickingRadius = 0.0, 
				collisionModel = core.BoundingVolumeCollisionModel.DynamicResolution, 
				type = core.BoundingVolumeType.SphereBoundingType }
	},
	{
		type = core.componentType.AttributeComponent,
		data = { health = 100, stamina = 100, morale = 2.0, 
			   alignment = core.RioterAlignment.Anarchist, rage = 0, pressure = 0, groupID = playerGroup, stanceRioter = core.RioterStance.Normal}
		,
		ignoreHard = true
	},
	{
		type = core.componentType.RotationComponent,
		--data = { rotation = { 0, 1*math.sin( -3.14/4 ), 0, math.cos( -3.14/4 ) } }
		data = { rotation = { 0, 0, 0, 1 } }
	},
	{
		type = core.componentType.FlowfieldComponent,
		data = { node = -1 }
	},
	{
		type = core.componentType.TargetingComponent,
		data = { weapon = -1 },
		ignoreHard = true
	}
}
)

local playerTurret = scen:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, 0, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/model/tankwars/leopard-turret.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/texture/tankwars/leopard-turret.material", false }
					--material = { core.loaders.MaterialLoader, "assets/texture/tankwars/snow/snow.material", false }
			   }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 1.0 }
	},
	{
		type = core.componentType.RotationComponent,
		--data = { rotation = { 0, 1*math.sin( -3.14/4 ), 0, math.cos( -3.14/4 ) } }
		data = { rotation = { 0, 0, 0, 1 } }
	}
}
)



local function CreateBullet(pos, direction)
	return scen:loadAssembly( 
	{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = pos }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/model/tankwars/bullet.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/texture/tankwars/bullet.material", false }
			   }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 2.0 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { 0, math.sin( (math.pi/2 - direction)/2 ), 0, math.cos( (math.pi/2 - direction)/2 ) } }
	}
}
	)
end

local function CreateBulletLight(pos)
	return scen:loadAssembly( 
	{
		{
			type = core.componentType.LightComponent,
			data =  { 
						color = {1.0, 0.7, 0.1},
						speccolor = {1.0, 0.7, 0.1},
						intensity = 2.0,
						spotangle = 0.0,
						spotpenumbra = 0.0,
						type = core.gfx.objectTypes.Light,
						lighttype = core.gfx.lightTypes.Point
					}
		},
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = pos }
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

local function CreateBulletFireLight(position, scale, direction)
	return scen:loadAssembly( 
	{
		{
			type = core.componentType.LightComponent,
			data =  { 
						color = {1.0, 0.7, 0.1},
						speccolor = {1.0, 0.7, 0.1},
						intensity = 5.0,
						spotangle = 1.0,
						spotpenumbra = 0.01,
						type = core.gfx.objectTypes.Light,
						lighttype = core.gfx.lightTypes.Spot
					}
		},
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = position }
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = scale } 
		},
		{
			type = core.componentType.RotationComponent,
			data = { rotation = { math.cos(direction), 0, math.sin(direction), 0 } } -- Lights uses rotation component as a direction vector, not a quaternion
		}
	} 
	)
end



local function CreateMissile(pos, direction, directionUp)


	local rotQuattt = { math.sin(directionUp * 0.5) , 0, 0, math.cos(directionUp * 0.5 )}

	local rotQuat = { 0, math.sin(direction * 0.5), 0, math.cos(direction * 0.5 )}


	local rotttttt = {	(rotQuat[2]*rotQuattt[3] - rotQuat[3]*rotQuattt[2] + rotQuat[1]*rotQuattt[4] + rotQuat[4]*rotQuattt[1]),
						(rotQuat[3]*rotQuattt[1] - rotQuat[1]*rotQuattt[3] + rotQuat[2]*rotQuattt[4] + rotQuat[4]*rotQuattt[2]),
						(rotQuat[1]*rotQuattt[2] - rotQuat[2]*rotQuattt[1] + rotQuat[3]*rotQuattt[4] + rotQuat[4]*rotQuattt[3]),
						(rotQuat[4]*rotQuattt[4] - rotQuat[1]*rotQuattt[1] - rotQuat[2]*rotQuattt[2] - rotQuat[3]*rotQuattt[3])}


	return scen:loadAssembly( 
	{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = pos }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/model/tankwars/missile.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/texture/tankwars/missile.material", false }
			   }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 1.0 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { rotttttt[1], rotttttt[2], rotttttt[3], rotttttt[4], } }
	}
}
	)
end

local function CreateMissileLight(pos)
	return scen:loadAssembly( 
	{
		{
			type = core.componentType.LightComponent,
			data =  { 
						color = {1.0, 0.7, 0.1},
						speccolor = {1.0, 0.7, 0.1},
						intensity = 5.0,
						spotangle = 0.0,
						spotpenumbra = 0.0,
						type = core.gfx.objectTypes.Light,
						lighttype = core.gfx.lightTypes.Point
					}
		},
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = pos }
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = 50.0 } 
		},
		{
			type = core.componentType.RotationComponent,
			data = { rotation = { 1, 0, 0, 0 } } -- Lights uses rotation component as a direction vector, not a quaternion
		}
	} 
	)
end



local function CreateRailGunBullet(pos, direction)
	return scen:loadAssembly( 
	{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = pos }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/model/tankwars/bullet.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/texture/tankwars/bullet.material", false }
			   }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 2.0 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { 0, math.sin( (math.pi/2 - direction)/2 ), 0, math.cos( (math.pi/2 - direction)/2 ) } }
	}
}
	)
end

local function CreateRailGunLight(pos, scale)
	return scen:loadAssembly( 
	{
		{
			type = core.componentType.LightComponent,
			data =  { 
						color = {0.5, 0.0, 0.5},
						speccolor = {0.5, 0.0, 0.5},
						intensity = 0.3,
						spotangle = 0.0,
						spotpenumbra = 0.0,
						type = core.gfx.objectTypes.Light,
						lighttype = core.gfx.lightTypes.Point
					}
		},
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = pos }
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = scale } 
		},
		{
			type = core.componentType.RotationComponent,
			data = { rotation = { 1, 0, 0, 0 } } -- Lights uses rotation component as a direction vector, not a quaternion
		}
	} 
	)
end


local function CreateExplosionLight(position, scale)
	return scen:loadAssembly( 
	{
		{
			type = core.componentType.LightComponent,
			data =  { 
						color = {1.0, 0.7, 0.1},
						speccolor = {1.0, 0.7, 0.1},
						intensity = 20.0,
						spotangle = 0.0,
						spotpenumbra = 0.0,
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
			data = { scale = scale } 
		},
		{
			type = core.componentType.RotationComponent,
			data = { rotation = { 1, 0, 0, 0 } } -- Lights uses rotation component as a direction vector, not a quaternion
		}
	} 
	)
end


local function CreateEnemyTankBody(position, group)
	return scen:loadAssembly( 
	{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { position[1], 0, position[2] } }
	},
	{
		type = core.componentType.UnitTypeComponent,
		data = { unitType = core.UnitType.Rioter }
	},
	{
		type = core.componentType.MovementComponent,
		data = { direction = { 0, 0, 0 }, newDirection = { 0, 0, 0 }, speed = 5000.0, 
		desiredSpeed = 5000.0, goal = false }
		,ignoreHard = true
	},
	{
		type = core.componentType.BoundingVolumeComponent,
		data = { sphereOffset = { 0, 0, 0 }, sphereRadius = 10.0, pickingRadius = 0.0,
				collisionModel = core.BoundingVolumeCollisionModel.DynamicResolution, 
				type = core.BoundingVolumeType.SphereBoundingType }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/model/tankwars/leopard-body.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/texture/tankwars/leopard-body.material", false }
			   }
	},
	{
		type = core.componentType.AttributeComponent,
		data = { health = 100, stamina = 100, morale = 2.0, 
			   alignment = core.RioterAlignment.Anarchist, rage = 0, pressure = 0, groupID = group, stanceRioter = core.RioterStance.Normal}
		,
		ignoreHard = true
	},
	{
		type = core.componentType.TargetingComponent,
		data = { weapon = -1 },
		ignoreHard = true
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 1.0 }
	},
	{
		type = core.componentType.FlowfieldComponent,
		data = { node = -1 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { 0, 0, 0, 1 } }
	}
})
end

local function CreateEnemyTankTurret(position)
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
					mesh = { core.loaders.GnomeLoader, "assets/model/tankwars/leopard-turret.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/texture/tankwars/leopard-turret.material", false }
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
}
	)
end



function CheckCollision(x1,y1,w1,h1, x2,y2,w2,h2)
  return x1 < x2 + w2 and
         x2 < x1 + w1 and
         y1 < y2 + h2 and
         y2 < y1 + h1
end




local enterWasDown = true
local started = false
local level = 1
local difficulty = 3
local score = 0
local maxHealth = 10000
local health = maxHealth
local alive = true



--local playerPosition = {0,0}
local velocity = {0,0}
local playerDirection = 0
local playerTurretDirection = 0



local acc = 2
local decc = 0.95
local turnSpeed = 2
local turnSpeedTurret = 2.5
local topSpeed = 10



local bulletBoundX = 100
local bulletBoundY = 100

local boundX = 10
local boundY = 10

local shootTimer = 0
local shootInterval = 1.0
local bulletSpeed = 150

local bullets = {}
local bulletLights = {}
local bulletFireLights = {}

local explosionLights = {}

local shootMissileTimer = 0
local shootMissileTimerInterval = 3
local missileSpeed = 65
local missileBarrage = 10
local currentMissileBarrage = missileBarrage
local missileLuanchInterval = 0.1
local missileLuanchIntervalTimer = 0.0
local isShootingMissiles = false

local missiles = {}
local missileLights = {}
local missileExplosions = {}




local shootRailgunTimer = 0
local shootRailgunTimerInterval = 10
local shootRailgunBuildUpTimer = 0
local shootRailgunBuildUpTimerInterval = 3


local railgunParticles = {}
local railgunLights = {}



local enemyTurnSpeed = 0.3
local enemyTurnSpeedTurret = 0.5
local enemyShootInterval = 5.0
local enemyTopSpeed = 5

local enemyTanks = {}
local enemyTankTurrets = {}


local tankGroups

local function InitEnemyTanks(n)
	tankGroups = tankGroups or core.system.groups.createGroup();
	local minSpawn = 20
	local maxSpawn = (boundX + boundY) / 2 - minSpawn
	for i=1,n do
		local randVec = {1 - 2 * math.random(), 1 - 2 * math.random()}
		local randDist = minSpawn + math.random() * maxSpawn
		
		
		if randVec[1] == 0 and randVec[2] == 0 then
			randVec[1] = 1
		end
		
		-- normalize
		local length = math.sqrt(randVec[1] * randVec[1] + randVec[2] * randVec[2])
		randVec[1] = randVec[1] / length
		randVec[2] = randVec[2] / length
		
		randVec[1] = randVec[1] * randDist
		randVec[2] = randVec[2] * randDist
		

		table.insert( enemyTanks, {entity = CreateEnemyTankBody(randVec, tankGroups), direction = math.pi * 2 * math.random(), velocity = {0,0}, shootTimer = 10.0 * math.random()})
		table.insert( enemyTankTurrets, {entity = CreateEnemyTankTurret(randVec), direction = math.pi * 2 * math.random()})
	end
end

InitEnemyTanks(1)

local function ShootBullets(direction, position, playerMade)
	local tankPos = playerTank:get(core.componentType.WorldPositionComponent)
	if position[1] > tankPos.position[1] - bulletBoundX
	or position[1] < tankPos.position[1] + bulletBoundX
	or position[3] > tankPos.position[3] - bulletBoundY
	or position[3] < tankPos.position[3] + bulletBoundY
	then
		table.insert(bullets, { entity = CreateBullet(position, direction), direction = direction, playerMade = playerMade })
		
		table.insert(bulletLights, { entity = CreateBulletLight(position)})
		
		table.insert(bulletFireLights, { entity = CreateBulletFireLight(position, 10, direction)})
		
	end
end


local function ShootMissiles(direction, directionUp, position, playerMade)
	local tankPos = playerTank:get(core.componentType.WorldPositionComponent)
	if position[1] > tankPos.position[1] - bulletBoundX
	or position[1] < tankPos.position[1] + bulletBoundX
	or position[3] > tankPos.position[3] - bulletBoundY
	or position[3] < tankPos.position[3] + bulletBoundY
	then
		table.insert(missiles, { entity = CreateMissile(position, direction, directionUp), direction = direction, directionUp = directionUp, playerMade = playerMade })
		
		table.insert(missileLights, { entity = CreateMissileLight(position)})
		
		--table.insert(bulletFireLights, { entity = CreateBulletFireLight(position, 200, direction)})
		
	end
end


local function ShootRailgun(direction, position, playerMade)

	local tmpPos = position

	local i = 1
	while i <= 101 do
		table.insert(railgunParticles, { entity = CreateBullet(tmpPos, direction), direction = direction, playerMade = playerMade })
		
		table.insert(railgunLights, { entity = CreateRailGunLight(tmpPos, 10)})
		
		tmpPos[1] = tmpPos[1] + math.cos(direction) * 0.5
		tmpPos[2] = position[2]
		tmpPos[3] = tmpPos[3] + math.sin(direction) * 0.5
		i = i + 1
	end
end



local function BulletHit(bulletID)
	tankSize = 3
	bulletSize = 1
	wasHit = false
	bulletPos = bullets[bulletID].entity:get(core.componentType.WorldPositionComponent).position
	
	if not bullets[bulletID].playerMade then
		playerPos = playerTank:get(core.componentType.WorldPositionComponent).position
		
		if CheckCollision(playerPos[1] - tankSize * 0.5, playerPos[3] - tankSize * 0.5, tankSize, tankSize, bulletPos[1] - bulletSize * 0.5, bulletPos[3] - bulletSize * 0.5, bulletSize, bulletSize) then
			health = health - 5
			table.insert(explosionLights, { entity = CreateExplosionLight(bulletPos, 20)})
			--explosionLights
			return true
		end

		return false
	end
	
	
	local i = 1
	while i <= #enemyTanks do
		local tankPos = enemyTanks[i].entity:get(core.componentType.WorldPositionComponent).position
		wasHit = CheckCollision(tankPos[1] - tankSize * 0.5, tankPos[3] - tankSize * 0.5, tankSize, tankSize, bulletPos[1] - bulletSize * 0.5, bulletPos[3] - bulletSize * 0.5, bulletSize, bulletSize)
		if wasHit == true then
			
			score = score + 10
			
			enemyTanks[i].entity:destroy()
			enemyTanks[i] = nil
			table.remove(enemyTanks, i)
			
			enemyTankTurrets[i].entity:destroy()
			enemyTankTurrets[i] = nil
			table.remove(enemyTankTurrets, i)
			
			
			table.insert(explosionLights, { entity = CreateExplosionLight(bulletPos, 20)})
			return true
		else
			i = i + 1
		end
	end
	
	return false
end



local function MissileHit(missileID)
	tankSize = 3
	missileSize = 10
	wasHit = false
	missilePos = missiles[missileID].entity:get(core.componentType.WorldPositionComponent).position
	if missilePos[2] <= 0.0 then
		if not missiles[missileID].playerMade then
			playerPos = playerTank:get(core.componentType.WorldPositionComponent).position
			
			if CheckCollision(playerPos[1] - tankSize * 0.5, playerPos[3] - tankSize * 0.5, tankSize, tankSize, missilePos[1] - missileSize * 0.5, missilePos[3] - missileSize * 0.5, missileSize, missileSize) then
				health = health - 5
				--table.insert(explosionLights, { entity = CreateBulletExplosionLight(missilePos, 20)})
				--explosionLights
				return true
			end

			return false
		end
		
		
		local i = 1
		while i <= #enemyTanks do
			local tankPos = enemyTanks[i].entity:get(core.componentType.WorldPositionComponent).position
			wasHit = CheckCollision(tankPos[1] - tankSize * 0.5, tankPos[3] - tankSize * 0.5, tankSize, tankSize, missilePos[1] - missileSize * 0.5, missilePos[3] - missileSize * 0.5, missileSize, missileSize)
			if wasHit == true then
				
				score = score + 10
				
				enemyTanks[i].entity:destroy()
				enemyTanks[i] = nil
				table.remove(enemyTanks, i)
				
				enemyTankTurrets[i].entity:destroy()
				enemyTankTurrets[i] = nil
				table.remove(enemyTankTurrets, i)
				
				
				--table.insert(explosionLights, { entity = CreateExplosionLight({missilePos[1], 1.5, missilePos[2]}, 20)})
				--return true
			
			else
				i = i + 1
			end
		end
		table.insert(explosionLights, { entity = CreateExplosionLight({missilePos[1], 1.5, missilePos[3]}, 40)})
		return true
	end
	return false
end


local function RailgunHit()
	tankSize = 3
	missileSize = 10
	wasHit = false

	--if not missiles[missileID].playerMade then
	--	playerPos = playerTank:get(core.componentType.WorldPositionComponent).position
	--	
	--	if CheckCollision(playerPos[1] - tankSize * 0.5, playerPos[3] - tankSize * 0.5, tankSize, tankSize, missilePos[1] - missileSize * 0.5, missilePos[3] - missileSize * 0.5, missileSize, missileSize) then
	--		health = health - 5
	--		--table.insert(explosionLights, { entity = CreateBulletExplosionLight(missilePos, 20)})
	--		--explosionLights
	--		return true
	--	end
    --
	--	return false
	--end
	
	
	local i = 1
	while i <= #enemyTanks do
		local tankPos = enemyTanks[i].entity:get(core.componentType.WorldPositionComponent).position
		wasHit = CheckCollision(tankPos[1] - tankSize * 0.5, tankPos[3] - tankSize * 0.5, tankSize, tankSize, missilePos[1] - missileSize * 0.5, missilePos[3] - missileSize * 0.5, missileSize, missileSize)
		if wasHit == true then
			
			score = score + 10
			table.insert(explosionLights, { entity = CreateExplosionLight({tankPos[1], 1.5, tankPos[3]}, 20)})
			
			enemyTanks[i].entity:destroy()
			enemyTanks[i] = nil
			table.remove(enemyTanks, i)
			
			enemyTankTurrets[i].entity:destroy()
			enemyTankTurrets[i] = nil
			table.remove(enemyTankTurrets, i)
			
			
			--table.insert(explosionLights, { entity = CreateExplosionLight({missilePos[1], 1.5, missilePos[2]}, 20)})
			--return true
		
		else
			i = i + 1
		end
	end
	
	--return true
	--end
	return false
end



local function UpdateBullets(delta)
	local tankPos = playerTank:get(core.componentType.WorldPositionComponent)
	local i = 1
	while i <= #bullets do
	
		local wpc
		wpc = bullets[i].entity:get(core.componentType.WorldPositionComponent)
		position = wpc.position
		
		if position[1] < tankPos.position[1] - bulletBoundX
		or position[1] > tankPos.position[1] + bulletBoundX
		or position[3] < tankPos.position[3] - bulletBoundY
		or position[3] > tankPos.position[3] + bulletBoundY
		or BulletHit(i)
		then            
			-- Remove bullet outside of bounds
			bullets[i].entity:destroy()
			table.remove(bullets, i)
			
			bulletLights[i].entity:destroy()
			table.remove(bulletLights, i)
		else
			wpc.position[1] = wpc.position[1] + bulletSpeed * math.cos(bullets[i].direction) * delta
			wpc.position[2] = 1.5
			wpc.position[3] = wpc.position[3] + bulletSpeed * math.sin(bullets[i].direction) * delta
			
			bullets[i].entity:set(core.componentType.WorldPositionComponent, wpc)
			bulletLights[i].entity:set(core.componentType.WorldPositionComponent, wpc)
			i = i + 1
		end
	end
end


local function UpdateMissiles(delta)
	local tankPos = playerTank:get(core.componentType.WorldPositionComponent)
	local i = 1
	while i <= #missiles do
	
		local wpc
		wpc = missiles[i].entity:get(core.componentType.WorldPositionComponent)
		position = wpc.position
		
		if position[1] < tankPos.position[1] - bulletBoundX
		or position[1] > tankPos.position[1] + bulletBoundX
		or position[3] < tankPos.position[3] - bulletBoundY
		or position[3] > tankPos.position[3] + bulletBoundY
		or MissileHit(i)
		then            
			-- Remove bullet outside of bounds
			missiles[i].entity:destroy()
			table.remove(missiles, i)
			
			missileLights[i].entity:destroy()
			table.remove(missileLights, i)
		else
			missiles[i].directionUp = missiles[i].directionUp - math.pi * 2 * delta
			wpc.position[1] = wpc.position[1] + missileSpeed * math.cos(missiles[i].direction) * delta
			wpc.position[2] = wpc.position[2] + missileSpeed * math.sin(missiles[i].directionUp) * delta
			wpc.position[3] = wpc.position[3] + missileSpeed * math.sin(missiles[i].direction) * delta
			
			missiles[i].entity:set(core.componentType.WorldPositionComponent, wpc)
			missileLights[i].entity:set(core.componentType.WorldPositionComponent, wpc)
			
			
			missiles[i].entity:get(core.componentType.WorldPositionComponent)
			
			
			--rotDir = {math.cos(missiles[i].direction - math.pi / 2),0, math.sin(missiles[i].direction - math.pi / 2) }
			--upDir ={0,1,0}
			
			--rotDir = core.glm.vec3.new( math.cos(missiles[i].direction - math.pi / 2),0, math.sin(missiles[i].direction - math.pi / 2) )
			--upDir = core.glm.vec3.new( 0,1,0 )
			
			--rotDir = tmpDir
			
			
			--rotQuat = core.glm.quat.new( 0,0,0,1 )
			
			--local missileRot = missiles[i].entity:get(core.componentType.RotationComponent)
			local rotQuat = { math.sin(missiles[i].directionUp * 0.5) , 0, 0, math.cos(missiles[i].directionUp * 0.5 )}
--local rotQuat = { 0, math.sin(missiles[i].direction * 0.5), 0, math.cos(missiles[i].direction * 0.5 )}
			local rotQuattt = { 0, 0, 0, 1}


			local rotttttt = {	(rotQuat[2]*rotQuattt[3] - rotQuat[3]*rotQuattt[2] + rotQuat[1]*rotQuattt[4] + rotQuat[4]*rotQuattt[1]),
								(rotQuat[3]*rotQuattt[1] - rotQuat[1]*rotQuattt[3] + rotQuat[2]*rotQuattt[4] + rotQuat[4]*rotQuattt[2]),
								(rotQuat[1]*rotQuattt[2] - rotQuat[2]*rotQuattt[1] + rotQuat[3]*rotQuattt[4] + rotQuat[4]*rotQuattt[3]),
								(rotQuat[4]*rotQuattt[4] - rotQuat[1]*rotQuattt[1] - rotQuat[2]*rotQuattt[2] - rotQuat[3]*rotQuattt[3]) }

			--rotQuat = rotQuat * rotQuattt
			--core.glm.quat.rotate(rotQuat, missiles[i].direction, core.glm.vec3.new(0,1,0))
			--rotDir.rotate()
			
			
			
			
			--return Quaternion(	y()*rhs.z() - z()*rhs.y() + x()*rhs.w() + w()*rhs.x(),
			--					z()*rhs.x() - x()*rhs.z() + y()*rhs.w() + w()*rhs.y(),
			--					x()*rhs.y() - y()*rhs.x() + z()*rhs.w() + w()*rhs.z(),
			--					w()*rhs.w() - x()*rhs.x() - y()*rhs.y() - z()*rhs.z());
				
			local missileRot = missiles[i].entity:get(core.componentType.RotationComponent)
			missileRot.rotation[1] = rotttttt[1]
			missileRot.rotation[2] = rotttttt[2]
			missileRot.rotation[3] = rotttttt[3]
			missileRot.rotation[4] = rotttttt[4]
			missiles[i].entity:set(core.componentType.RotationComponent, missileRot)
					
			i = i + 1
		end
	end
end


local function UpdateRailgun(delta)
	
	local i = 1
	while i <= #railgunParticles do
	
		local scale = railgunParticles[i].entity:get(core.componentType.ScaleComponent)
		
		scale.scale[1] = scale.scale[1] * 0.985
		scale.scale[2] = scale.scale[2] * 0.985
		scale.scale[3] = scale.scale[3] * 0.985
		railgunParticles[i].entity:set(core.componentType.ScaleComponent, scale)
		
		
		scale = railgunLights[i].entity:get(core.componentType.ScaleComponent)
		
		scale.scale[1] = scale.scale[1] * 0.985
		scale.scale[2] = scale.scale[2] * 0.985
		scale.scale[3] = scale.scale[3] * 0.985
		railgunLights[i].entity:set(core.componentType.ScaleComponent, scale)
		
		if scale.scale[1] < 0.1 then
			railgunParticles[i].entity:destroy()
			table.remove(railgunParticles, i)
			
			railgunLights[i].entity:destroy()
			table.remove(railgunLights, i)
			
		else
			i = i + 1
		end
		
		
	end
end




local function UpdateExplosions(delta)
	local i = 1
	while i <= #explosionLights do
		scale = explosionLights[i].entity:get(core.componentType.ScaleComponent)
		if scale.scale[1] < 0.001 then
			explosionLights[i].entity:destroy()
			table.remove(explosionLights, i)
		else
			local rnd = (0.95 * (math.random() + 0.5))
			scale.scale[1] = scale.scale[1] * rnd
			scale.scale[2] = scale.scale[2] * rnd
			scale.scale[3] = scale.scale[3] * rnd
			explosionLights[i].entity:set(core.componentType.ScaleComponent, scale)
		end
		i = i + 1
	end
end


local function UpdateBulletFireLights(delta)
	local i = 1
	while i <= #bulletFireLights do
		scale = bulletFireLights[i].entity:get(core.componentType.ScaleComponent)
		if scale.scale[1] < 0.001 then
			bulletFireLights[i].entity:destroy()
			table.remove(bulletFireLights, i)
		else
			scale.scale[1] = scale.scale[1] * 0.65
			scale.scale[2] = scale.scale[2] * 0.65
			scale.scale[3] = scale.scale[3] * 0.65
			bulletFireLights[i].entity:set(core.componentType.ScaleComponent, scale)
		end
		i = i + 1
	end
end


local function UpdateEnemyTanks(delta)
	local playerPos = playerTank:get(core.componentType.WorldPositionComponent).position
	
	core.system.groups.setGroupGoal(tankGroups, playerPos[1], playerPos[2], playerPos[3] )
	
	local i = 1
	while i <= #enemyTanks do
	
		local wpc
		wpc = enemyTanks[i].entity:get(core.componentType.WorldPositionComponent)
		position = wpc.position
		
		dir = {playerPos[1] - position[1], playerPos[3] - position[3]}
		length = math.sqrt(dir[1] * dir[1] + dir[2] * dir[2])
		dir[1] = dir[1] / length
		dir[2] = dir[2] / length
		
		local toPlayerDir = math.atan2(dir[2], dir[1]) --+ math.pi
		enemyTankTurrets[i].direction = toPlayerDir
		
		
		--if toPlayerDir < 0 then
		--	toPlayerDir = toPlayerDir + math.pi * 2
		--end
		
		--print (toPlayerDir)
		--local dire = toPlayerDir-- - math.pi
		--
		--while dire < 0 do
		--	dire = dire + math.pi * 2
		--end
		--if dire > math.pi * 2 then
		--	dire = dire - math.pi * 2
		--end
		
		
		--local direa = toPlayerDir - (math.pi / 4)-- + math.pi / 2)
		--local direb = toPlayerDir + (math.pi / 4)-- + math.pi / 2)
		--
		--if direa < 0 then
		--	direa = direa + math.pi * 2
		--elseif direb >= 2 * math.pi then
		--	direb = direb - math.pi * 2
		--end
		
		--enemyTanks[i].direction = toPlayerDir
		
		--print (direa)
		--print (direb)
		--print (enemyTankTurrets[i].direction)
		--
		--if dire < enemyTanks[i].direction then
		--	enemyTanks[i].direction = enemyTanks[i].direction - enemyTurnSpeed * delta
		--elseif dire >= enemyTanks[i].direction then
		--	enemyTanks[i].direction = enemyTanks[i].direction + enemyTurnSpeed * delta
		--end
		
		--if dire < enemyTankTurrets[i].direction - math.pi then
		--	enemyTankTurrets[i].direction = enemyTankTurrets[i].direction - enemyTurnSpeedTurret * delta
		--else--if dire >= enemyTankTurrets[i].direction then
		--	enemyTankTurrets[i].direction = enemyTankTurrets[i].direction + enemyTurnSpeedTurret * delta
		--end
		
		--if enemyTanks[i].direction < 0 then
		--	enemyTanks[i].direction = enemyTanks[i].direction + math.pi * 2
		--elseif enemyTanks[i].direction >= 2 * math.pi then
		--	enemyTanks[i].direction = enemyTanks[i].direction - math.pi * 2
		--end
		
		--if enemyTankTurrets[i].direction < 0 then
		--	enemyTankTurrets[i].direction = enemyTankTurrets[i].direction + math.pi * 2
		--elseif enemyTankTurrets[i].direction >= 2 * math.pi then
		--	enemyTankTurrets[i].direction = enemyTankTurrets[i].direction - math.pi * 2
		--end
		
		
		
		--local bo = false
		--if (direa < enemyTanks[i].direction)
		--or (direb > enemyTanks[i].direction)
		--then
		--	bo = true
		--end
		

		--if length > 15.0 then-- and bo == false then
		--	enemyTanks[i].velocity[1] = enemyTanks[i].velocity[1] + math.cos(enemyTanks[i].direction) * acc
		--	enemyTanks[i].velocity[2] = enemyTanks[i].velocity[2] + math.sin(enemyTanks[i].direction) * acc
		--else
		--	enemyTanks[i].velocity[1] = enemyTanks[i].velocity[1] * decc
		--	enemyTanks[i].velocity[2] = enemyTanks[i].velocity[2] * decc
		--end
		--
		--
		--local speed = math.sqrt(enemyTanks[i].velocity[1] * enemyTanks[i].velocity[1] + enemyTanks[i].velocity[2] * enemyTanks[i].velocity[2])
		--if speed > enemyTopSpeed then
		--	enemyTanks[i].velocity[1] = enemyTopSpeed * (enemyTanks[i].velocity[1] / speed)
		--	enemyTanks[i].velocity[2] = enemyTopSpeed * (enemyTanks[i].velocity[2] / speed)
		--elseif speed < 0.001 then
		--	enemyTanks[i].velocity[1] = 0
		--	enemyTanks[i].velocity[2] = 0
		--end
		
		
		local tankPos = enemyTanks[i].entity:get(core.componentType.WorldPositionComponent)
		--tankPos.position[1] = tankPos.position[1] + enemyTanks[i].velocity[1] * delta
		--tankPos.position[2] = 0.0
		--tankPos.position[3] = tankPos.position[3] + enemyTanks[i].velocity[2] * delta
		--enemyTanks[i].entity:set(core.componentType.WorldPositionComponent, tankPos)
		--
		--local tankRot = enemyTanks[i].entity:get(core.componentType.RotationComponent)
		--tankRot.rotation[1] = 0
		--tankRot.rotation[2] = math.sin( (math.pi/2 - enemyTanks[i].direction)/2 )
		--tankRot.rotation[3] = 0
		--tankRot.rotation[4] = math.cos( (math.pi/2 - enemyTanks[i].direction)/2 )
		--enemyTanks[i].entity:set(core.componentType.RotationComponent, tankRot)
		
		
		--local turretPos = enemyTankTurrets[i].entity:get(core.componentType.WorldPositionComponent)
		--turretPos.position[1] = tankPos.position[1]
		--turretPos.position[2] = tankPos.position[2]
		--turretPos.position[3] = tankPos.position[3]
		enemyTankTurrets[i].entity:set(core.componentType.WorldPositionComponent, tankPos)

		local turretRot = enemyTankTurrets[i].entity:get(core.componentType.RotationComponent)
		turretRot.rotation[1] = 0
		turretRot.rotation[2] = math.sin( (math.pi/2 - enemyTankTurrets[i].direction)/2 )
		turretRot.rotation[3] = 0
		turretRot.rotation[4] = math.cos( (math.pi/2 - enemyTankTurrets[i].direction)/2 )
		enemyTankTurrets[i].entity:set(core.componentType.RotationComponent, turretRot)
		
		
		enemyTanks[i].shootTimer = enemyTanks[i].shootTimer - delta
		if enemyTanks[i].shootTimer < 0 then
			enemyTanks[i].shootTimer = enemyShootInterval
			ShootBullets(enemyTankTurrets[i].direction, {tankPos.position[1] + math.cos(enemyTankTurrets[i].direction) * 3, 0, tankPos.position[3] + math.sin(enemyTankTurrets[i].direction) * 3}, false)
		end		
		
		i = i + 1
	end
end


local function UpdatePlayerTank(delta)

	local moveForward = keyboard.isKeyDown( key.Up )
	local leftTurn = keyboard.isKeyDown( key.Left )
	local rightTurn = keyboard.isKeyDown( key.Right )
	local moveBackward = keyboard.isKeyDown( key.Down )
	
	local leftTurretTurn = keyboard.isKeyDown( key.A )
	local rightTurretTurn = keyboard.isKeyDown( key.D )
	
	local shootBullet = keyboard.isKeyDown( key.Space )
	local fireMissile = keyboard.isKeyDown( key.M )
	local fireRailgun = keyboard.isKeyDown( key.R )
	
	if leftTurn == true and isShootingMissiles == false then
		playerDirection = playerDirection - turnSpeed * delta
	end
	
	if rightTurn == true and isShootingMissiles == false then
		playerDirection = playerDirection + turnSpeed * delta
	end
	
	if playerDirection < 0 then
		playerDirection = 2 * math.pi
	elseif playerDirection > 2 * math.pi then
		playerDirection = 0
	end
	
	
	
	
	if leftTurretTurn == true and isShootingMissiles == false then
		playerTurretDirection = playerTurretDirection - turnSpeedTurret * delta
	end
	
	if rightTurretTurn == true and isShootingMissiles == false then
		playerTurretDirection = playerTurretDirection + turnSpeedTurret * delta
	end
	
	if playerTurretDirection < 0 then
		playerTurretDirection = 2 * math.pi
	elseif playerTurretDirection > 2 * math.pi then
		playerTurretDirection = 0
	end
	
	
	if moveForward == true and isShootingMissiles == false then
		velocity[1] = velocity[1] + math.cos(playerDirection) * acc
		velocity[2] = velocity[2] + math.sin(playerDirection) * acc
	elseif moveBackward == true and isShootingMissiles == false then
		velocity[1] = velocity[1] - math.cos(playerDirection) * acc
		velocity[2] = velocity[2] - math.sin(playerDirection) * acc
	else
		velocity[1] = velocity[1] * decc
		velocity[2] = velocity[2] * decc
	end
	
	
	
	local speed = math.sqrt(velocity[1] * velocity[1] + velocity[2] * velocity[2])
	if speed > topSpeed then
		velocity[1] = topSpeed * (velocity[1] / speed)
		velocity[2] = topSpeed * (velocity[2] / speed)
	elseif speed < 0.001 then
		velocity[1] = 0
		velocity[2] = 0
	end
	
	local tankPos = playerTank:get(core.componentType.WorldPositionComponent)

	tankPos.position[1] = tankPos.position[1] + velocity[1] * delta
	tankPos.position[2] = 0
	tankPos.position[3] = tankPos.position[3] + velocity[2] * delta
	
	
	
	
	
	
	
	shootTimer = shootTimer - delta
	if shootTimer < 0 then
		shootTimer = 0
	end
	
	if shootBullet and shootTimer == 0 then 
		shootTimer = shootInterval
	else 
		shootBullet = false 
	end

	if shootBullet then
		ShootBullets(playerTurretDirection, {tankPos.position[1] + math.cos(playerTurretDirection) * 3, 1.5, tankPos.position[3] + math.sin(playerTurretDirection) * 3}, true)
	end


	
	
	
	

	--Missile barrage
	if not isShootingMissiles then
		shootMissileTimer = shootMissileTimer - delta
	end
	
	if fireMissile == true
	and isShootingMissiles == false
	and shootMissileTimer <= 0
	then
		isShootingMissiles = true
		shootMissileTimer = shootMissileTimerInterval
	end
	
	if isShootingMissiles then
		missileLuanchIntervalTimer = missileLuanchIntervalTimer - delta
		if missileLuanchIntervalTimer < 0 then
			missileLuanchIntervalTimer = missileLuanchInterval
			currentMissileBarrage = currentMissileBarrage - 1
			ShootMissiles(playerTurretDirection + math.random() * 0.15, math.pi * 0.5 + math.random() * 0.25, {tankPos.position[1] - math.cos(playerTurretDirection) * 3, 1.5, tankPos.position[3] - math.sin(playerTurretDirection) * 3}, true)
		end
		
		if currentMissileBarrage < 0 then
			isShootingMissiles = false
			currentMissileBarrage = missileBarrage
		end
	end

	
	
	
	
	
	--Railgun
	shootRailgunTimer = shootRailgunTimer - delta
	
	if fireRailgun then
		shootRailgunBuildUpTimer = shootRailgunBuildUpTimer - delta
		shootRailgunTimer = shootRailgunTimerInterval
	else
		shootRailgunBuildUpTimer = shootRailgunBuildUpTimerInterval
		--shootRailgunTimer = shootRailgunTimerInterval
	end
	
	if shootRailgunBuildUpTimer <= 0 then
		ShootRailgun(playerTurretDirection, {tankPos.position[1] + math.cos(playerTurretDirection) * 3, 1.5, tankPos.position[3] + math.sin(playerTurretDirection) * 3}, true)
		shootRailgunBuildUpTimer = shootRailgunBuildUpTimerInterval
		shootRailgunTimer = shootRailgunTimerInterval
	end
	
	
	
	
	--tankPos.position[1] = playerPosition[1]
	--tankPos.position[2] = 0.0
	--tankPos.position[3] = playerPosition[2]
	playerTank:set(core.componentType.WorldPositionComponent, tankPos)
	
	local tankRot = playerTank:get(core.componentType.RotationComponent)
	tankRot.rotation[1] = 0
	tankRot.rotation[2] = math.sin( (math.pi/2 - playerDirection)/2 )
	tankRot.rotation[3] = 0
	tankRot.rotation[4] = math.cos( (math.pi/2 - playerDirection)/2 )
	playerTank:set(core.componentType.RotationComponent, tankRot)
	
	
	
	local turretPos = playerTurret:get(core.componentType.WorldPositionComponent)
	turretPos.position[1] = tankPos.position[1]
	turretPos.position[2] = 0.0
	turretPos.position[3] = tankPos.position[3]
	playerTurret:set(core.componentType.WorldPositionComponent, turretPos)
	
	local turretRot = playerTurret:get(core.componentType.RotationComponent)
	turretRot.rotation[1] = 0
	turretRot.rotation[2] = math.sin( (math.pi/2 - playerTurretDirection)/2 )
	turretRot.rotation[3] = 0
	turretRot.rotation[4] = math.cos( (math.pi/2 - playerTurretDirection)/2 )
	playerTurret:set(core.componentType.RotationComponent, turretRot)
	
	local aimPos = aim:get(core.componentType.WorldPositionComponent)
	aimPos.position[1] = tankPos.position[1] + math.cos( playerTurretDirection) * 10.0
	aimPos.position[2] = 0.0
	aimPos.position[3] = tankPos.position[3] + math.sin( playerTurretDirection) * 10.0
	aim:set(core.componentType.WorldPositionComponent, aimPos)
end


local function UpdateCamera()
	local tankPos = playerTank:get(core.componentType.WorldPositionComponent)
	camera:lookAt( core.glm.vec3.new( tankPos.position[1], 50, tankPos.position[3] ), core.glm.vec3.new( tankPos.position[1], 0, tankPos.position[3] ))
	core.camera.gameCamera:setView( camera:getView( ) )
end


local function RestartGame(diff)
	-- Clear tables
	local i = 1
	while i <= #enemyTanks do
		enemyTanks[i].entity:destroy()
		enemyTanks[i] = nil
		table.remove(enemyTanks, i)
		
		enemyTankTurrets[i].entity:destroy()
		enemyTankTurrets[i] = nil
		table.remove(enemyTankTurrets, i)
	end
	
	i = 1
	while i <= #bullets do
		bullets[i].entity:destroy()
		bullets[i] = nil
		table.remove(bullets, i)
		
		bulletLights[i].entity:destroy()
		bulletLights[i] = nil
		table.remove(bulletLights, i)
	end
	
	
	-- Reset variables
	alive = true
	health = maxHealth
	
	local tankPos = playerTank:get(core.componentType.WorldPositionComponent)
	tankPos.position[1] = 0.0
	tankPos.position[2] = 0.0
	tankPos.position[3] = 0.0
	playerTank:set(core.componentType.WorldPositionComponent, tankPos)
	
	velocity = {0,0}
	playerDirection = 0
	playerTurretDirection = 0
	shootTimer = 0
	difficulty = diff
	
	--Create enemy tanks
	InitEnemyTanks(difficulty)
end


function Update(delta)

	if alive then
		UpdatePlayerTank(delta)
		UpdateEnemyTanks(delta)
		UpdateCamera()
	end
	
	UpdateBullets(delta)
	UpdateMissiles(delta)
	UpdateRailgun(delta)
	UpdateExplosions(delta)
	UpdateBulletFireLights(delta)
	
	if 0.0 >= health then
		alive = false
	end
	
	
	local enterIsDown = false
	if keyboard.isKeyDown( key.Enter ) then
		if not enterWasDown then
			enterIsDown = true
		end
		enterWasDown = true
	else
		enterWasDown = false
	end
	
	if not alive then
	
		core.draw.drawText( 605, 300, "YOU DED!" )
		core.draw.drawText( 580, 340, "FINAL SCORE: "..score )
		core.draw.drawText( 590, 380, "PRESS ENTER!" )
		
		if enterIsDown then
			health = maxLives
			difficulty = 3
			score = 0
			level = 1
			--started = false
			RestartGame(difficulty)
			return
		end
		return
	end
	
	
	
	if #enemyTanks == 0 then
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
	core.draw.drawText( 300, 75, "HEALTH: "..health )
	
	
	
	--if keyboard.isKeyDown( key.Left ) then
	--	mythingPos.position[1] =  mythingPos.position[1] - 100  * dt
	--elseif keyboard.isKeyDown( key.Right ) then
	--	mythingPos.position[1] =  mythingPos.position[1] + 100  * dt
	--elseif keyboard.isKeyDown (key.Up) then
	--	mythingPos.position[3] =  mythingPos.position[3] - 100  * dt
	--elseif keyboard.isKeyDown (key.Down) then
	--	mythingPos.position[3] =  mythingPos.position[3] + 100  * dt
	--end

	--camera:update(delta)
end

scen:registerUpdateCallback( Update )

return scen;