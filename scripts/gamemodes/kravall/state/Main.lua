local MoveMarker = require "visual_objects/MoveMarker"
local PoliceSquadHandler = require "gamemodes/kravall/PoliceSquadHandler"
local KravallControl = require "gui/KravallControl"

local Main = { name = "Main" }
function Main:new(o)
    o = o or {}
    setmetatable( o, self )
    self.__index = self

    --Init the things that are required.
    o.moveMarker = MoveMarker:new()

    o.gui = KravallControl:new( 
    {
        -- Called when the user is changing the formation from the gui.
        onFormationSelect = function( formation )
            o.policeHandler:setFormation( formation )
        end,
        -- Called when the user is changing stance from the gui.
        onStanceSelect = function( stance )
            o.policeHandler:setStance( stance )
        end,

        -- Called when the user is changing the current active ability in the gui.
        onAbilitySelect = function( ability )
            o.policeHandler:setAbility( ability )
        end
    })

    o.policeHandler = PoliceSquadHandler:new( 
    {
        -- Called when the currently active formation is changed logically.
        onFormationChange = function( formation )
            o.gui:setFormation( formation ) 
        end,
        -- Called when the currently active stance is changed logically.
        onStanceChange = function( stance )
            o.gui:setStance( stance )
        end,

        onAbilityChange = function( ability )
            o.gui:setAbility( ability )
        end,

        -- Called whenever the selection changes
        -- might contain previously sent units.
        -- Could be empty
        onSelectedSquadsChange = function( squads )
            o.gui:setSelectedSquads( squads )
        end,

        -- Called when the currently active unit changes
        -- or when the state of the unit might have changes (like health)
        onSelectedUnitInformationChange = function( data )
            o.gui:setUnitInformation( data ) 
        end,
        onMoveToPosition = function( squads, position, accept )
            if accept then
                o.moveMarker:playAccept(  position )
            else
                o.moveMarker:playDeny( position )
            end
        end,
    })	

    return o
end

function Main:update(delta)
    self.gui:update(delta)
    self.policeHandler:update(delta)
    self.moveMarker:update( delta )
end

function Main:destroy()
    self.moveMarker:destroy()
    self.gui:destroy()
    self.policeHandler:destroy()
end

return Main
