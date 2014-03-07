local GUI = require "gui/GUI"
local Button = require "gui/component/Button"
local Slider = require "gui/component/Slider"
local Checkbox = require "gui/component/Checkbox"
local CenterPlacer = require "gui/placement/CenterPlacer"
local toolTipHandler = require "gui/tooltip/ToolTipHandler"
local createTT = require "gui/tooltip/ToolTip"

MainMenu = {}

function MainMenu:new(o,menuState)
    o = o or {}
	
	local offsetX = -100
	local offsetY = 200

    o.gui = GUI:new()

    o.gui:addComponent(Button:new({
                                    matReleased = "assets/texture/ui/start-button-release_00.material",
                                    matPressed = "assets/texture/ui/start-button-press_00.material",
                                    matHover = "assets/texture/ui/start-button-hover_00.material",
                                    xoffset=offsetX, yoffset=offsetY, onClick = menuState.goScenario
                                  }))

    o.gui:addComponent(Button:new({

                                    matReleased = "assets/texture/ui/options-button-release_00.material",
                                    matPressed = "assets/texture/ui/options-button-press_00.material",
                                    matHover = "assets/texture/ui/options-button-hover_00.material",
                                    xoffset=offsetX,yoffset=offsetY,onClick = menuState.goSettings
                                  }))

    o.gui:addComponent(Button:new({
                                    matReleased = "assets/texture/ui/credits-button-release_00.material",
                                    matPressed = "assets/texture/ui/credits-button-press_00.material",
                                    matHover = "assets/texture/ui/credits-button-hover_00.material",
                                    xoffset=offsetX,yoffset=offsetY,onClick = menuState.goCredits
                                    }))

	 o.gui:addComponent(Button:new({
                                    matReleased = "assets/texture/ui/credits-button-release_00.material",
                                    matPressed = "assets/texture/ui/credits-button-press_00.material",
                                    matHover = "assets/texture/ui/credits-button-hover_00.material",
                                    xoffset=offsetX,yoffset=offsetY,onClick = menuState.goTutorial,
									toolTip=createTT:new( { handler=toolTipHandler, text="Don't mind the text, it is confused. It means to say 'Tutorial'!" } )
                                    }))

    o.gui:addPlacementHandler( CenterPlacer )

    setmetatable( o, self )
    self.__index = self
    return o
end

function MainMenu:destroy()
	toolTipHandler:deregister()
    self.gui:destroy() 
end

return MainMenu
