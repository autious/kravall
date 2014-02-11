local padding = 10
local PU = require "gui/placement/util"

return function( components, winWidth, winHeight, posx, posy )
    posx = posx or 0
    posy = posy or 0

    local tw,th = PU.getTotalDimHeight( components, padding, padding )
    local startx = winWidth + posx + padding
    local starty = (winHeight - th)/2 + posy + padding
    for _,c in pairs( components ) do 
        c:setPosition( c.xoffset + startx - c.width - 2*padding, starty + c.yoffset )
        starty = starty + c.height + padding
    end 
end
