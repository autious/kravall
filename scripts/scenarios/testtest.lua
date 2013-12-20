-- attempts to load a navmesh from lua

--load = { mesh = { core.loaders.GnomeLoader, "assets/cube.bgnome" } }
--load = { mesh = { core.loaders.NavigationMeshLoader, "testNaveMesh.txt" } }

local ASML = require "assembly_loader" 
local asm = ASML.loadPack( {} )

tjo = core.contentmanager.load( 
		core.loaders.NavigationMeshLoader, "testNaveMesh.txt", function( value ) end, false )

		assets[#assets+1] = core.contentmanager.load( loader_pair[1], loader_pair[2], function( value )
                        component.data[index] = value
                        count = count - 1
                        
                        -- If this is a later callback and we've passed the loadAssembly function
                        -- consider applying the metatable
                        if( count == 0 and passedload == true ) then
                            apply( asmtable )
                        end

                    end, async_load)
					
					material = { core.loaders.MaterialLoader, "assets/material/test-material.material" }

					
					
asm:loadAssembly( 
	{
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = { 5, 0, -15 } }
		},
		{
			type = core.componentType.GraphicsComponent,
			data = { mesh = 2000, material = 2000 },
			load = { 
						mesh = { core.loaders.GnomeLoader, "assets/teapot.bgnome" },
						material = { core.loaders.MaterialLoader, "assets/material/test-material.material" }
				   }
		
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = 0.1 }
		},
		{
			type = core.componentType.RotationComponent,
			data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
		}
	} 
)
collectgarbage()
