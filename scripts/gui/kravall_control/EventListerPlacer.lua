local PU = require "gui/placement/util"
local leftOffset = 20
local padding = 10

return function ( components, winWidth, winHeight, posx, posy )
    posx = posx or 0
    posy = posy or 0
    
    local tw,th = PU.getTotalDimHeight( components, padding, padding )
    local startx = 0
    local starty = winHeight - th
    for _,c in pairs( components ) do 
        c:setPosition( startx + c.xoffset + padding + posx, starty + c.yoffset + posy )
        starty = starty + c.height + padding
    end 
end

--local text = require "factories/text"
--local GUIComponent = require "gui/component/GUIComponent"
--
--local EventLister = 
--                { 
--                    count = 0,
--                    x=0,
--                    y=0,
--                    xoffset=0,
--                    yoffset=0,
--                    width=200,
--                    height=200,
--                    anchor = "NorthEast" 
--                }
--local TextEvent = {}
--
--function EventLister:new(o)
--    o = o or {}
--
--    setmetatable( o, self )
--    self.__index = self
--    o.textEvents = {}
--
--    o.GUIComponent = GUIComponent:new
--                                        {
--                                            x=o.x,
--                                            y=o.y,
--                                            width=o.width,
--                                            height=o.height,
--                                            onPress = function()  o:onPress() end,
--                                            onRelease = function() o:onRelease() end,
--                                            onEnter = function() o:onEnter() end,
--                                            onExit = function() o:onExit() end
--                                        }
--    o:setPosition( o.x, o.y )
--
--    local eventObj = {}
--    eventObj.str = "Event 1"
--    o:queueEvent( eventObj )
--
--    return o
--end
--
--function EventLister:queueEvent( eventObj )
--    local newEvent = {}
--    newEvent.text = text( self.x, self.y + self.height, eventObj.str )
--
--    self.textEvents[#(self.textEvents)] = newEvent
--end
--
--function EventLister:update( delta )
--    self.count = self.count + delta     
--
--    if self.count > 5 then
--        self.count = 0
--        local eventObj = {}
--        eventObj.str = "HEJHJ" .. delta
--        self:queueEvent( eventObj )
--    end
--
--    local eventCount = #(self.textEvents)
--
--    for i,v in pairs( self.textEvents ) do
--    end
--end
--
--function EventLister:setPosition( x, y )
--    print( "EventListerPos " .. x .. " " .. y )
--    self.GUIComponent:setPosition(x,y)
--    self.x = x
--    self.y = y
--end
--
--return EventLister
