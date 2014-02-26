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
    onSelectCurrentSquad = function(squadDef) end,
    onFinished = function() end,
    policeTypes = {}
}

function PrepInterface:new(o)
    o = o or {}
    setmetatable(o,self)
    self.__index = self

    o.gui = GUI:new()


    o.title = TextLabel:new{ label="\\/ Purchase menu \\/", xoffset=20,yoffset=-5, anchor="West" }
    o.totalCash = TextLabel:new{ label="Cash: $4000", xoffset=20,yoffset=-5, anchor="West" }
    o.unitSelection = TextSelectList:new
    { 
        xoffset=20,
        anchor="West", 
        elements=o.policeTypes,
        onSelect = o.onSelectCurrentSquad,
    }

    o.doneButton = Button:new{ anchor="SouthEast", xoffset=-10, yoffset=-10, onClick = o.onFinished }
    
    o.gui:addComponent( o.title )
    o.gui:addComponent( o.doneButton) 
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
