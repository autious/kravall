
local scenario = require "scenario"
local input = require "input" 
local ent = require "entities"
local scen = scenario.new()

--local ASML = require "assembly_loader" 
--local asm = ASML.loadPack( {} )


scen.asm:specific_content( core.contentmanager.load( 
		core.loaders.NavigationMeshLoader, "extremeScenario.txt", function( value ) end, false ) )

local rioter = ent.get "rioter"
local police = ent.get "police"
local building = require "entities/building"
		
--local centerPoint = { -5, 0, 0 }
--local side = math.sqrt( 50 )
--for i = -side / 2, side / 2 do
--	for p = -side / 2, side / 2 do
--		rioter( asm, p * 1.5 + centerPoint[1], 0  + centerPoint[2], i * 1.5  + centerPoint[3], 0 )
--	end
--end

--rioter( asm, 0, 0, 0, 0 )
--rioter( asm, 1, 0, 1, 0 )
--rioter( asm, 3, 0, 3, 0 )
--rioter( asm, 4, 0, 4, 0 )
--rioter( asm, 10, 0, 10, 0 )
--rioter( asm, 11, 0, 11, 0 )

local squadID = core.system.groups.createGroup()
police( scen, 0, 0, 0, squadID );
--police( asm, 0, 0, 0, squadID );
--police( asm, 0, 0, 0, squadID );
--police( asm, 0, 0, 0, squadID );


camera:lookAt( core.glm.vec3.new( 55, 55, 55 ), core.glm.vec3.new( 0, 0, 0 ) )

local function picking()
	
	
	
	
	
end

scen:registerUpdateCallback( picking )


return scen;






