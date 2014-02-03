local ent = require "entities"
local scenario = require "scenario"

local asm = scenario.new()

asm.asm:specific_content( core.contentmanager.load( 
		core.loaders.NavigationMeshLoader, "extremeScenario.txt", function( value ) end, false ) )

asm.gamemode =  require "gamemodes/normal":new()

asm:registerUpdateCallback( function( delta ) asm.gamemode:update(delta) end )
asm:registerDestroyCallback( function() asm.gamemode:destroy() end )


--local ambient = ent.get "ambientLight"
--local directional = ent.get "directionalLight"
--ambient.create(asm, 1.0, 1.0, 1.0, 0.1)
--directional.create(asm, -1, -1, 0.5)

local rioter = ent.get "rioter"
local police = ent.get "police"
local building = ent.get "building"
local centerPoint = { 0, 0, 0 }
local side = 32 -- math.sqrt( 1000 )


local gOne = core.system.groups.createGroup()
local gTwo = core.system.groups.createGroup()
local gThree = core.system.groups.createGroup()
local gFour = core.system.groups.createGroup()

for i = -side / 2, -1 do
	for p = -side / 2, -1 do
		rioter( asm, p * 2.0 + centerPoint[1], 0  + centerPoint[2], i * 2.0  + centerPoint[3], gOne )
	end                         
	for p = 1, side / 2 do      
		rioter( asm, p * 2.0 + centerPoint[1], 0  + centerPoint[2], i * 2.0  + centerPoint[3], gTwo )
	end                         
end                             
for i = 1, side / 2 do          
	for p = -side / 2, -1 do    
		rioter( asm, p * 2.0 + centerPoint[1], 0  + centerPoint[2], i * 2.0  + centerPoint[3], gThree )
	end                         
	for p = 1, side / 2 do      
		rioter( asm, p * 2.0 + centerPoint[1], 0  + centerPoint[2], i * 2.0  + centerPoint[3], gFour )
	end
end

core.system.groups.setGroupGoal(gFour, -70, 0, 40)
core.system.groups.setGroupGoal(gThree, -70, 0, -40)
core.system.groups.setGroupGoal(gTwo, 70, 0, 40)
--core.nav_mesh.set_group_goal(2, -50, 0, 4)
--core.nav_mesh.set_group_goal(3, -1, 0, 4)



for i = 0, 100 do
	local color = {1, 1, 1 }
    asm:loadAssembly( 
    {
        {
            type = core.componentType.LightComponent,
            data =  { 
                        color = color,
                        speccolor = color,
                        intensity = 1.0,
                        type = core.gfx.objectTypes.Light,
                        lighttype = core.gfx.lightTypes.Point,
						spotangle = 0,
						spotpenumbra = 0
                    }
                    ,
                    ignoreHard = true
        },
        {
            type = core.componentType.WorldPositionComponent,
            data = { position = { -20.0 + 100.0 * math.random(), 5 + 2.5 * math.random(), -20.0 + 100.0 * math.random() } }
        },
        {
            type = core.componentType.ScaleComponent,
            data = { scale = 10.0 + 5.0 * math.random() }
        },
        {
            type = core.componentType.RotationComponent,
            data = { rotation = { 0.0,0.0,0.0,0 } }
        }
    } 
    )
end

rioter( asm, 0, 0, 0, 0 )
core.system.groups.setGroupGoal(gOne, -30, 0, 0)

asm.gamemode.camera:lookAt( core.glm.vec3.new( 65, 65, 65 ), core.glm.vec3.new( 0, 0, 0 ) )

return asm;




