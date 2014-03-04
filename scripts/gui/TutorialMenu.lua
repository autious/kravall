local GUI = require "gui/GUI"
local Button = require "gui/component/Button"
local Slider = require "gui/component/Slider"
local Checkbox = require "gui/component/Checkbox"
local AnchorPlacer = require "gui/placement/AnchorPlacer"
local TextLabel = require "gui/component/TextLabel"
local TextBox = require "gui/component/TextBox"
local image = require "gui/component/Image"
local AnchorPlacer = require "gui/placement/AnchorPlacer"


TutorialMenu = {}
local logo
function TutorialMenu:new(o, menuState, logo)
	o = o or {}
	o.gui = GUI:new()
	
	--go back to main menu
	o.gui:addComponent(Button:new({
                                matReleased = "assets/texture/ui/back-button-release.material",
								matPressed = "assets/texture/ui/back-button-press.material",
								matHover = "assets/texture/ui/back-button-hover.material",
                                anchor="SouthWest",xoffset=0,yoffset=0, onClick = menuState.goMain }))



	if logo == -1 then
	elseif logo == 0 then
			o.bgImage = image:new
{ 
        ignoreConstrict=true,
        mat= "assets/texture/tutorial/tut1.material"
    }
	elseif logo == 1 then
	elseif logo == 2 then
	end


	o.gui:addComponent(o.bgImage)
	o.gui:addPlacementHandler( AnchorPlacer )
	setmetatable( o, self )
    self.__index = self

    return o

end

function TutorialMenu:destroy()
	self.gui:destroy()
end

return TutorialMenu