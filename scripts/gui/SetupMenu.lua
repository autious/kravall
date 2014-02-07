local GUI = require "gui/GUI"
local Button = require "gui/component/Button"
local Slider = require "gui/component/Slider"
local Checkbox = require "gui/component/Checkbox"
local CenterPlacer = require "gui/placement/CenterPlacer"
local TextLabel = require "gui/component/TextLabel"

SetupMenu = {}

function SetupMenu:new(o,menuState)
    o = o or {}

    o.gui = GUI:new()

    local scenarioNameLabel = TextLabel:new({ xoffset=0, yoffset=0, anchor="NorthEast", label="Undefined scenario"})

    if menuState.selectedScenario ~= nil then
        scenarioNameLabel:setLabel( menuState.selectedScenario.name )    
    end

    o.gui:addComponent(scenarioNameLabel)
    o.gui:addComponent(Button:new({

                                    matReleased = "assets/texture/ui/go-button-release.material",
                                    matPressed = "assets/texture/ui/go-button-press.material",
                                    matHover = "assets/texture/ui/go-button-hover.material",
                                    anchor="SouthEast",xoffset=0,yoffset=0,onClick = function() openscenario( menuState.selectedScenario.filename ) end }))

    o.gui:addPlacementHandler( CenterPlacer:new() )

    setmetatable( o, self )
    self.__index = self
    return o
end

function SetupMenu:destroy()
    self.gui:destroy() 
end

return SetupMenu
