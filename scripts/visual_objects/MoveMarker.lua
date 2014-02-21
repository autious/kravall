
local c = core.componentType
local MoveMarker = {}
function MoveMarker:new(o)
    o = o or {}
    setmetatable(o,self)
    self.__index = self

    o.markerList = {}

    o.acceptDestMeshAsset = core.contentmanager.load( 
        core.loaders.GnomeLoader, 
        "assets/model/animated/marker/dest-accept_00.bgnome", 
        function( vao_handle )
            o.acceptDestMeshVao = vao_handle
        end, 
        false )
    o.denyDestMeshAsset = core.contentmanager.load( 
        core.loaders.GnomeLoader, 
        "assets/model/animated/marker/dest-deny_00.bgnome", 
        function( vao_handle )
            o.denyDestMeshVao = vao_handle
        end, 
        false )

    o.acceptDestMaterialAsset = core.contentmanager.load( 
        core.loaders.MaterialLoader, 
        "assets/texture/animated/marker/dest-accept_00.material", 
        function( mat )
            o.acceptDestMaterialHandle = mat
        end, 
        false )
    o.denyDestMaterialAsset = core.contentmanager.load( 
        core.loaders.MaterialLoader, 
        "assets/texture/animated/marker/dest-deny_00.material", 
        function( mat )
            o.denyDestMaterialHandle = mat
        end, 
        false )

    return o
end

function MoveMarker:update( delta )
    --TODO:REMOVE MARKERS WHEN ANIMATION FINISHED PLAYING.    
end

local function createInstance(self,position, mesh, mat )
    position = position + core.glm.vec3.new(0,1,0)
    entity = core.entity.create( c.WorldPositionComponent,
                                   c.GraphicsComponent,
                                   c.AnimationComponent,
                                   c.ScaleComponent,
                                   c.RotationComponent )
    entity:set( c.WorldPositionComponent, { position = { 0, 0, 0 } } )
    entity:set( c.GraphicsComponent, 
    { 
        mesh = 0, 
        material = 0, 
        type = core.gfx.objectTypes.OpaqueGeometry,
        layer = core.gfx.layerTypes.MeshLayer, 
        outlineColor = {0, 0, 1, 1}, 
        render = false
    })
    entity:set( c.AnimationComponent,
    {
            animationID = 0,
            queuedAnimationID = 0,
            currentTime = 0.0,
            loop = false,
            playing = false,
            speed = 1.0,
            currentFrame = 0
    })
    entity:set( c.ScaleComponent, { scale = 1 } )
    entity:set( c.RotationComponent, { rotation = { 0, 0, 0, 1 }} )

    entity:set( c.WorldPositionComponent, { position = { position:get() } } )
    entity:set( c.GraphicsComponent, {render = true, mesh = mesh, material = mat }, true )
    core.animations.play( entity, "default" )

    
    self.markerList[#(self.markerList)+1] = entity
end

function MoveMarker:playAccept( position )
    createInstance( self, position, self.acceptDestMeshVao, self.acceptDestMaterialHandle)
end

function MoveMarker:playDeny( position )
    createInstance( self, position, self.denyDestMeshVao, self.denyDestMaterialHandle)
end

function MoveMarker:destroy()
    for _,v in pairs( self.markerList ) do
        v:destroy()
    end
    self.markerList = nil

    self.acceptDestMeshAsset:free()
    self.acceptDestMeshAsset = nil
    self.denyDestMeshAsset:free()
    self.denyDestMeshAsset = nil
    self.denyDestMaterialAsset:free()
    self.denyDestMaterialAsset = nil
    self.acceptDestMaterialAsset:free()
    self.acceptDestMaterialAsset = nil
end

return MoveMarker
