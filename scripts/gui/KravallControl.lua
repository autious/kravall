local GUI = require "gui/GUI"
local Button = require "gui/component/Button"
local Slider = require "gui/component/Slider"
local Checkbox = require "gui/component/Checkbox"
local TextSelectList = require "gui/component/TextSelectList"
local TextLabel = require "gui/component/TextLabel"
local TextBox = require "gui/component/TextBox"
local AnchorPlacer = require "gui/placement/AnchorPlacer"

local EventLister = require "gui/kravall_control/EventLister"

local KravallControl = {}

function KravallControl:new(o)
    o = o or {}

    print( "KravallControl" )

    setmetatable( o, self )
    self.__index = self

    o.gui = GUI:new()
    
    o.eventLister = EventLister:new({anchor = "NorthEast"})
     
    o.gui:addComponent( o.eventLister )
    local statusGUI = GUI:new( {x=0,y=0, width=200, height=200, anchor="SouthEast"} )
    statusGUI:addComponent( o.eventLister )
    statusGUI:addPlacementHandler( AnchorPlacer:new() )

    local labelName = TextLabel:new( {label="Name: Greger", anchor="SouthEast"} )
    local labelMorale = TextLabel:new( {label="Morale: Bad", anchor="SouthEast"} )
    local labelHealth = TextLabel:new( {label="Health: Good", anchor="SouthEast"} )
    local labelStatus = TextLabel:new( {label="Status: Cool", anchor="SouthEast"} )

    o.gui:addComponent( labelName   )
    o.gui:addComponent( labelMorale )
    o.gui:addComponent( labelHealth )
    o.gui:addComponent( labelStatus )
    o.gui:addComponent( statusGUI )

    o.gui:addPlacementHandler( AnchorPlacer:new() )

    return o
end

function KravallControl:init()
end

function KravallControl:update( delta )
    self.eventLister:update( delta )
end

function KravallControl:destroy()
    self.gui:destroy() 
end

return KravallControl
