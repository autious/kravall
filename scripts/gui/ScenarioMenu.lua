local GUI = require "gui/GUI"
local Button = require "gui/component/Button"
local Slider = require "gui/component/Slider"
local Checkbox = require "gui/component/Checkbox"
local TextSelectList = require "gui/component/TextSelectList"
local TextLabel = require "gui/component/TextLabel"
local TextBox = require "gui/component/TextBox"
local AnchorPlacer = require "gui/placement/AnchorPlacer"

ScenarioMenu = {}

function ScenarioMenu:new(o,menuState)
    o = o or {}

    o.gui = GUI:new()

    o.gui:addComponent(Button:new({anchor="SouthWest",xoffset=0,yoffset=0,onClick = menuState.goMain }))
    o.gui:addComponent(Button:new({anchor="SouthEast",xoffset=0,yoffset=0,onClick = menuState.goMain }))
    o.gui:addComponent(TextSelectList:new({ anchor="West", xoffset=300}))
    o.gui:addComponent(TextLabel:new({ xoffset=-50, yoffset=250, anchor="NorthEast", label="Scenario Title"}))
    o.gui:addComponent(TextBox:new({anchor="East", body="This is a scenario description"}))

    o.gui:addPlacementHandler( AnchorPlacer:new() )

    setmetatable( o, self )
    self.__index = self
    return o
end

function ScenarioMenu:destroy()
    self.gui:destroy() 
end

return ScenarioMenu
