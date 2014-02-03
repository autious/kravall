local GUI = require "gui/GUI"
local Button = require "gui/component/Button"
local Slider = require "gui/component/Slider"
local Checkbox = require "gui/component/Checkbox"
local CenterPlacer = require "gui/placement/CenterPlacer"

ScenarioMenu = {}

function ScenarioMenu:new(o,menuState)
    o = o or {}

    o.gui = GUI:new()

    o.gui:addComponent(Button:new({xoffset=10,yoffset=10,onClick = menuState.goMain }))
    o.gui:addComponent(Checkbox:new())

    o.gui:addPlacementHandler( CenterPlacer:new() )

    setmetatable( o, self )
    self.__index = self
    return o
end

function ScenarioMenu:destroy()
    self.gui:destroy() 
end

return ScenarioMenu
