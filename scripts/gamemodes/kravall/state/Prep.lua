local PrepInterface = require "gui/kravall/prep/PrepInterface"

local SquadPositionDecal = require "visual_objects/SquadPositionDecal"

local Prep = { 
    name = "Prep",
    policeTypes = nil,
    onFinished = function() end
}

local input = require "input"

function Prep:new(o)
    o = o or {}
    setmetatable( o, self )
    self.__index = self
  
    o.squadPositionDecal = SquadPositionDecal:new()

    o.spawnAreas = {}
    o.createdSquads = {}

    o.prepInterface = PrepInterface:new(
    {
        policeTypes = o.policeTypes,
        createdSquads = o.createdSquads,
        onFinished = function()
            o.onFinished()
        end,
        onSelectCurrentSquad = function(squadDef)
            o.activeSquad = squadDef
            o.squadPositionDecal:setActiveSquad(squadDef)
        end
    }
    )

    o.onButton = function( button, action, mods, consumed )
        if button == core.input.mouse.button.Left 
           and action == core.input.action.Press then 
            if o.activeSquad then
                if o.canPlace then 
                    --pLace unit
                    print( "CREATE LIFE" )
                    table.insert( o.createdSquads, o.activeSquad )
                    o.prepInterface:updatePurchasedList()
                end
            end 
        end
    end

    input.registerOnButton( o.onButton, "GAME" )

    o.canPlace = false

    o.onPosition = function( x, y )
        o.squadPositionDecal:setPosition( core.system.picking.getGroundHit( x,y ))
        o.canPlace = o.squadPositionDecal:verifyPlacement( o.spawnAreas )
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
