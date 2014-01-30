local LeftPlacer = {leftOffset = 20, padding = 10}

function LeftPlacer:new(o)
    o = o or {}

    setmetatable( o, self )
    self.__index = self

    return o
end

function LeftPlacer:constrict( components, winWidth, winHeight )
    local tw,th = self:getTotalDim( components )
    local startx = self.leftOffset
    local starty = (winHeight - th)/2
    for _,c in pairs( components ) do 
        c:setPosition( startx, starty )
        starty = starty + c.height + self.padding
    end 
end

function LeftPlacer:getTotalDim( components )
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

return LeftPlacer
