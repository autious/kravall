local GUI = require "gui/GUI"
local Button = require "gui/component/Button"
local Slider = require "gui/component/Slider"
local Checkbox = require "gui/component/Checkbox"
local TextSelectList = require "gui/component/TextSelectList"
local TextLabel = require "gui/component/TextLabel"
local TextBox = require "gui/component/TextBox"

local AnchorPlacer = require "gui/placement/AnchorPlacer"
local EastPlacer = require "gui/placement/EastPlacer"
local NorthPlacer = require "gui/placement/NorthPlacer"
local WestPlacer = require "gui/placement/WestPlacer"
local CenterPlacer = require "gui/placement/CenterPlacer"

local EventListerGUI = require "gui/kravall/main/EventListerGUI"
local StanceGUI = require "gui/kravall/main/subgui/StanceGUI"
local AbilityGUI = require "gui/kravall/main/subgui/AbilityGUI" 
local FormationGUI = require "gui/kravall/main/subgui/FormationGUI"
local UnitStatGUI = require "gui/kravall/main/subgui/UnitStatGUI"

local KravallControl = 
{ 
    onFormationSelect = function(formation) core.log.error("No handler set for onFormationChange in KravallControl") end,
    onStanceSelect = function(stance) core.log.error("No handler set for onStanceChange in KravallControl") end,
    onAbilitySelect = function(ability) end
}
                            
function KravallControl:new(o)
    o = o or {}

    setmetatable( o, self )
    self.__index = self

    o.gui = GUI:new()
     
    ------------------
    o.statusGUI = UnitStatGUI:new( { show = false } )
    --------------
    o.eventGUIPadder = GUI:new( {width=220,height=220, anchor="SouthWest"} )
    o.eventGUIPadder:addPlacementHandler( CenterPlacer )
    o.eventGUI = EventListerGUI:new( {x=0,y=0, width=200, height=200, anchor="SouthWest"} )
    o.eventGUIPadder:addComponent( o.eventGUI )
    ----------
    o.stanceGUI = StanceGUI:new( {
        onStanceSelect = function(stance) o.onStanceSelect( stance ) end 
    })
    o.abilitiesGUI = AbilityGUI:new( {
        onAbilitySelect = function(ability) o.onAbilitySelect( ability ) end 
    })
    o.formationGUI = FormationGUI:new( { 
        onFormationSelect = function(form) o.onFormationSelect(form) end 
    })

    o.rightControlGUI = GUI:new{x=0,y=0, width=150,height=600, anchor="NorthEast"}
    o.rightControlGUI:addPlacementHandler( AnchorPlacer )

    o.rightControlGUI:addComponent( o.stanceGUI )
    o.rightControlGUI:addComponent( o.abilitiesGUI )
    o.rightControlGUI:addComponent( o.formationGUI )
    -------

    o.gui:addComponent( o.rightControlGUI )
    o.gui:addComponent( o.statusGUI )
    o.gui:addComponent( o.eventGUIPadder )
    o.gui:addPlacementHandler( AnchorPlacer )

    return o
end

function KravallControl:setFormation( formation )
    self.formationGUI:setFormation( formation )
end

function KravallControl:setStance( stance )
    self.stanceGUI:setStance( stance )
end

function KravallControl:setAbility( ability )
    self.abilitiesGUI:setAbility( ability ) 
end

function KravallControl:setUnitInformation( data )
    self.statusGUI:setFormation( data.formation )
    self.statusGUI:setStance( data.stance )
    self.statusGUI:setName( data.name )
    self.statusGUI:setHealth( data.health )
    self.statusGUI:setMorale( data.morale )
    self.statusGUI:setStamina( data.stamina )
end

function KravallControl:setSelectedSquads( squads )
    if squads and #squads > 0 then
        self.statusGUI:setShow( true )
    else
        self.statusGUI:setShow( false )
    end
    --TODO: show/hide the status field depending on selection.
end

function KravallControl:setUsableAbilities(abilities)
    self.abilitiesGUI:setUsableAbilities(abilities)
end



function KravallControl:addEvent( component )
    self.eventGUI:addComponent( component )
end

function KravallControl:init()
end

function KravallControl:update( delta )
    -- For debug writing
    self.gui:renderDebug( delta )

    --self.count = self.count or 0
    --self.count = self.count + delta

    --if self.count > 10 then
    --    self.ind = self.ind or 0
    --    self.ind = self.ind + 1
    --    
    --    if self.ind % 2 == 1 then
    --        self:addEvent(TextLabel:new( {label="Status: " .. self.ind} ))
    --    else
    --        self:addEvent(Button:new( {onClick=function() print( "JOEL" ) end }))
    --    end
    --    self.count = 0
    --end

    -- constrict each frame to give animation like results.
    self.eventGUI:constrict( delta )
end

function KravallControl:destroy()
    self.gui:destroy() 
end

return KravallControl