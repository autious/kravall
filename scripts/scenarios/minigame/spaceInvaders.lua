local entities = require "entities"
local scenario = require "scenario"
local input = require "input" 
local scen = scenario.new()

local mouse = core.input.mouse
local keyboard = core.input.keyboard
local key = keyboard.key

--Plane
scen:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, 10, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
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
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
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


local spaceship = scen:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 30, 10, 55 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
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
		data = { rotation = { math.sin( 3.14 / 2 ) ,0, 0, math.cos(3.14/2.0) } }
	}
}
)

function CreateShot(x, z)
	local shot = scen:loadAssembly( 
	{
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = { x, 10, z } }
		},
		{
			type = core.componentType.GraphicsComponent,
			data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
			load = { 
						mesh = { core.loaders.GnomeLoader, "assets/sphere.bgnome", false },
						material = { core.loaders.MaterialLoader, "assets/material/asteroids/roids_white.material", false }
				   }
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = 1.5 }
		},
		{
			type = core.componentType.RotationComponent,
			data = { rotation = { math.sin( 3.14 / 2 ) ,0, 0, math.cos(3.14/2.0) } }
		}
	}
	)

	return shot
end

function CreateEnemy(x, z)

	local enemy = scen:loadAssembly( 
	{
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = { x, 10, z } }
		},
		{
			type = core.componentType.GraphicsComponent,
			data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
			load = { 
						mesh = { core.loaders.GnomeLoader, "assets/teapot.bgnome", false },
						material = { core.loaders.MaterialLoader, "assets/material/asteroids/roids_white.material", false }
				   }
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = 1.5 }
		},
		{
			type = core.componentType.RotationComponent,
			data = { rotation = { math.sin( 3.14 / 2 ) ,0, 0, math.cos(3.14/2.0) } }
		}
	}
	)

	return shot

end

camera:lookAt( core.glm.vec3.new( 0, 200, 0 ), core.glm.vec3.new( 0, 0, 0 ) )

local score = 0
local shotLastFrame = false
local player = {}
player.speed = 100
player.shots = {}

local enemies = {}

function Shoot(wpc)

	local shot = {}
	shot.x = wpc.position[1]
	shot.y = wpc.position[3] - 5
	shot.radius = 0.2
	shot.instance = CreateShot(shot.x, shot.y)
	
	table.insert(player.shots, shot)
end

function ControlPlayer(dt)
	
	local wpc = spaceship:get(core.componentType.WorldPositionComponent)
	
	if keyboard.iskeydown( key.Left ) and wpc.position[1] > -70 then
		wpc.position[1] =  wpc.position[1] - player.speed  * dt
	elseif keyboard.iskeydown( key.Right ) and wpc.position[1] < 70 then
		wpc.position[1] =  wpc.position[1] + player.speed  * dt
	end
	
	if keyboard.iskeydown (key.L) and not shotLastFrame then
		Shoot(wpc)
		shotLastFrame = true
	elseif not keyboard.iskeydown (key.L) then
		shotLastFrame = false
	end

	spaceship:set(core.componentType.WorldPositionComponent, wpc)
end

function UpdateShots(dt)
	
	local rmShots = {}

	for i,v in ipairs(player.shots) do
        local wpc = v.instance:get(core.componentType.WorldPositionComponent)
		wpc.position[3] = wpc.position[3] - 100 * dt
		v.instance:set(core.componentType.WorldPositionComponent, wpc)

		if wpc.position[3] < -73 then
			table.insert(rmShots, i)
		end
	end

	for i,v in ipairs(rmShots) do
		--v.instance.destroy()
		player.shots[v].instance:destroy()
		table.remove(player.shots,v)
		--v.instance:destroy()
	end
end

function Update(dt)
	ControlPlayer(dt)
	UpdateShots(dt)
end

scen:registerUpdateCallback(Update)

return scen