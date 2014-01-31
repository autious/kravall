local GUI = require "gui/GUI"
local Button = require "gui/component/Button"
local Slider = require "gui/component/Slider"
local Checkbox = require "gui/component/Checkbox"
local CenterPlacer = require "gui/placement/CenterPlacer"

SetupMenu = {}

function SetupMenu:new(o)
    o = o or {}

    o.gui = GUI:new()

    o.gui:addComponent(Button:new({xoffset=10,yoffset=10}))
    o.gui:addComponent(Button:new({xoffset=10,yoffset=10}))

    o.gui:addPlacementHandler( CenterPlacer:new() )

    setmetatable( o, self )
    self.__index = self
    return o
end

function SetupMenu:destroy()
    self.gui:destroy() 
end

return SetupMenu
