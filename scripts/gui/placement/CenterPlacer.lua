local CenterPlacer = { padding = 10 }

function CenterPlacer:new(o)
    o = o or {}

    setmetatable( o, self )
    self.__index = self

    return o
end

function CenterPlacer:constrict( components, winWidth, winHeight )
    local tw,th = self:getTotalDim( components )
    local startx = winWidth/2
    local starty = (winHeight - th)/2
    for _,c in pairs( components ) do 
        c:setPosition( startx - c.width/2 + c.xoffset, starty + c.yoffset )
        starty = starty + c.height + self.padding
    end 
end

function CenterPlacer:getTotalDim( components )
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

return CenterPlacer
