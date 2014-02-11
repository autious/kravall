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

local KravallControl = {}

function KravallControl:new(o)
    o = o or {}

    print( "KravallControl" )

    setmetatable( o, self )
    self.__index = self

    o.gui = GUI:new()
     
    o.statusGUI = GUI:new( {x=0,y=0, width=200, height=150, anchor="SouthEast"} )

    local labelName = TextLabel:new( {label="Name: Greger"} )
    local labelMorale = TextLabel:new( {label="Morale: Bad"} )
    local labelHealth = TextLabel:new( {label="Health: Good"} )
    local labelStatus = TextLabel:new( {label="Status: Cool"} )

    o.statusGUI:addComponent( labelName   )
    o.statusGUI:addComponent( labelMorale )
    o.statusGUI:addComponent( labelHealth )
    o.statusGUI:addComponent( labelStatus )

    o.statusGUI:addPlacementHandler( WestPlacer )

    o.eventGUI = EventListerGUI:new( {x=0,y=0, width=200, height=200, anchor="SouthWest"} )

    o.eventGUI:addComponent( TextLabel:new( {label="Status: Cool", anchor="SouthEast"} ) )
    o.eventGUI:addComponent( TextLabel:new( {label="Status: Cool", anchor="SouthEast"} ) )
    o.eventGUI:addComponent( TextLabel:new( {label="Status: Cool", anchor="SouthEast"} ) )
    o.eventGUI:addComponent( TextLabel:new( {label="Status: Cool", anchor="SouthEast"} ) )
    o.eventGUI:addPlacementHandler( EventLister )

    o.gui:addComponent( o.statusGUI )
    o.gui:addComponent( o.eventGUI )
    o.gui:addPlacementHandler( AnchorPlacer )

    return o
end

function KravallControl:addEvent( component )

end

function KravallControl:init()
end

function KravallControl:update( delta )
    -- For debug writing
    self.gui:update( delta )
    self.eventGUI:update( delta )
    self.statusGUI:update( delta )

    -- constrict each frame to give animation like results.
    self.eventGUI:constrict( delta )
end

function KravallControl:destroy()
    self.gui:destroy() 
end

return KravallControl
