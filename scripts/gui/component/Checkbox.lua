local GUIComponent = require "gui/component/GUIComponent"
local image = require"factories/image"

local Checkbox = 
                {
                    x=0,
                    y=0,
                    xoffset=0,
                    yoffset=0,
                    width=0,
                    height=0,
                    checked = false,
                    matOpen = "assets/texture/ui/checkbox-open_00.material",
                    matSelected = "assets/texture/ui/checkbox-selected_00.material",
                    matHoverOpen = "assets/texture/ui/checkbox-hover-open_00.material",
                    matHoverSelected = "assets/texture/ui/checkbox-hover-selected_00.material",
                    onChange = nil
                }


function Checkbox:active(  img )
    self.openImg:show( img == self.openImg )
    self.selectedImg:show( img == self.selectedImg )
    self.hoverOpenImg:show( img == self.hoverOpenImg )
    self.hoverSelectedImg:show( img == self.hoverSelectedImg )
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
        o:active( o.selectedImg )
    else
        o:active( o.openImg )  
    end

    o.openImg.ent:set( core.componentType.GraphicsComponent, { render = true }, true )
    return o
end

function Checkbox:setPosition( x,y )
    self.GUIComponent:setPosition( x,y )
    self.openImg:setPosition( x, y )
    self.selectedImg:setPosition( x,y )
    self.hoverOpenImg:setPosition( x,y )
    self.hoverSelectedImg:setPosition( x,y )

    if self.checked then
        if self.inside then
            self:active( self.hoverSelectedImg )
        else
            self:active( self.selectedImg )
        end
    else
        if self.inside then
            self:active( self.hoverOpenImg )
        else
            self:active( self.openImg )
        end
    end
end

function Checkbox:onPress() 
end

function Checkbox:onRelease()
    if self.checked then
        self.checked = false
        self:active( self.hoverOpenImg )
    else
        self.checked = true
        self:active( self.hoverSelectedImg )
    end

    self.onChange( self.checked )
end

function Checkbox:onEnter()
    self.inside = true
    if self.checked then
        self:active( self.hoverSelectedImg )
    else
        self:active( self.hoverOpenImg )
    end
end

function Checkbox:onExit()
    self.inside = false
    if self.checked then
        self:active(self.selectedImg )
    else
        self:active( self.openImg )
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
