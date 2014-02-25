local c = core.componentType
local Reticule = {}
function Reticule:new(o)
    o = o or {}
    setmetatable(o,self)
    self.__index = self

    local vao_handle
    local material_handle
    
    o.entity = core.entity.create( c.WorldPositionComponent,
                                   c.GraphicsComponent,
                                   c.ScaleComponent,
                                   c.RotationComponent )

    o.mesh = core.contentmanager.load( 
        core.loaders.GnomeLoader, 
        "assets/model/dev/cube.bgnome", 
        function( vao )
            vao_handle = vao
        end, 
        false )

    o.material = core.contentmanager.load( 
        core.loaders.MaterialLoader, 
        "assets/texture/dev/positionPreview.material", 
        function( mat )
            material_handle = mat
        end, 
        false )

    o.entity:set( c.GraphicsComponent, 
    { 
        mesh = vao_handle, 
        material = material_handle, 
        type = core.gfx.objectTypes.DecalGeometry,
        layer = core.gfx.layerTypes.MeshLayer, 
        outlineColor = {10, 10, 10, 10}, 
        render = true
    })

    o.entity:set( c.ScaleComponent, { scale = 1 } )
    o.entity:set( c.RotationComponent, { rotation = { 0, 0, 0, 1 }} )

    return o
end

function Reticule:SetPosition(x, y, z)
    self.entity:set(c.WorldPositionComponent, {position = {x, y, z}}, true)
end

function Reticule:SetScale(scale)
    self.entity:set(c.GrapichsComponent, {outlineColor = {scale, scale, scale, scale}}, true)
end

function Reticule:destroy()
    self.mesh:free()
    self.material:free()
    self.mesh = nil
    self.material = nil
end

return Reticule
