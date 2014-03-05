local padding = 10
local PU = require "gui/placement/util"

return function( components, winWidth, winHeight, posx, posy )
    posx = posx or 0
    posy = posy or 0

    local tw,th = PU.getTotalDimWidth( components, padding, padding )
    local startx = posx
    local starty = (winHeight - th)/2 + posy
    for _,c in pairs( components ) do 
        if c.ignoreConstrict then
            c:setPosition( posx + c.xoffset, posy + c.yoffset )
        else
            c:setPosition( c.xoffset + startx, starty + c.yoffset )
        end
        startx = startx + c.width + padding
    end 
end
