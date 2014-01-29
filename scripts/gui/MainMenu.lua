local GUI = require "gui/GUI"
local Button = require "gui/component/Button"
local Slider = require "gui/component/Slider"
local Checkbox = require "gui/component/Checkbox"

MainMenu = {}

function MainMenu:new(o)
    o = o or {}

    o.gui = GUI:new()
    o.gui:addComponent(Button:new({x=100,y=100}))
    o.gui:addComponent(Slider:new({x=100,y=300}))
    o.gui:addComponent(Checkbox:new({x=100,y=400}))

    setmetatable( o, self )
    self.__index = self
    return o
end

function MainMenu:destroy()
    self.gui:destroy() 
end

return MainMenu
