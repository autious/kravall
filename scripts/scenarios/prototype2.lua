local ent = require "entities"
local ASML = require "assembly_loader" 
local asm = ASML.loadPack( {} )

asm:specific_content( core.contentmanager.load( 
		core.loaders.NavigationMeshLoader, "extremeScenario.txt", function( value ) end, false ) )

local rioter = ent.get "rioter"
local police = ent.get "police"
local building = ent.get "building"
		
local centerPoint = { 0, 0, 0 }
local side = 32 -- math.sqrt( 1000 )
for i = -side / 2, -1 do
	for p = -side / 2, -1 do
		rioter( asm, p * 2.0 + centerPoint[1], 0  + centerPoint[2], i * 2.0  + centerPoint[3], 0 )
	end                         
	for p = 1, side / 2 do      
		rioter( asm, p * 2.0 + centerPoint[1], 0  + centerPoint[2], i * 2.0  + centerPoint[3], 1 )
	end                         
end                             
for i = 1, side / 2 do          
	for p = -side / 2, -1 do    
		rioter( asm, p * 2.0 + centerPoint[1], 0  + centerPoint[2], i * 2.0  + centerPoint[3], 2 )
	end                         
	for p = 1, side / 2 do      
		rioter( asm, p * 2.0 + centerPoint[1], 0  + centerPoint[2], i * 2.0  + centerPoint[3], 3 )
	end
end

rioter( asm, 0, 0, 0, 0 )
core.nav_mesh.set_group_goal(0, -30, 0, 0)

camera:lookAt( core.glm.vec3.new( 65, 65, 65 ), core.glm.vec3.new( 0, 0, 0 ) )




return asm;




