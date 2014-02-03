local GUI = require "gui/GUI"
local Button = require "gui/component/Button"
local Slider = require "gui/component/Slider"
local Checkbox = require "gui/component/Checkbox"
local CenterPlacer = require "gui/placement/CenterPlacer"

MainMenu = {}

function MainMenu:new(o,menuState)
    o = o or {}

    o.gui = GUI:new()
    
    o.gui:addComponent(Button:new({xoffset=10,yoffset=10,onClick = function() openscenario( "prototype_area") end}))
    o.gui:addComponent(Button:new({xoffset=10,yoffset=10,onClick = menuState.goScenario }))
    o.gui:addComponent(Button:new({xoffset=10,yoffset=10}))

    o.gui:addPlacementHandler( CenterPlacer:new() )

    setmetatable( o, self )
    self.__index = self
    return o
end

function MainMenu:destroy()
    self.gui:destroy() 
end

return MainMenu
