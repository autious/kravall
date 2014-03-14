local GUI = require "gui/GUI"

local Button = require "gui/component/Button"
--local Slider = require "gui/component/Slider"
--local Checkbox = require "gui/component/Checkbox"
local TextSelectList = require "gui/component/TextSelectList"
--local TextLabel = require "gui/component/TextLabel"
local TextBox = require "gui/component/TextBox"
local Image = require "gui/component/Image"

local AnchorPlacer = require "gui/placement/AnchorPlacer"
local SimplePlacer = require "gui/placement/SimplePlacer"

ScenarioMenu = { scenarios={} }

function ScenarioMenu:new(o,menuState)
    o = o or {}

    setmetatable( o, self )
    self.__index = self

    o.gui = GUI:new()
	
	local textYOffset = 10
	local textXOffset = 20
	
	o.wrapperGUI = GUI:new( { width=1028, height=447, anchor="Center" } )
    local descriptionBox = TextBox:new( { xoffset=441+textXOffset, yoffset=0, body="", width=560, height=500 } )
	
    --local scenarioLabel = TextLabel:new({ xoffset=-50, yoffset=250, anchor="NorthEast", label=""})

    local function onScenarioSelect( object )
        descriptionBox:setText( object.description ) 
		local dimX, dimY = descriptionBox.text:getDim()
		descriptionBox.yoffset = textYOffset - dimY
		
        menuState.selectedScenario = object
        o.wrapperGUI:constrict()

    end

    local selectionList = TextSelectList:new( { 
												width=339, height=426, yoffset=textYOffset, xoffset=textXOffset, elements=o.scenarios, 
												onSelect = onScenarioSelect 
											} )

    o.wrapperGUI:addComponent( selectionList )
    --o.wrapperGUI:addComponent( scenarioLabel )
    o.wrapperGUI:addComponent( descriptionBox )
    
    if menuState.selectedScenario ~=nil then
        selectionList:setActive( menuState.selectedScenario )
    elseif #(o.scenarios) > 0 then
        selectionList:setActive( o.scenarios[1] )
    end
	
	o.wrapperGUI:addComponent( Image:new( { 
										mat="assets/texture/ui/scenario-select-backgrounds.material"
								} ) )
	
    o.wrapperGUI:addPlacementHandler( SimplePlacer )
	o.gui:addComponent( o.wrapperGUI )
	
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
	
	o.gui:addPlacementHandler( AnchorPlacer )

    return o
end

function ScenarioMenu:destroy()
    self.gui:destroy() 
end

return ScenarioMenu
