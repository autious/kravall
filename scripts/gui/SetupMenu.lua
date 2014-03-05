local GUI = require "gui/GUI"
local Button = require "gui/component/Button"
local Slider = require "gui/component/Slider"
local Checkbox = require "gui/component/Checkbox"
local AnchorPlacer = require "gui/placement/AnchorPlacer"
local TextLabel = require "gui/component/TextLabel"
local TextBox = require "gui/component/TextBox"

SetupMenu = {}

function SetupMenu:new(o,menuState)
    o = o or {}

    o.gui = GUI:new()

    local scenarioNameLabel = TextLabel:new({ xoffset=0, yoffset=50, anchor="Center", label="Undefined scenario"})

    if menuState.selectedScenario ~= nil then
        scenarioNameLabel:setLabel( menuState.selectedScenario.name )    
    end

    o.gui:addComponent(scenarioNameLabel)
    o.gui:addComponent( TextBox:new({ 
    body=[[Hint: In the deployment screen, you can buy and place police squads in one of the multiple spawn points in the scenario. Keep in mind the scenario objectives! 


Hint: To move a selected squad, right click and drag to set formation or single right click to move in the existing formation.


Hint: When you think you have all the squads you need, or have no more money left, press the Go!-button and the game starts immediately.


Hint: Different points of interest are marked either on the ground or with a coloured lamp in the deployment screen.


Hint: The more police, the quicker rioters get
angry and agitated.]]
    ,width=700, height=600, yoffset=-250, anchor="Center"}))
    o.gui:addComponent(Button:new({

                                    matReleased = "assets/texture/ui/go-button-release.material",
                                    matPressed = "assets/texture/ui/go-button-press.material",
                                    matHover = "assets/texture/ui/go-button-hover.material",
                                    anchor="SouthEast",xoffset=0,yoffset=0,onClick = function() openscenario( menuState.selectedScenario.filename ) end }))

    o.gui:addComponent(Button:new({
                                    matReleased = "assets/texture/ui/back-button-release.material",
                                    matPressed = "assets/texture/ui/back-button-press.material",
                                    matHover = "assets/texture/ui/back-button-hover.material",
                                    anchor="SouthWest",xoffset=0,yoffset=0,onClick = menuState.goScenario }))

    o.gui:addPlacementHandler( AnchorPlacer )

    setmetatable( o, self )
    self.__index = self
    return o
end

function SetupMenu:destroy()
    self.gui:destroy() 
end

return SetupMenu
