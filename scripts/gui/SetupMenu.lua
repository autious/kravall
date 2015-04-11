local GUI = require "gui/GUI"

local Button = require "gui/component/Button"
--local Slider = require "gui/component/Slider"
--local Checkbox = require "gui/component/Checkbox"
local TextLabel = require "gui/component/TextLabel"
local TextBox = require "gui/component/TextBox"
local Image = require "gui/component/Image"

local AnchorPlacer = require "gui/placement/AnchorPlacer"

SetupMenu = {}

function SetupMenu:new(o,menuState)
    o = o or {}

    o.gui = GUI:new()
	
	o.wrapperGUI = GUI:new{ width=733, height=559, yoffset=-10, anchor="Center" }
	local hintOffsetX = 30
    local scenarioNameLabel = TextLabel:new({ xoffset=0, yoffset=51, anchor="Center", label="Undefined scenario"})

    if menuState.selectedScenario ~= nil then
        scenarioNameLabel:setLabel( menuState.selectedScenario.name )    
    end

    o.wrapperGUI:addComponent( scenarioNameLabel )
    o.wrapperGUI:addComponent( TextBox:new( { width=o.wrapperGUI.width-hintOffsetX*2, yoffset=-300, height=o.wrapperGUI.height, anchor="Center",
    body=[[Hint: In the deployment screen, you can purchase police squads by selecting a type of squad in the list and placing the squad in one of the multiple spawn points in the scenario. Keep in mind the scenario objectives! 

	
	
Hint: When you think you have all the squads you need, or have no more money left, press the Start-button and the game starts immediately.



Hint: To move a selected squad, right click and drag to set formation or single right click to move in the existing formation.



Hint: The more police in one spot, the quicker nearby rioters get angry and agitated.]]
    } ) )
	o.wrapperGUI:addComponent( Image:new( { mat="assets/texture/ui/hint-background.material", ignoreconstrict=true } ) )
	
	o.wrapperGUI:addPlacementHandler( AnchorPlacer )
	o.gui:addComponent( o.wrapperGUI )
	
    o.gui:addComponent( Button:new( {
										matReleased="assets/texture/ui/go-button-release.material",
										matPressed="assets/texture/ui/go-button-press.material",
										matHover="assets/texture/ui/go-button-hover.material",
										anchor="SouthEast", xoffset=-66, yoffset=-11,
										onClick=function() openscenario( menuState.selectedScenario.filename ) end 
									} ) )

    o.gui:addComponent( Button:new( {
										matReleased="assets/texture/ui/back-button-release.material",
										matPressed="assets/texture/ui/back-button-press.material",
										matHover="assets/texture/ui/back-button-hover.material",
										anchor="SouthWest", xoffset=30, yoffset=-11, onClick=menuState.goScenario 
									} ) )

    o.gui:addPlacementHandler( AnchorPlacer )

    setmetatable( o, self )
    self.__index = self
    return o
end

function SetupMenu:destroy()
    self.gui:destroy() 
end

return SetupMenu
