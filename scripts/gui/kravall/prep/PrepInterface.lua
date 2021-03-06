local GUI = require "gui/GUI"

local Button = require "gui/component/Button"
local Slider = require "gui/component/Slider"
local Checkbox = require "gui/component/Checkbox"
local TextSelectList = require "gui/component/TextSelectList"
local TextLabel = require "gui/component/TextLabel"
local TextBox = require "gui/component/TextBox"
local Image = require "gui/component/Image"
local MsgBox = require "gui/component/MessageBox"

local AnchorPlacer = require "gui/placement/AnchorPlacer"
local EastPlacer = require "gui/placement/EastPlacer"
local NorthPlacer = require "gui/placement/NorthPlacer"
local WestPlacer = require "gui/placement/WestPlacer"
local CenterPlacer = require "gui/placement/CenterPlacer"
local SimplePlacer = require "gui/placement/SimplePlacer"

local standardPolice = (require "game_constants").standardPolice
local tearGasPolice = (require "game_constants").tearGasPolice

local Util = require "gui/placement/util"

local PrepInterface = 
{
    policeTypes = {},
    onSelectCurrentSquad = function( squadDef ) end,
    onFinished = function() end,
    onSelectCreatedSquad = function ( squadInstance ) end,
    onRemoveSelected = function() end
}

function PrepInterface:new(o)
    o = o or {}
    setmetatable(o,self)
    self.__index = self

	local leftXOffset = 15
	
	o.msgBox = MsgBox:new( { 
								buttons="YesNo",
								title="Do you want to continue?",
								message=[[You still have money left in your account.
								
								Are you sure you want to continue?]],
								onPrimaryClick=o.onFinished, onSecondaryClick=function() end
							} )
	o.msgBox:setShow( false )
	
	o.cashLeft = 0;
	
    o.gui = GUI:new()
	o.gui:addComponent( TextLabel:new( { label="Select units to purchase and assign them to a spawning position", anchor="North" } ) )
	
	-- Cash Sub-GUI
	o.cashSubGUI = GUI:new { width=240, height=30, anchor="NorthWest" }
	
	o.cashSubGUI:addComponent( TextLabel:new( { label="Cash:", anchor="NorthWest", xoffset=leftXOffset, yoffset=1 } ) )
	o.cashLabel = TextLabel:new( { label="$" .. o.cashLimit, anchor="NorthEast", xoffset=-leftXOffset-2, yoffset=1 } )
	o.cashSubGUI:addComponent( o.cashLabel )
	o.cashSubGUI:addComponent( Image:new( { mat="assets/texture/ui/aa-temp_prep_cashPanel.material", ignoreConstrict=true, anchor="NorthWest" } ) )
	
	o.cashSubGUI:addPlacementHandler( AnchorPlacer )
	o.gui:addComponent( o.cashSubGUI )
	
	--Shield Unit Stats Sub-GUI
	o.shieldStatsSubGUI = GUI:new{ width=210, height=118, xoffset=leftXOffset-5, yoffset=196 }
	
	o.shieldStatsSubGUI:addComponent( Image:new( { mat="assets/texture/ui/aa-temp_prep_shieldPolice.material" } ) )
	o.shieldStatsSubGUI:addComponent( TextLabel:new( { label="Cost: $" .. standardPolice.cost, font="assets/font/toolTip.font", 
														xoffset=110, yoffset=-9 } ) )
	o.shieldStatsSubGUI:addComponent( TextLabel:new( { label="Health: " .. standardPolice.maxHealth, font="assets/font/toolTip.font", 
														xoffset=110, yoffset=9} ) )
	o.shieldStatsSubGUI:addComponent( TextLabel:new( { label="Stamina: " .. standardPolice.maxStamina, font="assets/font/toolTip.font", 
														xoffset=110, yoffset=27  } ) )
	o.shieldStatsSubGUI:addComponent( TextLabel:new( { label="Morale: " .. standardPolice.maxMorale, font="assets/font/toolTip.font", 
														xoffset=110, yoffset=45 } ) )
	o.shieldStatsSubGUI:addComponent( TextLabel:new( { label="Abilities: ", font="assets/font/toolTip.font", 
														xoffset=110, yoffset=63 } ) )
	o.shieldStatsSubGUI:addComponent( TextLabel:new( { label="None", font="assets/font/toolTip.font", 
														xoffset=110+leftXOffset, yoffset=81 } ) )
	
	o.shieldStatsSubGUI:addPlacementHandler( SimplePlacer )
	
	--Teargas Unit Stats Sub-GUI
	o.tgStatsSubGUI = GUI:new{ width=210, height=118, xoffset=leftXOffset-5, yoffset=196 }
	
	o.tgStatsSubGUI:addComponent( Image:new( { mat="assets/texture/ui/aa-temp_prep_tearGasPolice.material" } ) )
	o.tgStatsSubGUI:addComponent( TextLabel:new( { label="Cost: $" .. tearGasPolice.cost, font="assets/font/toolTip.font", 
													xoffset=110, yoffset=-9 } ) )
	o.tgStatsSubGUI:addComponent( TextLabel:new( { label="Health: " .. tearGasPolice.maxHealth, font="assets/font/toolTip.font", 
													xoffset=110, yoffset=9 } ) )
	o.tgStatsSubGUI:addComponent( TextLabel:new( { label="Stamina: " .. tearGasPolice.maxStamina, font="assets/font/toolTip.font", 
													xoffset=110, yoffset=27  } ) )
	o.tgStatsSubGUI:addComponent( TextLabel:new( { label="Morale: " .. tearGasPolice.maxMorale, font="assets/font/toolTip.font", 
													xoffset=110, yoffset=45 } ) )
	o.tgStatsSubGUI:addComponent( TextLabel:new( { label="Abilities: ", font="assets/font/toolTip.font", 
													xoffset=110, yoffset=63 } ) )
	o.tgStatsSubGUI:addComponent( TextLabel:new( { label="TearGas", font="assets/font/toolTip.font", 
													xoffset=110+leftXOffset, yoffset=81 } ) )
	
	o.tgStatsSubGUI:addPlacementHandler( SimplePlacer )

	-- Units Sub-GUI
	o.unitsSubGUI = GUI:new{ width=240, height=330, yoffset=5 }
	
	o.unitsSubGUI:addComponent( o.shieldStatsSubGUI )
	o.unitsSubGUI:addComponent( o.tgStatsSubGUI )
	
	o.unitsSubGUI:addComponent( TextLabel:new( { label="Available units", xoffset=leftXOffset, yoffset=9 } ) )
	o.unitSelection = TextSelectList:new(
										{
											width=217,
											height=135,
											xoffset=leftXOffset,
											yoffset=42,
											elements=o.policeTypes,
											onSelect = function( squadDef )
												if squadDef then
													if squadDef.name == "Shield Squad" then
														o.shieldStatsSubGUI:setShow( true )
													else
														o.shieldStatsSubGUI:setShow( false )
													end
													
													if squadDef.name == "Teargas Squad" then
														o.tgStatsSubGUI:setShow( true )
													else
														o.tgStatsSubGUI:setShow( false )
													end
												end
												
												o.onSelectCurrentSquad( squadDef )
											end
										} )
	o.unitsSubGUI:addComponent( o.unitSelection )	
	--o.unitsSubGUI:addComponent( Image:new( { mat="assets/texture/ui/aa-temp_prep_leftPanel.material" } ) )
	
	o.unitsSubGUI:addPlacementHandler( SimplePlacer )
	
	-- Purchased Units Sub-GUI
	o.purchaseSubGUI = GUI:new{ width=240, height=322, yoffset=350 }
	
	o.purchaseSubGUI:addComponent( Button:new
											{ 
												xoffset=-5,
												yoffset=-12,
												anchor="South",
												matReleased="assets/texture/ui/remove-button-release.material",
												matPressed="assets/texture/ui/remove-button-press.material",
												matHover="assets/texture/ui/remove-button-hover.material",
												onClick=o.onRemoveSelected,
											} )
	
	--o.purchaseSubGUI:addComponent( Image:new( { mat="assets/texture/ui/aa-temp_prep_buyPanel.material", ignoreConstrict=true } ) )
	o.purchaseSubGUI:addComponent( TextLabel:new( { label="Bought units", anchor="NorthWest", xoffset=leftXOffset, yoffset=12 } ) )
	
	o.unitPurchased = TextSelectList:new
										{
											width=216,
											height=208,
											xoffset=leftXOffset,
											yoffset=18,
											anchor="NorthWest", 
											elements=o.createdSquads,
											onSelect=o.onSelectCreatedSquad
										}
	o.purchaseSubGUI:addComponent( o.unitPurchased )
	
	o.purchaseSubGUI:addPlacementHandler( AnchorPlacer )
	
	o.panelSubGUI = GUI:new{ width=240, height=673, anchor="SouthWest" }
	
	o.panelSubGUI:addComponent( o.unitsSubGUI )
	o.panelSubGUI:addComponent( o.purchaseSubGUI )
	o.panelSubGUI:addPlacementHandler( SimplePlacer )
	
	o.panelSubGUI:addComponent( Image:new( { mat="assets/texture/ui/prep-panel.material" } ) )
	o.gui:addComponent( o.panelSubGUI )
	
    o.goButton = Button:new	{ 
										anchor="SouthEast", xoffset=-66, yoffset=-11, enabled=false,
										onClick=function() 
													if o.cashLeft == 0 then
														o.onFinished()
													else
														o.msgBox:setShow( true ) 
													end
												end,										
										matReleased="assets/texture/ui/go-button-release.material",
										matPressed="assets/texture/ui/go-button-press.material",
										matHover="assets/texture/ui/go-button-hover.material",
										matDisable="assets/texture/ui/go-button-disable.material",
									}
	
	o.gui:addComponent( o.goButton )
											
	o.gui:addPlacementHandler( AnchorPlacer )

	o.shieldStatsSubGUI:setShow( false )
	o.tgStatsSubGUI:setShow( false )
	
    return o
end

function PrepInterface:enableGo( flag )
    self.goButton:setEnabled( flag )
end

function PrepInterface:setSelected( squad )
   self.unitSelection:setActive( squad )
end

function PrepInterface:setBoughtSelected( instance )
    self.unitPurchased:setActive( instance )
end

function PrepInterface:setRemainingMoney( value )
	self.cashLeft = value
    self.cashLabel:setLabel( "$" .. self.cashLeft )
end

function PrepInterface:updatePurchasedList()
    self.unitPurchased:updateList()
end

function PrepInterface:destroy()
	self.msgBox:destroy()
    self.gui:destroy()  
    self.gui = nil
end

return PrepInterface
