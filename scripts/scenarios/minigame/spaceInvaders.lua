local entities = require "entities"
local scenario = require "scenario"
local input = require "input" 
local scen = scenario.new()

local mouse = core.input.mouse
local keyboard = core.input.keyboard
local key = keyboard.key

local spaceship = scen:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 30, 10, 50 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry },
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
--Plane
scen:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, 10, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry },
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
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry },
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

camera:lookAt( core.glm.vec3.new( 0, 200, 0 ), core.glm.vec3.new( 0, 0, 0 ) )

local score = 0
local player = {}
player.speed = 100

function ControlPlayer(dt)
	
	local wpc = spaceship:get(core.componentType.WorldPositionComponent)
	
	if keyboard.iskeydown( key.Left ) then
		wpc.position[1] =  wpc.position[1] + player.speed  * dt
	elseif keyboard.iskeydown( key.Right ) then
		wpc.position[1] =  wpc.position[1] - player.speed  * dt
	end

	spaceship:set(core.componentType.WorldPositionComponent, wpc)

end

function Update(dt)
	ControlPlayer(dt)
end

scen:registerUpdateCallback(Update)

return scen