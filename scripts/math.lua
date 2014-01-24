local M = {}

function M.rayXZPlaneIntersect( source, direction )
    local up = vec3.new(0,1,0)
    local d = ((-source).dot(up))/(direction.dot(up))
    return source + direction*d;
end

return M
