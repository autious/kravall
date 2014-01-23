local AABB = {0,0,0,0}

function AABB:new(o)
    o = o or {}
    setmetatable(o,self)
    self.__index = self
    return o
end

function AABB:collides( x,y )
    return (self.x < x and 
            x < self.x + self.width and 
            self.y < y and 
            y < self.y + self.height)
end

return AABB
