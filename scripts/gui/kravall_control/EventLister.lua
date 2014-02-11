local text = require "factories/text"
local GUIComponent = require "gui/component/GUIComponent"

local EventLister = 
                { 
                    count = 0,
                    x=0,
                    y=0,
                    xoffset=0,
                    yoffset=0,
                    width=200,
                    height=200,
                    anchor = "NorthEast" 
                }
local TextEvent = {}

function EventLister:new(o)
    o = o or {}

    setmetatable( o, self )
    self.__index = self
    o.textEvents = {}

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
    o:setPosition( o.x, o.y )
    return o
end

function EventLister:queueEvent( eventObj )
    local newEvent = {}
    newEvent.text = text( self.x, self.y + self.height, eventObj.str )

    self.textEvents[#(self.textEvents)] = newEvent
end

function EventLister:update( delta )
    self.count = self.count + delta     

    if self.count > 5 then
        self.count = 0
        local eventObj = {}
        eventObj.str = "HEJHJ" .. delta
        self:queueEvent( eventObj )
    end

    local eventCount = #(self.textEvents)

    for i,v in pairs( self.textEvents ) do
    end
end

function EventLister:setPosition( x, y )
    print( "New Position" .. x .. y )
    self.GUIComponent:setPosition(x,y)
    self.x = x
    self.y = y
end

function EventLister:onPress()
end

function EventLister:onRelease()
end

function EventLister:onEnter()
    print ("ENTER" )
end

function EventLister:onExit()
end

function EventLister:destroy()
    self.GUIComponent:destroy()
end

return EventLister
