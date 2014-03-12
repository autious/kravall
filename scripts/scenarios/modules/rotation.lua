return function( scen, T )
    function T.rotateY( ent, delta )
        local rotation = ent:get( core.componentType.RotationComponent ).rotation

        local qrot = core.glm.quat.new( unpack(rotation) )
        
        qrot = qrot:rotate( delta*10, core.glm.vec3.new(0,1,0) )

        ent:set( core.componentType.RotationComponent, { rotation = {qrot:get()}})
    end

    function T.rotateX( ent, delta )
        local rotation = ent:get( core.componentType.RotationComponent ).rotation

        local qrot = core.glm.quat.new( unpack(rotation) )
        
        qrot = qrot:rotate( delta*10, core.glm.vec3.new(1,0,0) )

        ent:set( core.componentType.RotationComponent, { rotation = {qrot:get()}})
    end

    function T.rotateZ( ent, delta )
        local rotation = ent:get( core.componentType.RotationComponent ).rotation

        local qrot = core.glm.quat.new( unpack(rotation) )
        
        qrot = qrot:rotate( delta*10, core.glm.vec3.new(0,0,1) )

        ent:set( core.componentType.RotationComponent, { rotation = {qrot:get()}})
    end

    function T.rotateYLight( ent, delta )

        local r = ent:get(core.componentType.RotationComponent).rotation
        r[4] = 0
        local vec = core.glm.vec4.new(unpack(r))

        local qrot = core.glm.quat.new( )
        
        qrot = qrot:rotate( delta*10, core.glm.vec3.new(0,1,0) )
        local d = qrot:mat4Cast() 
        local newD = d * vec     
        
        ent:set( core.componentType.RotationComponent, { rotation = {newD:get()}})
    end
end
