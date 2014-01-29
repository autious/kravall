local GUI = require "gui/GUI"
local Button = require "gui/component/Button"

MainMenu = {}

function MainMenu:new(o)
    o = o or {}

    o.gui = GUI:new()
    o.gui:addComponent(Button:new({x=100,y=100}))

    setmetatable( o, self )
    self.__index = self
    return o
end

function MainMenu:destroy()
    
end

return MainMenu
