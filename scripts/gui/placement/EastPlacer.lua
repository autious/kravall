local EastPlacer = {leftOffset = 20, padding = 10}

function EastPlacer:new(o)
    o = o or {}

    setmetatable( o, self )
    self.__index = self

    return o
end

function EastPlacer:constrict( components, winWidth, winHeight )
    local tw,th = self:getTotalDim( components )
    local startx = winWidth
    local starty = (winHeight - th)/2
    for _,c in pairs( components ) do 
        c:setPosition( c.xoffset + startx - c.width - self.padding, starty + c.yoffset )
        starty = starty + c.height + self.padding
    end 
end

function EastPlacer:getTotalDim( components )
    local tw = 0
    local th = 0
    for _,c in pairs( components ) do     
        if tw < c.width then
            tw = c.width
        end
        th = th + c.height + self.padding
    end
    
    return tw, th
end

return EastPlacer
