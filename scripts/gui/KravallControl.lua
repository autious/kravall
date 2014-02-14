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

local EventListerGUI = require "gui/kravall_control/EventListerGUI"
local StanceGUI = require "gui/kravall_control/subgui/StanceGUI"
local AbilityGUI = require "gui/kravall_control/subgui/AbilityGUI" 
local FormationGUI = require "gui/kravall_control/subgui/FormationGUI"
local UnitStatGUI = require "gui/kravall_control/subgui/UnitStatGUI"

local KravallControl = { onFormationSelect = function() core.log.error("No handler set for onFormationChange in KravallControl") end }

function KravallControl:new(o)
    o = o or {}

    setmetatable( o, self )
    self.__index = self

    o.gui = GUI:new()
     
    ------------------
    o.statusGUI = UnitStatGUI:new( {} )
    --------------
    o.eventGUIPadder = GUI:new( {width=220,height=220, anchor="SouthWest"} )
    o.eventGUIPadder:addPlacementHandler( CenterPlacer )
    o.eventGUI = EventListerGUI:new( {x=0,y=0, width=200, height=200, anchor="SouthWest"} )
    o.eventGUIPadder:addComponent( o.eventGUI )
    ----------
    o.stanceGUI = StanceGUI:new()
    o.abilitiesGUI = AbilityGUI:new()
    o.formationGUI = FormationGUI:new( { onFormationSelect = function(form) o.onFormationSelect(form) end } )

    o.rightControlGUI = GUI:new{x=0,y=0, width=150,height=500, anchor="NorthEast"}
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
