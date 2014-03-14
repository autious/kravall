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
                                    anchor="SouthWest",xoffset=15, yoffset=-15,
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
	elements =
    { 
        {name = "Normal", data="identity"},  
        {name = "Greyscale", data="blackandwhite"}, 
        {name= "Noir", data="redtest"}, 
        {name = "Sepia", data="sepia"}, 
        {name = "Darkness", data="coolnamelut"}, 
        {name = "Deuteranomaly", data="deuteranomaly"}, 
        {name = "Inverted", data="invert"}, 
        {name = "Midnight", data="midnight"}, 
        {name = "Candyland", data="candyland"}
    },
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

    local forceSpanSize = core.config.maxCameraForce - core.config.minCameraForce

	o.gui:addComponent( TextLabel:new{xoffset=200, yoffset= 180,label="Camera Speed", anchor="NorthWest"} )
	local cameraForceSlider = Slider:new{
	    a = (core.config.cameraForce - core.config.minCameraForce)/forceSpanSize,
        onChange = function( self, value) core.config.cameraForce = value * forceSpanSize + core.config.minCameraForce end, 
        anchor = "NorthWest", 
        xoffset=200, 
        yoffset= 180
    }
    o.gui:addComponent(cameraForceSlider)

    local spanSize = core.config.maxCameraRotationSpeed - core.config.minCameraRotationSpeed

	o.gui:addComponent( TextLabel:new{xoffset=200, yoffset= 180,label="Camera Rotation", anchor="NorthWest"} )
	local cameraRotationSlider = Slider:new{
	    a = (core.config.cameraRotationSpeed - core.config.minCameraRotationSpeed)/spanSize,
        onChange = function( self, value ) core.config.cameraRotationSpeed = value * spanSize + core.config.minCameraRotationSpeed end, 
        anchor = "NorthWest", 
        xoffset=200, 
        yoffset= 180
    }
    o.gui:addComponent(cameraRotationSlider)

    o.gui:addComponent(Button:new({
                                    matReleased = "assets/texture/ui/default-button-release.material",
                                    matPressed = "assets/texture/ui/default-button-press.material",
                                    matHover = "assets/texture/ui/default-button-hover.material",
                                    anchor="SouthEast",xoffset=-15, yoffset=-15,
                                    onClick = function() 
                                        core.config.gamma = core.config.defaultGamma
                                        core.gfx.setGamma(core.config.gamma)
                                        core.config.cameraForce = core.config.defaultCameraForce  
                                        core.config.cameraRotationSpeed = core.config.defaultCameraRotationSpeed
                                        core.config.fullscreen = core.config.defaultFullscreen
                                        o:changeLut({data="identity"})              
                                        menuState.goSettings()
                                    end}))
        

    o.gui:addPlacementHandler( AnchorPlacer )

    return o
end

function SettingsMenu:changeLut(o)

    --[[
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
    ]]--

    core.gfx.setLUT(o.data)
    core.config.lut = o.data
end

function SettingsMenu:destroy()
    self.gui:destroy() 
end

return SettingsMenu
