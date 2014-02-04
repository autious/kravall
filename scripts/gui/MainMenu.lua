local GUI = require "gui/GUI"
local Button = require "gui/component/Button"
local Slider = require "gui/component/Slider"
local Checkbox = require "gui/component/Checkbox"
local CenterPlacer = require "gui/placement/CenterPlacer"

MainMenu = {}

function MainMenu:new(o,menuState)
    o = o or {}

    o.gui = GUI:new()


    o.gui:addComponent(Button:new({
                                    matReleased = "assets/material/ui/logoKravall.material",
                                    matPressed = "assets/material/ui/logoKravall.material",
                                    matHover = "assets/material/ui/logoKravall.material",
                                    xoffset=0,yoffset=-100}))

    o.gui:addComponent(Button:new({
                                    matReleased = "assets/material/ui/startButton.material",
                                    matPressed = "assets/material/ui/startButton.material",
                                    matHover = "assets/material/ui/startButton.material",
                                    xoffset=-20,yoffset=10,onClick = function() openscenario( "prototype_area") end
                                  }))

    o.gui:addComponent(Button:new({

                                    matReleased = "assets/material/ui/optionsButton.material",
                                    matPressed = "assets/material/ui/optionsButton.material",
                                    matHover = "assets/material/ui/optionsButton.material",
                                    xoffset=100,yoffset=20,onClick = menuState.goScenario 
                                  }))

    o.gui:addComponent(Button:new({
                                    matReleased = "assets/material/ui/creditsButton.material",
                                    matPressed = "assets/material/ui/creditsButton.material",
                                    matHover = "assets/material/ui/creditsButton.material",
                                    xoffset=-10,yoffset=20,onClick = menuState.goCredits
                                    }))

    o.gui:addPlacementHandler( CenterPlacer:new() )

    setmetatable( o, self )
    self.__index = self
    return o
end

function MainMenu:destroy()
    self.gui:destroy() 
end

return MainMenu
