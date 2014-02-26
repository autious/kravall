local c = core.componentType
local SquadPositionDecal = {}
function SquadPositionDecal:new(o)
    o = o or {}
    setmetatable(o,self)
    self.__index = self

    self.offsets = {}
    self.origin = core.glm.vec3.new( 0,0,0 )

    o.mesh = core.contentmanager.load( 
        core.loaders.GnomeLoader, 
        "assets/model/dev/cube.bgnome", 
        function( vao )
            o.vao_handle = vao
        end, 
        false )

    o.material = core.contentmanager.load( 
        core.loaders.MaterialLoader, 
        "assets/texture/dev/positionPreview.material", 
        function( mat )
            o.material_handle = mat
        end, 
        false )

    o.fail_material = core.contentmanager.load( 
        core.loaders.MaterialLoader, 
        "assets/texture/dev/positionPreviewRed.material", 
        function( mat )
            o.fail_material_handle = mat
        end, 
        false )


    return o
end


function SquadPositionDecal:createInstance( pos  )

    local entity = core.entity.create( c.WorldPositionComponent,
                                   c.GraphicsComponent,
                                   c.ScaleComponent,
                                   c.RotationComponent )
    entity:set( c.GraphicsComponent, 
    { 
        mesh = self.vao_handle, 
        material = self.material_handle, 
        type = core.gfx.objectTypes.DecalGeometry,
        layer = core.gfx.layerTypes.MeshLayer, 
        outlineColor = {1, 1, 1, 1}, 
        render = true
    })

    entity:set( c.ScaleComponent, { scale = 1 } )
    entity:set( c.RotationComponent, { rotation = { 0, 0, 0, 1 }} )
    entity:set(c.WorldPositionComponent, {position = {(pos):get()}}, true)

    return entity
end

function SquadPositionDecal:setPosition(x, y, z)
    self.origin = core.glm.vec3.new(x,y,z)

    for _,v in pairs( self.offsets ) do
        self.entities[v]:set(c.WorldPositionComponent, {position = {(v+self.origin):get()}}, true)
    end
end

function SquadPositionDecal:verifyPlacement( areas )

    for _,h in pairs( self.offsets ) do
        local isInside = false
        for _,v in pairs( areas ) do
            isInside = isInside or core.system.area.isPointInside( v, h+self.origin )
        end

        if  isInside then
            self.entities[h]:set( c.GraphicsComponent, 
            { 
                material = self.material_handle, 
            }, true)
        else
            self.entities[h]:set( c.GraphicsComponent, 
            { 
                material = self.fail_material_handle, 
            }, true)
        end
    end


end

function SquadPositionDecal:setFormationOffsets( list )
    if self.entities then
        for i,v in pairs(self.entities) do
            v:destroy()
        end
    end

    self.entities = {}
    self.offsets = {}

    for i,v in pairs(list) do
        local ent = self:createInstance( self.origin + v )
        self.entities[v] = ent
        table.insert( self.offsets, v )
    end 
end

function SquadPositionDecal:destroy()
    if self.entities then
        for i,v in pairs(self.entities) do
            v:destroy()
        end
    end
    self.entities = nil
    self.mesh:free()
    self.fail_material:free()
    self.material:free()
    self.mesh = nil
    self.material = nil
    self.fail_material = nil
end

return SquadPositionDecal
