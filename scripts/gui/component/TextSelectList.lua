local GUIComponent = require "gui/component/GUIComponent"
local image = require"factories/image"
local text = require"factories/text"

local TextSelectList = 
                {
                    x=0,
                    y=0,
                    xoffset=0,
                    yoffset=0,
                    width=0,
                    height=0,
                    elements = { { name = "default" } }
                }

function TextSelectList:new(o)
    o = o or {}
    setmetatable( o,self )
    self.__index = self

    o.onClick = o.onClick or function()  end

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
                                            onExit = function() o:onExit() end
                                        }

    o:updateList()

    return o
end

function TextSelectList:updateList()
    for _,v in pairs( self.textElements ) do
        v:destroy()        
    end

    self.textElements = {}

    for k,v in pairs( self.elements ) do
        self.textElements[#(self.textElements)] = text(20,20,v.name)
    end
end

function TextSelectList:render()

end

function TextSelectList:setPosition(x,y)
end

function TextSelectList:onPress() 
end

function TextSelectList:onRelease()
end

function TextSelectList:onEnter()
end

function TextSelectList:onExit()
end

function TextSelectList:destroy()
    self.GUIComponent:destroy()
end

return TextSelectList
