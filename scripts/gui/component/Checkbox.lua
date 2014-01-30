local GUIComponent = require "gui/component/GUIComponent"
local image = require"factories/image"

local Checkbox = 
                {
                    x=0,
                    y=0,
                    checked = false,
                    matOpen = "assets/material/ui/checkbox_open.material",
                    matSelected = "assets/material/ui/checkbox_selected.material",
                    matHoverOpen = "assets/material/ui/checkbox_hover_open.material",
                    matHoverSelected = "assets/material/ui/checkbox_hover_selected.material"
                }

local function show( img, flag )
    img.ent:set( core.componentType.GraphicsComponent, { render = flag }, true )
end

local function active( o, img )
    show( o.openImg, img == o.openImg ) 
    show( o.selectedImg, img == o.selectedImg )
    show( o.hoverOpenImg, img == o.hoverOpenImg )
    show( o.hoverSelectedImg, img == o.hoverSelectedImg )
end

function Checkbox:new(o)
    o = o or {}
    setmetatable( o,self )
    self.__index = self
    
    o.openImg = image( o.x, o.y, o.matOpen )
    o.selectedImg = image( o.x, o.y, o.matSelected )
    o.hoverOpenImg = image( o.x, o.y, o.matHoverOpen )
    o.hoverSelectedImg = image( o.x, o.y, o.matHoverSelected )
    
    o.width = o.openImg.width
    o.height = o.openImg.height

    o.onChange = o.onChange or function() end

    o.GUIComponent = GUIComponent:new
                                        {
                                            x=o.x,
                                            y=o.y,
                                            width=o.width,
                                            height=o.height,
                                            onPress = function()  o:onPress() end,
                                            onRelease = function() o:onRelease() end,
                                            onEnter = function() o:onEnter() end,
                                            onExit = function() o:onExit() end
                                        }

    if o.checked then
        active( o,o.selectedImg )
    else
        active( o,o.openImg )  
    end

    o.openImg.ent:set( core.componentType.GraphicsComponent, { render = true }, true )
    return o
end


function Checkbox:onPress() 
end

function Checkbox:onRelease()
    if self.checked then
        self.checked = false
        active( self, self.hoverOpenImg )
    else
        self.checked = true
        active( self, self.hoverSelectedImg )
    end

    self.onChange( self.checked )
end

function Checkbox:onEnter()
    if self.checked then
        active( self, self.hoverSelectedImg )
    else
        active( self, self.hoverOpenImg )
    end
end

function Checkbox:onExit()
    if self.checked then
        active( self, self.selectedImg )
    else
        active( self, self.openImg )
    end
end

function Checkbox:destroy()
    self.openImg:destroy()
    self.selectedImg:destroy()
    self.hoverOpenImg:destroy()
    self.hoverSelectedImg:destroy()
    self.GUIComponent:destroy()
end

return Checkbox
