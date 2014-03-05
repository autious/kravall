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

    o.gui = GUI:new()


    o.gui:addComponent(Button:new({
                                    matReleased = "assets/texture/ui/logo-kravall_00.material",
                                    matPressed = "assets/texture/ui/logo-kravall_00.material",
                                    matHover = "assets/texture/ui/logo-kravall_00.material",
                                    xoffset=0, yoffset=-100}))

    o.gui:addComponent(Button:new({
                                    matReleased = "assets/texture/ui/start-button-release_00.material",
                                    matPressed = "assets/texture/ui/start-button-press_00.material",
                                    matHover = "assets/texture/ui/start-button-hover_00.material",
                                    xoffset=-20, yoffset=10, onClick = menuState.goScenario,
									toolTip=createTT:new( { handler=toolTipHandler, text="Start the game!" } )
                                  }))

    o.gui:addComponent(Button:new({

                                    matReleased = "assets/texture/ui/options-button-release_00.material",
                                    matPressed = "assets/texture/ui/options-button-press_00.material",
                                    matHover = "assets/texture/ui/options-button-hover_00.material",
                                    xoffset=100,yoffset=20,onClick = menuState.goSettings
                                  }))

    o.gui:addComponent(Button:new({
                                    matReleased = "assets/texture/ui/credits-button-release_00.material",
                                    matPressed = "assets/texture/ui/credits-button-press_00.material",
                                    matHover = "assets/texture/ui/credits-button-hover_00.material",
                                    xoffset=-10,yoffset=20,onClick = menuState.goCredits
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
