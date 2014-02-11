local PU = require "gui/placement/util"
local padding = 10

return function ( components, winWidth, winHeight, posx, posy )
    posx = posx or 0
    posy = posy or 0
    
    local tw,th = PU.getTotalDimHeight( components, padding, padding )
    local startx = 0
    local starty = winHeight - th
    for _,c in pairs( components ) do 
        c:setPosition( startx + c.xoffset + padding + posx, starty + c.yoffset + posy )
        starty = starty + c.height + padding
    end 
end
