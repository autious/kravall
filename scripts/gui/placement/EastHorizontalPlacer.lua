local padding = 10
local PU = require "gui/placement/util"

return function( components, winWidth, winHeight, posx, posy )
    posx = posx or 0
    posy = posy or 0

    local tw,th = PU.getTotalDimWidth( components, padding, padding )
    local startx = winWidth + posx - tw
    local starty = (winHeight - th)/2 + posy
    for _,c in pairs( components ) do 
        c:setPosition( c.xoffset + startx, starty + c.yoffset )
        startx = startx + c.width + padding
    end 
end
