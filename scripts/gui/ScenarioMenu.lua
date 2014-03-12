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

    setmetatable( o, self )
    self.__index = self

    o.gui = GUI:new()
    
    local descriptionBox = TextBox:new( {anchor="NorthEast", yoffset=200, body="", width=600, height=500} )
	local textYOffset = 220
    --local scenarioLabel = TextLabel:new({ xoffset=-50, yoffset=250, anchor="NorthEast", label=""})

    local function onScenarioSelect( object )
        descriptionBox:setText( object.description ) 
		local dimX, dimY = descriptionBox.text:getDim()
		descriptionBox.yoffset = textYOffset - dimY
		
        menuState.selectedScenario = object
        o.gui:constrict()

    end

    local selectionList = TextSelectList:new( { 
												width=330, height=200, yoffset=textYOffset, xoffset=250, elements=o.scenarios, 
												anchor="NorthWest", onSelect = onScenarioSelect 
											} )

    o.gui:addComponent(Button:new({
                                    matReleased="assets/texture/ui/back-button-release.material",
                                    matPressed="assets/texture/ui/back-button-press.material",
                                    matHover="assets/texture/ui/back-button-hover.material",
                                    anchor="SouthWest", xoffset=30, yoffset=-11, onClick=menuState.goMain }))
    o.gui:addComponent(Button:new({

                                    matReleased="assets/texture/ui/tutorial/next-release.material",
                                    matPressed="assets/texture/ui/tutorial/next-press.material",
                                    matHover="assets/texture/ui/tutorial/next-hover.material",
                                    anchor="SouthEast", xoffset=-66, yoffset=-11, onClick=menuState.goSetup }))
    o.gui:addComponent( selectionList )
    --o.gui:addComponent( scenarioLabel )
    o.gui:addComponent( descriptionBox )
    
    if menuState.selectedScenario ~=nil then
        selectionList:setActive( menuState.selectedScenario )
    elseif #(o.scenarios) > 0 then
        selectionList:setActive( o.scenarios[1] )
    end

    o.gui:addPlacementHandler( AnchorPlacer )

    return o
end

function ScenarioMenu:destroy()
    self.gui:destroy() 
end

return ScenarioMenu
