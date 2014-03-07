local GUI = require "gui/GUI"
local Button = require "gui/component/Button"
local Image = require "gui/component/Image"

local CenterPlacer = require "gui/placement/CenterPlacer"

local PauseMenuGUI = GUI:new
{ 
    
}

function PauseMenuGUI:new(o)
    o = GUI.new(self,o)

    o:addPlacementHandler(CenterPlacer)
    o:addComponent( Image:new{
        mat = "assets/texture/ui/pause-title_00.material",
    } )
    o:addComponent( Button:new{
        onClick = function() openscenario( "main_menu" ) end ,
        matReleased = "assets/texture/ui/exit-pause-button-release.material",
        matPressed = "assets/texture/ui/exit-pause-button-press.material",
        matHover = "assets/texture/ui/exit-pause-button-hover.material",
      
    })
    o:addComponent( Button:new{
        onClick = core.flagExit,
        matReleased = "assets/texture/ui/exit-pause-button-release.material",
        matPressed = "assets/texture/ui/exit-pause-button-press.material",
        matHover = "assets/texture/ui/exit-pause-button-hover.material",
    })
    return o
end

return PauseMenuGUI
