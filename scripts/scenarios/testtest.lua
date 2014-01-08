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
					
for i = -1, 3 do
	for p = 0, 10 do
		asm:loadAssembly( 
			{
				{
					type = core.componentType.WorldPositionComponent,
					data = { position = { p * 5 + 1, 0, i * 2 } }
				},
				{
					type = core.componentType.GraphicsComponent,
					data = { mesh = 2000, material = 2000, type = core.gfx.objectTypes.OpaqueGeometry },
					load = { 
								mesh = { core.loaders.GnomeLoader, "assets/teapot.bgnome" },
								material = { core.loaders.MaterialLoader, "assets/material/test-material.material" }
						   }
				},
				{
					type = core.componentType.ScaleComponent,
					data = { scale = 1.0 }
				},
				{
					type = core.componentType.RotationComponent,
					data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
				},
				{
					type = core.componentType.UnitTypeComponent,
					data = { unitType = core.UnitType.Rioter }
				},
				{
					type = core.componentType.MovementComponent,
					--data = { direction = { -1, 0, -1 }, speed = 5.8, maxSpeed = 50 }
					data = { direction = { 0, 0, 0 }, speed = 5.8, maxSpeed = 50 }
				},
				{
					type = core.componentType.AttributeComponent,
					data = { health = 0, stamina = 0, morale = 0, 
						   alignment = core.RioterAlignment.Anarchist, rage = 0, pressure = 0, groupID = 0 }
				},
				{
					type = core.componentType.BoundingVolumeComponent,
					data = { sphereOffset = { 0, 0, 0 }, sphereRadius = 1.5, 
							collisionModel = core.BoundingVolumeCollisionModel.DynamicResolution, 
							type = core.BoundingVolumeType.SphereBoundingType }
				}
			}
		)
	end
end
	
--collectgarbage()
return asm;




