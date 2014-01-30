local CenterPlacer = {}

function CenterPlacer:new(o)
    o = o or {}

    setmetatable( o, self )
    self.__index = self

    return o
end

function CenterPlacer:constrict( components, winWidth, winHeight )
    local tw,th = self:getTotalDim( components )
    local startx = (winWidth - tw)
    local starty = (winHeight - th)
    for _,c in pairs( components ) do 
        print( startx," ", starty )
        c:setPosition( startx, starty )
        starty = starty + c.height
    end 
end

function CenterPlacer:getTotalDim( components )
    local tw = 0
    local th = 0
    for _,c in pairs( components ) do     
        if tw < c.width then
            tw = c.width
        end
        th = th + c.height 
    end
    
    return tw, th
end

return CenterPlacer
