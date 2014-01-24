local AABB = {0,0,0,0}

function AABB:new(o)
    o = o or {}
    setmetatable(o,self)
    self.__index = self
    return o
end

function AABB:collides( x,y )
    return (self[1] < x and 
            x < self[1] + self[3] and 
            self[2] < y and 
            y < self[2] + self[4])
end

return AABB
