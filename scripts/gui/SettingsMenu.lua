local GUI = require "gui/GUI"
local Button = require "gui/component/Button"
local Slider = require "gui/component/Slider"
local Checkbox = require "gui/component/Checkbox"
local TextSelectList = require "gui/component/TextSelectList"
local TextLabel = require "gui/component/TextLabel"
local TextBox = require "gui/component/TextBox"
local AnchorPlacer = require "gui/placement/AnchorPlacer"

local SettingsMenu = {}

function SettingsMenu:new(o,menuState)
    o = o or {}

    setmetatable( o, self )
    self.__index = self

    o.gui = GUI:new()

    o.gui:addComponent(Button:new({
                                    matReleased = "assets/texture/ui/back-button-release.material",
                                    matPressed = "assets/texture/ui/back-button-press.material",
                                    matHover = "assets/texture/ui/back-button-hover.material",
                                    anchor="SouthWest",xoffset=0,yoffset=0,onClick = menuState.goMain }))

    o.gui:addPlacementHandler( AnchorPlacer )

    return o
end

function SettingsMenu:destroy()
    self.gui:destroy() 
end

return SettingsMenu
