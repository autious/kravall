local GUI = require "gui/GUI"

local Checkbox = require "gui/component/Checkbox"
local TextLabel = require "gui/component/TextLabel"
local TextBox = require "gui/component/TextBox"
local Image = require "gui/component/Image"
local Button = require "gui/component/Button"

local AnchorPlacer = require "gui/placement/AnchorPlacer"
local CenterPlacer = require "gui/placement/CenterPlacer"
local SimplePlacer = require "gui/placement/SimplePlacer"

-- buttons can be set to "OKOnly", "OKCancel" or "YesNo"
local MessageBox = { 
						x=0, y=0, width=300, height=120, show=false, title="Kravall message", message="This is a message!",
						buttons="OKOnly", onPrimaryClick=function( self ) end, 
						onSecondaryClick=function( self ) end
					}

-- To work, must not be put in a GUI but added as a variable and created BEFORE any GUI components it is supposed
-- to be on top of. Because of this, it also needs to be destroyed manually by calling the MessageBox:destroy()-
-- function on the variable.


function MessageBox:new(o)
    o = o or {}

    setmetatable( o, self )
    self.__index = self
    
	o.gui = GUI:new( { } )
	o.wrapperGUI = GUI:new( { width=360, height=181 } )
	
	o.wrapperGUI:addComponent( TextLabel:new( { label=o.title, xoffset=18, font="assets/font/toolTip.font" } ) )
	--o.wrapperGUI:addComponent( TextLabel:new( { label="MessageBox text.", xoffset=20, yoffset=23, font="assets/font/toolTip.font" } ) )
	
	local message = TextBox:new( { body=o.message, xoffset=22, yoffset=22, width=323, height=105, font="assets/font/toolTip.font" } )
	local dimX, dimY = message.text:getDim()
		message.yoffset = message.yoffset - dimY	
	o.wrapperGUI:addComponent( message )
	
	o.buttonGUI = GUI:new( { xoffset=17, yoffset=120, width=328, height=61 } )
	if o.buttons == "OKCancel" then
		-- OK Button
		o.buttonGUI:addComponent( Button:new (
												{ 
													anchor="West",
													matReleased="assets/texture/ui/ok-button-release.material",
													matPressed="assets/texture/ui/ok-button-press.material",
													matHover="assets/texture/ui/ok-button-hover.material",
													onClick=function() o:setShow( false ) o:onPrimaryClick() end
												} ) )
		-- Cancel Button
		o.buttonGUI:addComponent( Button:new (
												{
													anchor="East",
													matReleased="assets/texture/ui/cancel-button-release.material",
													matPressed="assets/texture/ui/cancel-button-press.material",
													matHover="assets/texture/ui/cancel-button-hover.material",
													onClick=function() o:setShow( false ) o:onSecondaryClick() end
												} ) )
	elseif o.buttons == "YesNo" then
		-- Yes button
		o.buttonGUI:addComponent( Button:new (
												{ 
													anchor="West",
													matReleased="assets/texture/ui/yes-button-release.material",
													matPressed="assets/texture/ui/yes-button-press.material",
													matHover="assets/texture/ui/yes-button-hover.material",
													onClick=function() o:setShow( false ) o:onPrimaryClick() end
												} ) )
		-- No button
		o.buttonGUI:addComponent( Button:new (
												{
													anchor="East",
													matReleased="assets/texture/ui/no-button-release.material",
													matPressed="assets/texture/ui/no-button-press.material",
													matHover="assets/texture/ui/no-button-hover.material",
													onClick=function() o:setShow( false ) o:onSecondaryClick() end
												} ) )
	else
		-- OK button
		o.buttonGUI:addComponent( Button:new (
												{
													anchor="Center",
													matReleased="assets/texture/ui/ok-button-release.material",
													matPressed="assets/texture/ui/ok-button-press.material",
													matHover="assets/texture/ui/ok-button-hover.material",
													onClick=function() o:setShow( false ) o:onPrimaryClick() end
												} ) )
	end
	o.buttonGUI:addPlacementHandler( AnchorPlacer )
	o.wrapperGUI:addComponent( o.buttonGUI )
	o.wrapperGUI:addComponent( Image:new( { mat="assets/texture/ui/message-box-background.material" } ) )
	
	o.wrapperGUI:addPlacementHandler( SimplePlacer )
	
	o.gui:addComponent( o.wrapperGUI )
	o.gui:addPlacementHandler( CenterPlacer )
	
	o:setShow( o.show )
    return o
end

--function MessageBox:onButtonClick( button )
--{
--	self:setShow( false )
--	if button then
--		button:onClick()
--	end
--}

function MessageBox:setShow( flag )
	self.show = flag
    self.gui:setShow( self.show ) 
end

function MessageBox:destroy()
    self.gui:destroy() 
end

return MessageBox
