local GUI = require "gui/GUI"
local Statistics = require "statistics/Statistics"

local TextLabel = require "gui/component/TextLabel"
local TextBox = require "gui/component/TextBox"
local Image = require "gui/component/Image"
local Button = require "gui/component/Button"
local Objective = require "gui/component/objective_handler/Objective"

local AnchorPlacer = require "gui/placement/AnchorPlacer"
local CenterPlacer = require "gui/placement/CenterPlacer"
local SimplePlacer = require "gui/placement/SimplePlacer"

local Util = require "gui/placement/util"

local EndScreen = 
					{
						anchor="Center",
						won=false,
						objectives={}
					}

function EndScreen:new(o)
    o = o or {}
    setmetatable(o,self)
    self.__index = self

	local offsetX = 15
	local offsetY = 0
	
	Statistics.prepare()
	
	o.gui = GUI:new( { } )
	
	o.screenGUI = GUI:new( { width=623, height=689, anchor="Center" } )
	o.inBoxGUI = GUI:new( { width=563, height=563, xoffset=30, yoffset=45, anchor="North" } )
	
	o.inBoxGUI:addComponent( TextLabel:new( { label="Rank: " .. Statistics.rank, xoffset=offsetX, yoffset=0 } ) )
	offsetY = offsetY + Statistics.newLineY 
	
	--offsetY = offsetY + Statistics.newLineY
	
	Statistics.guiWidth = o.inBoxGUI.width - offsetX * 2
	
	--local objGUI = Statistics.getObjectivesAsSubGUI( offsetX, offsetY )
	--o.inBoxGUI:addComponent( objGUI )
	--
	--offsetY = offsetY + objGUI.height + Statistics.newLineY
	--
	--local subGUI = Statistics.getCategoryAsSubGUI( "Units", offsetX, offsetY )
	--o.inBoxGUI:addComponent( subGUI )
	--
	--offsetY = offsetY + subGUI.height + Statistics.newLineY
	--
	--subGUI = Statistics.getCategoryAsSubGUI( "Other", offsetX, offsetY )
	--o.inBoxGUI:addComponent( subGUI )
	--
	--offsetY = offsetY + subGUI.height + Statistics.newLineY
	
	print( "offsetY: " .. offsetY )
	local subGUI = Statistics.getAllAsSubGUI( offsetX, offsetY )
	o.inBoxGUI:addComponent( subGUI )
	
	offsetY = offsetY + subGUI.height
	print( "All subGUI height: " .. subGUI.height .. ", new offsetY: " .. offsetY )
	
	if o.won then
		o.inBoxGUI:addComponent( TextLabel:new( { label="Game is over, you won! :D", xoffset=offsetX, yoffset=offsetY } ) )
	else
		o.inBoxGUI:addComponent( TextLabel:new( { label="Game is over, you lost. Awwww :(", xoffset=offsetX, yoffset=offsetY } ) )
	end
	
	-- GUI background, buttons and title label
	o.inBoxGUI:addPlacementHandler( SimplePlacer )
	
	o.screenGUI:addComponent( o.inBoxGUI )
	
	local label = TextLabel:new( { label="GAME SUMMARY", yoffset=18 } )
	local dimX, dimY = label.text:getDim()
	label.xoffset = ( o.screenGUI.width - dimX ) * 0.5
	o.screenGUI:addComponent( label )
	
	o.screenGUI:addComponent( Button:new (
										{ 
											xoffset=20,
											yoffset=429,
											matReleased="assets/texture/ui/restart-button-release.material",
											matPressed="assets/texture/ui/restart-button-press.material",
											matHover="assets/texture/ui/restart-button-hover.material",
											onClick=reopenScenario
										} ) )	
	o.screenGUI:addComponent( Button:new (
										{ 
											xoffset=260,
											yoffset=429,
											matReleased="assets/texture/ui/menu-button-release.material",
											matPressed="assets/texture/ui/menu-button-press.material",
											matHover="assets/texture/ui/menu-button-hover.material",
											onClick=function() openscenario( "main_menu" ) end
										} ) )	
	o.screenGUI:addComponent( Image:new( { mat="assets/texture/ui/end-game-screen.material" } ) )
	
	o.screenGUI:addPlacementHandler( SimplePlacer )
	o.gui:addComponent( o.screenGUI )
	o.gui:addPlacementHandler( AnchorPlacer )

    return o
end

function EndScreen:destroy()
    self.gui:destroy()  
    self.gui = nil
end

return EndScreen
