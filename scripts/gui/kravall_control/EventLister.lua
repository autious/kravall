local text = require "factories/text"
local GUIComponent = require "gui/component/GUIComponent"

local EventListerPlacer = 
                { 
                }
local TextEvent = { padding = 10}

function EventListerPlacer:new(o)
    o = o or {}

    setmetatable( o, self )
    self.__index = self
    o.textEvents = {}

    return o
end

function SouthWestPlacer:constrict( components, winWidth, winHeight, posx, posy )
    posx = posx or 0
    posy = posy or 0
    
    local tw,th = self:getTotalDim( components )
    local startx = 0
    local starty = winHeight - th
    for _,c in pairs( components ) do 
        c:setPosition( startx + c.xoffset + self.padding + posx, starty + c.yoffset + posy )
        starty = starty + c.height + self.padding
    end 
end

function EventListerPlacer:update( delta )
    self.count = self.count + delta     

    if self.count > 5 then
        self.count = 0
        local eventObj = {}
        eventObj.str = "HEJHJ" .. delta
        self:queueEvent( eventObj )
    end

    local eventCount = #(self.textEvents)

    for i,v in pairs( self.textEvents ) do
        self.textEvents  
    end
end

function EventListerPlacer:setPosition( x, y )
    print( "EventListerPlacerPos " .. x .. " " .. y )
    self.GUIComponent:setPosition(x,y)
    self.x = x
    self.y = y
end

function EventListerPlacer:onPress()
end

function EventListerPlacer:onRelease()
end

function EventListerPlacer:onEnter()
    print ("ENTER" )
end

function EventListerPlacer:onExit()
end

function EventListerPlacer:destroy()
    self.GUIComponent:destroy()
end

return EventListerPlacer
