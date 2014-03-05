local GUIComponent = require "gui/component/GUIComponent"
local image = require"factories/image"
local text = require"factories/text"

local TextLabel = 
                {
                    x=0,
                    y=0,
                    xoffset=0,
                    yoffset=0,
                    width=0,
                    height=0,
                    padding=4,
                    show = true,
                    label = "Default Label" 
                }

function TextLabel:new(o)
    o = o or {}
    setmetatable( o,self )
    self.__index = self

    o.onClick = o.onClick or function(self)  end

    o.textElements = {}

    o.text = text( o.x, o.y, o.label, o.color, o.font )
    o.width, o.height = o.text:getDim()

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
    o:setPosition( o.x, o.y )

    o:setShow( o.show )

    return o
end

function TextLabel:setShow( flag )
    self.show = flag
    self.text:show( flag )
end

function TextLabel:setLabel( string )
    self.text:setText( string ) 
    self.width, self.height = self.text:getDim()
    self.GUIComponent.width =  self.width
    self.GUIComponent.height =  self.height
end

function TextLabel:setPosition( x, y )
    self.x = x;
    self.y = y 
    
    self.text:setPosition( self.x, self.y )
    self.GUIComponent:setPosition( self.x, self.y )
end

function TextLabel:onPress() 
end

function TextLabel:onRelease()
    self.onClick(self)
end

function TextLabel:onEnter()
end

function TextLabel:onExit()
end

function TextLabel:destroy()
    self.text:destroy()
    self.GUIComponent:destroy()
end

return TextLabel
