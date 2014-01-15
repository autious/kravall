---- attempts to load a navmesh from lua
--
----load = { mesh = { core.loaders.GnomeLoader, "assets/cube.bgnome" } }
----load = { mesh = { core.loaders.NavigationMeshLoader, "testNaveMesh.txt" } }
--
--local ASML = require "assembly_loader" 
--local asm = ASML.loadPack( {} )
--
--tjo = core.contentmanager.load( 
--		core.loaders.NavigationMeshLoader, "testNaveMesh2.txt", function( value ) end, false )
--
--local rioter = require "entities/teapot"
--local police = require "entities/police"
--					
--for i = -0, 3 do
--	for p = 1, 20 do
--		rioter.create( asm, -10 + p * 2, 0, 5 + i * 2 )
--	end
--end
--
----rioter.create( asm, -10, 0, 5, 1, 0 )
----rioter.create( asm, 10, 0, 5, -1, 0 )
--
----for i = 0, 1 do	
----	for p = 1, 10 do
----		police.create( asm, -15 + i * 1.6, 0, p * 1.6 )
----	end
----end
--
--
--collectgarbage()	
--return asm;


local ASML = require "assembly_loader" 
local asm = ASML.loadPack( {} )

asm:specific_content( core.contentmanager.load( 
		core.loaders.NavigationMeshLoader, "prototypeLevel.txt", function( value ) end, false ) )
		--core.loaders.NavigationMeshLoader, "extremeScenario.txt", function( value ) end, false ) )

local rioter = require "entities/rioter"
local police = require "entities/police"
local building = require "entities/building"
		
local centerPoint = { 0, 0, 0 }
local side = math.sqrt( 10 )
for i = -side / 2, side / 2 do
	for p = -side / 2, side / 2 do
		police.create( asm, p * 1.5 + centerPoint[1], 0  + centerPoint[2], i * 1.5  + centerPoint[3], 0 )
	end
end

--rioter.create( asm, 0, 0, 0, 0 )

camera:lookAt( core.glm.vec3.new( 65, 65, 65 ), core.glm.vec3.new( 0, 0, 0 ) )




return asm;






