local c = core.componentType
local Reticule = {}
function Reticule:new(o)
    o = o or {}
    setmetatable(o,self)
    self.__index = self

    local vao_handle
    
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

    o.acceptMaterial = core.contentmanager.load( 
        core.loaders.MaterialLoader, 
        "assets/texture/dev/smokeAcceptReticule.material", 
        function( mat )
            o.accept_material_handle = mat
        end, 
        false )

    o.denyMaterial = core.contentmanager.load( 
        core.loaders.MaterialLoader, 
        "assets/texture/dev/smokeDenyReticule.material", 
        function( mat )
            o.deny_material_handle = mat
        end, 
        false )
    o.entity:set( c.GraphicsComponent, 
    { 
        mesh = vao_handle, 
        material = o.accept_material_handle, 
        type = core.gfx.objectTypes.DecalGeometry,
        layer = core.gfx.layerTypes.MeshLayer, 
        outlineColor = {15, 1, 15, 1}, 
        render = false
    })

    o.entity:set( c.ScaleComponent, { scale = 1 } )
    o.entity:set( c.RotationComponent, { rotation = { 0, 0, 0, 1 }} )

    return o
end

function Reticule:SetPosition(x, y, z)
    self.entity:set(c.WorldPositionComponent, {position = {x, y, z}}, true)
end

function Reticule:SetScale(scale)
    self.entity:set(c.GrapichsComponent, {outlineColor = {scale, 1, scale, scale}}, true)
end

function Reticule:SetDeny()
    self.entity:set(c.GraphicsComponent, {material = self.deny_material_handle}, true)
end

function Reticule:SetAccept()
    self.entity:set(c.GraphicsComponent, {material = self.accept_material_handle}, true)
end

function Reticule:destroy()
    self.mesh:free()
    self.acceptMaterial:free()
    self.denyMaterial:free()
    self.mesh = nil
    self.material = nil
end

return Reticule
