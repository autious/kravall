local PrepInterface = require "gui/kravall/prep/PrepInterface"

local SquadPositionDecal = require "visual_objects/SquadPositionDecal"

local Prep = { name = "Prep" }

local input = require "input"

function Prep:new(o)
    o = o or {}
    setmetatable( o, self )
    self.__index = self
  
    o.squadPositionDecal = SquadPositionDecal:new()
    o.squadPositionDecal:setFormationOffsets( 
    {
        core.glm.vec3.new(1,0,0),
        core.glm.vec3.new(-1,0,0),
        core.glm.vec3.new(0,0,1),
        core.glm.vec3.new(0,0,-1),
    })

    o.spawnAreas = {}

    o.prepInterface = PrepInterface:new()

    o.onButton = function( button, action, mods, consumed )
         
    end

    input.registerOnButton( o.onButton, "GAME" )

    o.onPosition = function( x, y )
        o.squadPositionDecal:setPosition( core.system.picking.getGroundHit( x,y ))
        o.squadPositionDecal:verifyPlacement( o.spawnAreas )
    end 

    input.registerOnPosition( o.onPosition )

    return o
end

-- Add a spawn zone, designated by an entity with an AreaComponent
function Prep:addAreaSpawnZone( ent )
    table.insert( self.spawnAreas, ent )
end

function Prep:update( delta )

end

function Prep:destroy()
    self.squadPositionDecal:destroy()
    input.deregisterOnButton( self.onButton )
    input.deregisterOnPosition( self.onPosition )
    self.prepInterface:destroy()
end

return Prep
