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
                    show = true,
                    checked = false,
                    inside = false,
                    matOpen = "assets/texture/ui/checkbox-open_00.material",
                    matSelected = "assets/texture/ui/checkbox-selected_00.material",
                    matHoverOpen = "assets/texture/ui/checkbox-hover-open_00.material",
                    matHoverSelected = "assets/texture/ui/checkbox-hover-selected_00.material",
                    onChange = function(self, value) end,
                    onClick = function(self, value) end,
                    onHover = function(self) end,
                    onExit = function(self) end,
                    doStateSwitchOnPress = true
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

    o.GUIComponent = GUIComponent:new
                                        {
                                            x=o.x,
                                            y=o.y,
                                            width=o.width,
                                            height=o.height,
                                            onPress = function()  o:onPress() end,
                                            onRelease = function() o:onRelease() end,
                                            onEnter = function() o:onEnter() end,
                                            onExit = function() o:onExtExit() end,
											toolTip = o.toolTip
                                        }

    o.openImg.ent:set( core.componentType.GraphicsComponent, { render = true }, true )

    o:setShow( o.show )
    o:updateVisual()
    return o
end

function Checkbox:setPosition( x,y )
    self.GUIComponent:setPosition( x,y )
    self.openImg:setPosition( x, y )
    self.selectedImg:setPosition( x,y )
    self.hoverOpenImg:setPosition( x,y )
    self.hoverSelectedImg:setPosition( x,y )

    self:updateVisual()
end

function Checkbox:setShow( flag )
    self.show = flag
    self.GUIComponent:setShow( flag )
    self:updateVisual()
end

function Checkbox:setChecked( checked )
    assert( type(checked) == "boolean" )
    self.checked = checked
    self:updateVisual()
    self:onChange(self.checked)
end

function Checkbox:setInside( inside )
    assert( type(checked) == "boolean" )
    self.inside = inside
    self:updateVisual()
end

function Checkbox:updateVisual()
    if self.show then
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
    else
        self:active( nil )
    end
    
end

function Checkbox:onPress() 
end

function Checkbox:onRelease()

    if self.show then
        if self.doStateSwitchOnPress then
            if self.checked then
                self:setChecked( false )
            else
                self:setChecked( true )
            end
        end

        if self.checked then
            self:onClick( false )
        else
            self:onClick( true )
        end 
    end


    self:updateVisual()
end

function Checkbox:onEnter()
    self.inside = true
    self:onHover()
    self:updateVisual()
end

function Checkbox:onExtExit()
    self.inside = false
    self:onExit()
    self:updateVisual()
end

function Checkbox:destroy()
    self.openImg:destroy()
    self.selectedImg:destroy()
    self.hoverOpenImg:destroy()
    self.hoverSelectedImg:destroy()
    self.GUIComponent:destroy()
end

return Checkbox
