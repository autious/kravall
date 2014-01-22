local M = {}

function m.rayXZPlaneIntersect( source, direction )

    local up = vec3.new(0,1,0)
    local theD = ((-source).dot(up))/(direction.dot(up))

    return source + direction*theD;
end

