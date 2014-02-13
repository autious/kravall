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

local EventListerGUI = require "gui/kravall_control/EventListerGUI"
local StanceGUI = require "gui/kravall_control/subgui/StanceGUI"
local AbilityGUI = require "gui/kravall_control/subgui/AbilityGUI" 
local FormationGUI = require "gui/kravall_control/subgui/FormationGUI"

local KravallControl = { onFormationChange = function() core.log.error("No handler set for onFormationChange in KravallControl") end }

function KravallControl:new(o)
    o = o or {}

    print( "KravallControl" )

    setmetatable( o, self )
    self.__index = self

    o.gui = GUI:new()
     
    ------------------
    local labelName = TextLabel:new( {label="Name: Greger"} )
    local labelMorale = TextLabel:new( {label="Morale: Bad"} )
    local labelHealth = TextLabel:new( {label="Health: Good"} )
    local labelStatus = TextLabel:new( {label="Status: Cool"} )

    o.statusGUI = GUI:new( {x=0,y=0, width=200, height=150, anchor="SouthEast"} )
    
    o.statusGUI:addComponent( labelName   )
    o.statusGUI:addComponent( labelMorale )
    o.statusGUI:addComponent( labelHealth )
    o.statusGUI:addComponent( labelStatus )

    o.statusGUI:addPlacementHandler( WestPlacer )
    --------------
    o.eventGUI = EventListerGUI:new( {x=0,y=0, width=200, height=200, anchor="SouthWest"} )
    ----------
    o.stanceGUI = StanceGUI:new()
    o.abilitiesGUI = AbilityGUI:new()
    o.formationGUI = FormationGUI:new( { onFormationChange = function(form) o.onFormationChange(form) end } )

    o.rightControlGUI = GUI:new{x=0,y=0, width=150,height=500, anchor="NorthEast"}
    o.rightControlGUI:addPlacementHandler( AnchorPlacer )

    o.rightControlGUI:addComponent( o.stanceGUI )
    o.rightControlGUI:addComponent( o.abilitiesGUI )
    o.rightControlGUI:addComponent( o.formationGUI )
    -------

    o.gui:addComponent( o.rightControlGUI )
    o.gui:addComponent( o.statusGUI )
    o.gui:addComponent( o.eventGUI )
    o.gui:addPlacementHandler( AnchorPlacer )

    return o
end

function KravallControl:setFormation( formation )
    o.formationGUI:setFormation( formation )
end

function KravallControl:addEvent( component )
    self.eventGUI:addComponent( component )
end

function KravallControl:init()
end

function KravallControl:update( delta )
    -- For debug writing
    self.gui:update( delta )
    self.eventGUI:update( delta )
    self.statusGUI:update( delta )
    self.stanceGUI:update(delta)
    self.abilitiesGUI:update(delta )
    self.formationGUI:update(delta )
    self.rightControlGUI:update(delta )

    self.count = self.count or 5
    self.count = self.count + delta

    if self.count > 1 then
        self.ind = self.ind or 0
        self.ind = self.ind + 1
        
        if self.ind % 2 == 1 then
            self:addEvent(TextLabel:new( {label="Status: " .. self.ind} ))
        else
            self:addEvent(Button:new( {onClick=function() print( "JOEL" ) end }))
        end
        self.count = 0
    end

    -- constrict each frame to give animation like results.
    self.eventGUI:constrict( delta )
end

function KravallControl:destroy()
    self.gui:destroy() 
end

return KravallControl
