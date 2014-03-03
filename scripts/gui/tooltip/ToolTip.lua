
local ToolTip = 
                {
					x=0,
					y=0,
					show=true,
					width=30,
					height=10
                    --x=0,
                    --y=0,
                    --xoffset=0,
                    --yoffset=0,
                    --show = true,
                    --matReleased = "assets/texture/ui/button-release_00.material",
                    --matPressed = "assets/texture/ui/button-press_00.material",
                    --matHover = "assets/texture/ui/button-hover_00.material",
                    --onClick = function( self ) end
                }

function ToolTip:new(o)
    o = o or {}
    setmetatable( o,self )
    self.__index = self
	
	o.x = x
	o.y = y
	o.width = width
	o.height = height
	o.show = show
    
    --o.pressedImg = image( o.x, o.y, o.matPressed )
    --o.releasedImg = image( o.x, o.y, o.matReleased )
    --o.hoverImg = image( o.x, o.y, o.matHover )
    --
    --o.width = o.pressedImg.width
    --o.height = o.pressedImg.height
    --
    --o.GUIComponent = GUIComponent:new
    --                                    {
    --                                        x=o.x,
    --                                        y=o.y,
    --                                        width=o.width,
    --                                        height=o.height,
    --                                        onPress = function()  o:onPress() end,
    --                                        onRelease = function() o:onRelease() end,
    --                                        onEnter = function() o:onEnter() end,
    --                                        onExit = function() o:onExit() end
    --                                    }
    --
    --o.pressedImg:show(false)
    --o.releasedImg:show(true)
    --o.hoverImg:show(false)
    --
    --o:setShow( o.show )

    return o
end

function ToolTip:update()
	--mouseX, mouseY = core.input.mouse.getPosition()
	--print("mousePos : " .. mouseX .. ", " .. mouseY
	--						  pos.x, pos.y, dim.x, dim.y, solid
	--core.draw.drawRectangle(float, float, float, float, bool)
	core.draw.drawRectangle(60.0, 60.0, 10.0, 10.0, true)
end

function ToolTip:setShow( flag )
    self.show = flag
    --self.GUIComponent:setShow(flag)
    --self.releasedImg:show(flag)
    --
    --if flag == false then
    --    self.pressedImg:show(false)
    --    self.hoverImg:show(false)
    --end
end

function ToolTip:setPosition(x,y)
    --self.GUIComponent:setPosition(x,y)
    --self.pressedImg:setPosition(x,y)
    --self.releasedImg:setPosition(x,y)
    --self.hoverImg:setPosition(x,y)
end

function ToolTip:onPress() 
    --if self.show then
    --    self.pressedImg:show(true)
    --    self.releasedImg:show(false)
    --    self.hoverImg:show(false)
    --end
end

function ToolTip:onRelease()
    --if self.show then
    --    self.pressedImg:show(false)
    --    self.releasedImg:show(false)
    --    self.hoverImg:show(true)
    --
    --    self:onClick()
    --end
end

function ToolTip:onEnter()
    --if self.show then
    --    self.pressedImg:show( false )
    --    self.releasedImg:show( false )
    --    self.hoverImg:show( true )
    --end
end

function ToolTip:onExit()
    --if self.show then
    --    self.pressedImg:show(false)
    --    self.releasedImg:show(true)
    --    self.hoverImg:show(false)
    --end
end

function ToolTip:destroy()
    --self.pressedImg:destroy()
    --self.releasedImg:destroy()
    --self.hoverImg:destroy()
    --self.GUIComponent:destroy()
end

return ToolTip