local GUIComponent = require "gui/component/GUIComponent"
local image = require"factories/image"

local Image = 
                {
                    x=0,
                    y=0,
                    xoffset=0,
                    yoffset=0,
                    show = true,
                    mat = "assets/texture/ui/button-release_00.material",
                }

function Image:new(o)
    o = o or {}
    setmetatable( o,self )
    self.__index = self
    
    o.img = image( o.x, o.y, o.mat )
    
    o.width = o.img.width
    o.height = o.img.height

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

    o.img:show( o.show )
    o:setShow( o.show )

    return o
end

function Image:setShow( flag )
    self.show = flag
    self.GUIComponent:setShow(flag)
    self.img:show(flag)
end

function Image:setPosition(x,y)
    self.GUIComponent:setPosition(x,y)
    self.img:setPosition(x,y)
end

function Image:onPress() 
end

function Image:onRelease()
end

function Image:onEnter()
end

function Image:onExit()
end

function Image:destroy()
    self.img:destroy()
    self.GUIComponent:destroy()
end

return Image
