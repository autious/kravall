local GUI = require "gui/GUI"
local Button = require "gui/component/Button"
local Image = require "gui/component/Image"

local CenterPlacer = require "gui/placement/CenterPlacer"
local SimplePlacer = require "gui/placement/SimplePlacer"

local PauseMenuGUI = GUI:new
{ 
    onClickContinue = function() end 
}

function PauseMenuGUI:new(o)
    o = GUI.new(self,o)
	
	local xOff = 15
	o.panel = GUI:new ( { width=243, height=267 } )
	
    o.panel:addComponent( Button:new{
									xoffset=xOff, yoffset=15,
									onClick = o.onClickContinue,
									matReleased = "assets/texture/ui/continue-button-release.material",
									matPressed = "assets/texture/ui/continue-button-press.material",
									matHover = "assets/texture/ui/continue-button-hover.material",
								  
								} )
    o.panel:addComponent( Button:new{
									xoffset=xOff, yoffset=70,
									onClick = reopenScenario,
									matReleased = "assets/texture/ui/restart-button-release.material",
									matPressed = "assets/texture/ui/restart-button-press.material",
									matHover = "assets/texture/ui/restart-button-hover.material",
								} )
	o.panel:addComponent( Button:new{
									xoffset=xOff, yoffset=125,
									onClick = function() openscenario( "main_menu" ) end ,
									matReleased = "assets/texture/ui/menu-button-release.material",
									matPressed = "assets/texture/ui/menu-button-press.material",
									matHover = "assets/texture/ui/menu-button-hover.material",
								  
								} )
	o.panel:addComponent( Button:new{
									xoffset=xOff, yoffset=180,
									onClick = core.flagExit,
									matReleased = "assets/texture/ui/exit-pause-button-release.material",
									matPressed = "assets/texture/ui/exit-pause-button-press.material",
									matHover = "assets/texture/ui/exit-pause-button-hover.material",
								} )
	o.panel:addComponent( Image:new{
								mat = "assets/texture/ui/pause-background_00.material",
							} )	
							
	o.panel:addPlacementHandler( SimplePlacer )
	
	o:addComponent( o.panel )
	o:addPlacementHandler( CenterPlacer )
    return o
end

return PauseMenuGUI
