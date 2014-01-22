local AABB = {}

function AABB.new( x, y, width, height )
    self = {}
    self.x = x
    self.y = y
    self.width = width
    self.height = height
end

function AABB:collides( x,y )
    return (self.x < x and 
            x < self.x + self.width and 
            self.y < y and 
            y < self.y + self.height)
end

return AABB
