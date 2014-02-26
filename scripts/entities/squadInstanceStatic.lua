local ent = require "entities"

local function createMember( scen, pos, memberDef )

    assert( pos )
    assert( memberDef )

    return scen:loadAssembly( 
		{
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { pos:get() } }
			},
			{
				type = core.componentType.GraphicsComponent,
				data = { render = true, mesh = 2000, material = 2000, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 1, 0, 1} },
				load = { 
							mesh = { core.loaders.GnomeLoader, memberDef.mesh },
							material = { core.loaders.MaterialLoader, memberDef.material }
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
				type = core.componentType.AnimationComponent,
				data = { 
					animationID = 0,
					currentTime = 0.0,
					loop = false,
					playing = false,
					speed = 1.6,
					currentFrame = 0,
					queuedAnimationID = 0
				}
			},
		}
	)
end

return function( scen, squadInstance )
    local e = {}

    function e:setOutline( color )
        if color then
            for _,v in ipairs(self) do
                v:set( core.componentType.GraphicsComponent, { layer = core.gfx.layerTypes.OutlineLayer, outlineColor = color}, true )
            end
        else
            for _,v in ipairs(self) do
                v:set( core.componentType.GraphicsComponent, { layer = core.gfx.layerTypes.MeshLayer }, true )
            end
        end
    end

    function e:destroy()
        for _,v in ipairs(self) do
            v:destroy()
        end
    end

    for i,v in pairs( squadInstance.squadDef.setup ) do
        table.insert( e, createMember(scen, squadInstance.position + core.glm.vec3.new( unpack( v.positionOffset ) ), v ) )
    end

    return e
end
