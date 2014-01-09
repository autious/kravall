-- attempts to load a navmesh from lua

--load = { mesh = { core.loaders.GnomeLoader, "assets/cube.bgnome" } }
--load = { mesh = { core.loaders.NavigationMeshLoader, "testNaveMesh.txt" } }

local ASML = require "assembly_loader" 
local asm = ASML.loadPack( {} )

--tjo = core.contentmanager.load( 
--		core.loaders.NavigationMeshLoader, "testNaveMesh.txt", function( value ) end, false )
--
--		assets[#assets+1] = core.contentmanager.load( loader_pair[1], loader_pair[2], function( value )
--                        component.data[index] = value
--                        count = count - 1
--                        
--                        -- If this is a later callback and we've passed the loadAssembly function
--                        -- consider applying the metatable
--                        if( count == 0 and passedload == true ) then
--                            apply( asmtable )
--                        end
--
--                    end, async_load)
--					
--					material = { core.loaders.MaterialLoader, "assets/material/test-material.material" }

tjo = core.contentmanager.load( 
		--core.loaders.NavigationMeshLoader, "testNaveMesh.txt", function( value ) end, false )
		core.loaders.NavigationMeshLoader, "testNaveMesh2.txt", function( value ) end, false )

local rioter = require "entities/teapot"
local police = require "entities/police"
					
for i = -1, 3 do
	for p = 1, 20 do
		--rioter.create( asm, -10 + p * 2, 0, 5 + i * 2 )
	end
end

rioter.create( asm, -10, 0, 5, 1, 0 )
rioter.create( asm, 10, 0, 5, -1, 0 )

--for i = 0, 1 do	
--	for p = 1, 10 do
--		police.create( asm, -15 + i * 1.6, 0, p * 1.6 )
--	end
--end
--collectgarbage()	
return asm;




