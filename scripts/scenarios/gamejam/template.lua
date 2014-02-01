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

local mything = scen:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 30, 10, -10 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/cube.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/simple.material", false }
			   }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 10.5 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { 0, 1*math.sin( -3.14/4 ), 0, math.cos( -3.14/4 ) } }
	}
}
)

---- Ambient light
scen:loadAssembly( 
{
	{
		type = core.componentType.LightComponent,
		data =  { 
					color = { 1.0, 1.0, 1.0 },
					intensity = 0.01,
					type = core.gfx.objectTypes.Light,
					lighttype = core.gfx.lightTypes.Ambient,
					spotangle = 0,
					spotpenumbra = 0,
					speccolor = {0,0,0}
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

function Update(dt)

	local mythingPos
	mythingPos = mything:get(core.componentType.WorldPositionComponent)


	if keyboard.isKeyDown( key.Left ) then
		mythingPos.position[1] =  mythingPos.position[1] - 100  * dt
	elseif keyboard.isKeyDown( key.Right ) then
		mythingPos.position[1] =  mythingPos.position[1] + 100  * dt
	elseif keyboard.isKeyDown (key.Up) then
		mythingPos.position[3] =  mythingPos.position[3] - 100  * dt
	elseif keyboard.isKeyDown (key.Down) then
		mythingPos.position[3] =  mythingPos.position[3] + 100  * dt
	end

	mything:set(core.componentType.WorldPositionComponent, mythingPos)

end

scen:registerUpdateCallback( Update )

return scen;