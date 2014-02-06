local GUI = require "gui/GUI"
local Button = require "gui/component/Button"
local Slider = require "gui/component/Slider"
local Checkbox = require "gui/component/Checkbox"
local TextSelectList = require "gui/component/TextSelectList"
local TextLabel = require "gui/component/TextLabel"
local TextBox = require "gui/component/TextBox"
local AnchorPlacer = require "gui/placement/AnchorPlacer"

ScenarioMenu = { scenarios={} }

function ScenarioMenu:new(o,menuState)
    o = o or {}

    o.gui = GUI:new()

    local descriptionBox = TextBox:new({anchor="East", body="", width = 100, height = 100})
    local scenarioLabel = TextLabel:new({ xoffset=-50, yoffset=250, anchor="NorthEast", label=""})

    local function onScenarioSelect( object )
        descriptionBox:setText( "LOL" )         
        scenarioLabel:setLabel( object.name )
        o.gui:constrict()
    end

    o.gui:addComponent(Button:new({
                                    matReleased = "assets/texture/ui/back-button-release.material",
                                    matPressed = "assets/texture/ui/back-button-press.material",
                                    matHover = "assets/texture/ui/back-button-hover.material",
                                    anchor="SouthWest",xoffset=0,yoffset=0,onClick = menuState.goMain }))
    o.gui:addComponent(Button:new({

                                    matReleased = "assets/texture/ui/go-button-release.material",
                                    matPressed = "assets/texture/ui/go-button-press.material",
                                    matHover = "assets/texture/ui/go-button-hover.material",
                                    anchor="SouthEast",xoffset=0,yoffset=0,onClick = menuState.goMain }))
    o.gui:addComponent(TextSelectList:new({ anchor="West", xoffset=300, elements=o.scenarios, onSelect = onScenarioSelect }))
    o.gui:addComponent( scenarioLabel )
    o.gui:addComponent( descriptionBox )

    o.gui:addPlacementHandler( AnchorPlacer:new() )

    setmetatable( o, self )
    self.__index = self
    return o
end

function ScenarioMenu:destroy()
    self.gui:destroy() 
end

return ScenarioMenu
