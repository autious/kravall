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
                                        core.saveHomeConfiguration() 
                                        menuState.goMain()
                                    end}))
        

    o.fullscreenGUI = GUI:new{xoffset=200, yoffset= 150, width=2,height=2,anchor="NorthWest"}
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

    o.gui:addComponent( TextLabel:new{xoffset=200, yoffset= 125,label="Options", anchor="NorthWest"} )
    o.gui:addComponent( o.fullscreenGUI  )
	
	
    o.gui:addComponent( TextLabel:new{xoffset=200, yoffset= 180,label="Color grading", anchor="NorthWest"} )
	
	local lutList = TextSelectList:new{
	xoffset = 220, yoffset = 180, show = true, width = 200, height = 200, 
	elements ={ {name = "Normal"},  {name = "Greyscale"}, {name= "Noir"}, {name = "Sepia"}, {name = "Darkness"}, {name = "Deuteranomaly"}, {name = "Inverted"}, {name = "Midnight"}, {name = "Candyland"}},
	onSelect = nil,
	anchor = "NorthWest"}
	lutList.onSelect = function() SettingsMenu:changeLut(lutList.activeObject) end
	o.gui:addComponent(lutList)
	
	o.gui:addComponent( TextLabel:new{xoffset=200, yoffset= 180,label="Gamma", anchor="NorthWest"} )
	
	local gammaSlider = Slider:new{
	    a = (core.gfx.getGamma() - 0.2) / 4 , 
        onChange = function( self, value) core.gfx.setGamma(0.2 + value * 4); core.config.gamma = core.gfx.getGamma() end, 
        anchor = "NorthWest", 
        xoffset=200, 
        yoffset= 180
    }
	o.gui:addComponent(gammaSlider)

	o.gui:addComponent( TextLabel:new{xoffset=200, yoffset= 180,label="Camera Force", anchor="NorthWest"} )
	local cameraForceSlider = Slider:new{
	    a = (core.config.cameraForce - core.config.minCameraForce)/core.config.maxCameraForce, 
        onChange = function( self, value) core.config.cameraForce = value * (core.config.maxCameraForce-core.config.minCameraForce) + core.config.minCameraForce end, 
        anchor = "NorthWest", 
        xoffset=200, 
        yoffset= 180
    }
    o.gui:addComponent(cameraForceSlider)

    o.gui:addPlacementHandler( AnchorPlacer )

    return o
end

function SettingsMenu:changeLut(o)
	if o.name == "Noir" then
		core.gfx.setLUT("redtest")
	elseif o.name == "Inverted" then
		core.gfx.setLUT("invert")
	elseif o.name == "Normal" then
		core.gfx.setLUT("identity")
	elseif o.name == "Sepia" then
		core.gfx.setLUT("sepia")
	elseif o.name == "Greyscale" then
		core.gfx.setLUT("blackandwhite")
	elseif o.name == "Deuteranomaly" then
		core.gfx.setLUT("deuteranomaly")
	elseif o.name == "Darkness" then
		core.gfx.setLUT("coolnamelut")
	elseif o.name == "Midnight" then
		core.gfx.setLUT("midnight")
	elseif o.name == "Candyland" then
		core.gfx.setLUT("candyland")
	end
end

function SettingsMenu:destroy()
    self.gui:destroy() 
end

return SettingsMenu
