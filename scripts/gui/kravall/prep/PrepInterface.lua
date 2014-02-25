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

local PrepInterface = 
{
    onSelectCurrentUnit = function() end,
}

function PrepInterface:new(o)
    o = o or {}
    setmetatable(o,self)
    self.__index = self

    o.gui = GUI:new()

    o.totalCash = TextLabel:new{ label="Cash: $4000", xoffset=20,yoffset=-5, anchor="West" }
    o.unitSelection = TextSelectList:new{ xoffset=20,anchor="West"}
    
    o.gui:addComponent( o.totalCash )
    o.gui:addComponent( o.unitSelection )
    o.gui:addPlacementHandler( AnchorPlacer )

    return o
end

function PrepInterface:destroy()
    self.gui:destroy()  
    self.gui = nil
end

return PrepInterface
