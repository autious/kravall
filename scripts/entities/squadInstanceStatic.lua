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
            {
                type = core.componentType.BoundingVolumeComponent,
                data = {
                    sphereOffset = { 0, 0, 0 }, 
                    sphereRadius = 0.1, 
                    pickingRadius = 3,
                    collisionModel = core.BoundingVolumeCollisionModel.StaticResolution, 
                    type = core.BoundingVolumeType.SphereBoundingType
                } 
            }
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

    function e:updatePosition()
        self:setPosition(squadInstance.position)
    end

    function e:setPosition( pos )
        for i,v in ipairs( self ) do
            local p = pos + e.offsets[v]
            v:set( core.componentType.WorldPositionComponent, {position={p:get()}}, true )
        end
    end

    function e:destroy()
        for _,v in ipairs(self) do
            v:destroy()
        end
    end
    e.offsets = {}
    for i,v in pairs( squadInstance.squadDef.setup ) do
        local mem = createMember(scen, squadInstance.position + core.glm.vec3.new( unpack( v.positionOffset ) ), v )
        table.insert( e, mem )
        e.offsets[mem] = core.glm.vec3.new( unpack( v.positionOffset ) )
    end

    return e
end
