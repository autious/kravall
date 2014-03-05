local GUI = require "gui/GUI"
local Button = require "gui/component/Button"
local Slider = require "gui/component/Slider"
local Checkbox = require "gui/component/Checkbox"
local TextSelectList = require "gui/component/TextSelectList"
local TextLabel = require "gui/component/TextLabel"
local TextBox = require "gui/component/TextBox"
local AnchorPlacer = require "gui/placement/AnchorPlacer"
local WestHorizontalPlacer = require "gui/placement/WestHorizontalPlacer"

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
                                    anchor="SouthWest",xoffset=0,yoffset=0,
                                    onClick = function() 
                                        menuState.goMain()
                                    end}))
        

    o.fullscreenGUI = GUI:new{xoffset=200, yoffset= 100, width=2,height=2,anchor="NorthWest"}
    o.fullscreenGUI:addPlacementHandler( WestHorizontalPlacer )

    o.fullscreenGUI:addComponent(Checkbox:new
    {
        checked = core.config.fullscreen,
        onChange = function( self, value )
            core.config.fullscreen = value
            core.saveHomeConfiguration() 
        end
    })

    o.fullscreenGUI:addComponent( TextLabel:new{label="Fullscreen (requires restart to take effect)"} )

    o.gui:addComponent( o.fullscreenGUI  )
    o.gui:addPlacementHandler( AnchorPlacer )

    return o
end

function SettingsMenu:destroy()
    self.gui:destroy() 
end

return SettingsMenu
