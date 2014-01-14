
local ASML = require "assembly_loader" 
local asm = ASML.loadPack( {} )

navMesh = core.contentmanager.load( 
		core.loaders.NavigationMeshLoader, "extremeScenario.txt", function( value ) end, false )

local rioter = require "entities/rioter"
local police = require "entities/police"
local building = require "entities/building"
		
local centerPoint = { 0, 0, 0 }
local side = math.sqrt( 100 )
for i = -side / 2, side / 2 do
	for p = -side / 2, side / 2 do
		rioter.create( asm, p * 1.5 + centerPoint[1], 0  + centerPoint[2], i * 1.5  + centerPoint[3], 0 )
	end
end

--camera:setPosition( core.glm.vec3.new( 45, 45, 45 ) )
camera:lookAt( core.glm.vec3.new( 65, 65, 65 ), core.glm.vec3.new( 0, 0, 0 ) )




return asm;




