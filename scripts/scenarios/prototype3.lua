
local ASML = require "assembly_loader" 
local asm = ASML.loadPack( {} )

asm:specific_content( core.contentmanager.load( 
		core.loaders.NavigationMeshLoader, "extremeScenario.txt", function( value ) end, false ) )

local rioter = require "entities/rioter"
local police = require "entities/police"
local building = require "entities/building"
		
local centerPoint = { 0, 0, 0 }
local side = 32 -- math.sqrt( 1000 )
for i = -side / 2, -1 do
	for p = -side / 2, -1 do
		rioter.create( asm, p * 2.0 + centerPoint[1], 0  + centerPoint[2], i * 2.0  + centerPoint[3], 0, fists )
	end                         
	for p = 1, side / 2 do      
		rioter.create( asm, p * 2.0 + centerPoint[1], 0  + centerPoint[2], i * 2.0  + centerPoint[3], 1, fists )
	end                         
end                             
for i = 1, side / 2 do          
	for p = -side / 2, -1 do    
		rioter.create( asm, p * 2.0 + centerPoint[1], 0  + centerPoint[2], i * 2.0  + centerPoint[3], 2, fists )
	end                         
	for p = 1, side / 2 do      
		rioter.create( asm, p * 2.0 + centerPoint[1], 0  + centerPoint[2], i * 2.0  + centerPoint[3], 3, fists )
	end
end

for i = -side / 2, -1 do
	for p = -side / 2, -1 do
		rioter.create( asm, p * 2.0 + centerPoint[1], 0  + centerPoint[2], i * 2.0  + centerPoint[3], 0, fists )
	end                         
	for p = 1, side / 2 do      
		rioter.create( asm, p * 2.0 + centerPoint[1], 0  + centerPoint[2], i * 2.0  + centerPoint[3], 1, fists )
	end                         
end                             
for i = 1, side / 2 do          
	for p = -side / 2, -1 do    
		rioter.create( asm, p * 2.0 + centerPoint[1], 0  + centerPoint[2], i * 2.0  + centerPoint[3], 2, fists )
	end                         
	for p = 1, side / 2 do      
		rioter.create( asm, p * 2.0 + centerPoint[1], 0  + centerPoint[2], i * 2.0  + centerPoint[3], 3, fists )
	end
end

for i = 0, 1023 do
	local color = { math.random(), math.random(), math.random() }
    asm:loadAssembly( 
    {
        {
            type = core.componentType.LightComponent,
            data =  { 
                        color = color,
                        speccolor = color,
                        intensity = 1.0,
                        type = core.gfx.objectTypes.Light,
                        lighttype = core.gfx.lightTypes.Point
                    }
        },
        {
            type = core.componentType.WorldPositionComponent,
            data = { position = { -50.0 + 100.0 * math.random(), 5 + 2.5 * math.random(), -50.0 + 100.0 * math.random() } }
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

rioter.create( asm, 0, 0, 0, 0, fists )
core.nav_mesh.set_group_goal(0, -30, 0, 0)

camera:lookAt( core.glm.vec3.new( 65, 65, 65 ), core.glm.vec3.new( 0, 0, 0 ) )




return asm;




