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

    o.gui:addComponent( TextLabel:new{xoffset=-50, yoffset= 180,label="(Following settings require restart to take effect)", anchor="NorthEast"} )

    local shadowMapSliderSteps = {
            {name="Off", map=256, cascade=0},
            {name="Low", map=256, cascade=1},
            {name=core.config.defaultQualityName, map=core.config.defaultShadowResolution, cascade=core.config.defaultShadowQuality},
            {name="High", map=1024, cascade=2},
            {name="Stefan",map=2048, cascade=2},
    }

    local currentSliderStep = 1
    for i,v in pairs( shadowMapSliderSteps ) do
        if v.name == core.config.shadowQualityName then
            currentSliderStep = i
        end
    end

    local shadowMapLabel = TextLabel:new{xoffset=-50, yoffset= 180,label="Shadow Quality", anchor="NorthEast"}

    shadowMapLabel:setLabel( "Shadow Quality (" .. shadowMapSliderSteps[currentSliderStep].name .. ")")
    o.gui:addComponent(shadowMapLabel)
    o.gui:addComponent( Slider:new{
                a = (currentSliderStep-1)/(#shadowMapSliderSteps-1),
                onChange = 
                    function( self, value )     
                    local index = math.floor( value*(#shadowMapSliderSteps-1) + 0.5 ) + 1
                    self.a = (index-1)/(#shadowMapSliderSteps-1)
                    if index > 0 then
                        core.config.shadowQualityName = shadowMapSliderSteps[index].name
                        core.config.shadowResolution = shadowMapSliderSteps[index].map
                        core.config.shadowQuality = shadowMapSliderSteps[index].cascade
                        shadowMapLabel:setLabel( "Shadow Quality (" .. shadowMapSliderSteps[index].name .. ")")
                        print("Name",core.config.shadowQualityName)
                        print("Res", core.config.shadowResolution )
                        print("Quality", core.config.shadowQuality )
                    end
                end, 
                anchor = "NorthEast", 
                xoffset=-50, 
                yoffset= 180
            })

    local mipmapSliderSteps = {
        { name = "Off", data = 0 },
        { name = "Mipmap", data = 1 },
        { name = "Anisotropic", data = 2 },
    }

    local currentMipStep = 1
    for i,v in pairs( mipmapSliderSteps ) do
        if v.data == core.config.mipmapQuality then
            currentMipStep = i
        end
    end

    local mipmapLabel = TextLabel:new{xoffset=-100, yoffset= 180,label="Mipmap ("..mipmapSliderSteps[currentMipStep].name ..")", anchor="NorthEast"}
    
    o.gui:addComponent( mipmapLabel )

    o.gui:addComponent( Slider:new{
                a = (currentMipStep-1)/(#mipmapSliderSteps-1),
                onChange = 
                    function( self, value )     
                    local index = math.floor( value*(#mipmapSliderSteps-1) + 0.5 ) + 1
                    self.a = (index-1)/(#mipmapSliderSteps-1)
                    if index > 0 then
                        core.config.mipmapQuality = mipmapSliderSteps[index].data
                        mipmapLabel:setLabel( "Mipmap (" .. mipmapSliderSteps[index].name .. ")")
                        print( "Name", core.config.mipmapQuality )
                    end
                end, 
                anchor = "NorthEast", 
                xoffset=-50, 
                yoffset= 180
            })

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
                                        core.config.shadowResolution = core.config.defaultShadowResolution
                                        core.config.fullscreen = core.config.defaultFullscreen
                                        o:changeLut({data="identity"})

                                        core.config.shadowQualityName = core.config.defaultQualityName
                                        core.config.shadowResolution = core.config.defaultShadowResolution
                                        core.config.shadowQuality = core.config.defaultShadowQuality

                                        print("Name",core.config.shadowQualityName)
                                        print("Res", core.config.shadowResolution )
                                        print("Quality", core.config.shadowQuality )

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
