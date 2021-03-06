local GUIComponent = require "gui/component/GUIComponent"
local image = require"factories/image"
local text = require"factories/text"

local TextBox = 
                {
                    x=0,
                    y=0,
                    xoffset=0,
                    yoffset=0,
                    width=200,
                    height=200,
                    padding=4,
                    body = "default",
                    show = true
                }

function TextBox:new(o)
    o = o or {}
    setmetatable( o,self )
    self.__index = self

    o.onClick = o.onClick or function() end

    o.textElements = {}

    o.GUIComponent = GUIComponent:new
                                        {
                                            x=o.x,
                                            y=o.y,
                                            width=o.width,
                                            height=o.height,
                                            onPress = function()  o:onPress() end,
                                            onRelease = function() o:onRelease() end,
                                            onEnter = function() o:onEnter() end,
                                            onExit = function() o:onExit() end,
											toolTip = o.toolTip
                                        }
    o.text = text( o.x, o.y, o.body, o.color, o.font, true, {o.width, o.height})
    o:setPosition( o.x, o.y )

    o.text:show( show )

    return o
end

function TextBox:setShow( flag )
    self.show = flag 
   
    self.text:show(flag)
end

function TextBox:setText( string )
    self.text:setText( string )
end

function TextBox:setPosition( x, y )
    self.x = x
    self.y = y
	
    self.text:setPosition( self.x, self.y )
end

function TextBox:getPosition( x, y )
    return self.x, self.y
end

function TextBox:onPress() 
end

function TextBox:onRelease()
end

function TextBox:onEnter()
end

function TextBox:onExit()
end

function TextBox:destroy()
    self.text:destroy()
    self.GUIComponent:destroy()
end

return TextBox
