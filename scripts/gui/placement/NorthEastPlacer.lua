local PU = require "gui/placement/util"
local padding = 10

return function( components, winWidth, winHeight, posx, posy )
    posx = posx or 0
    posy = posy or 0

    local tw,th = PU.getTotalDimHeight( components, padding, padding )
    local startx = winWidth + posx + padding
    local starty = posy + padding
    for _,c in pairs( components ) do 
        c:setPosition( startx - c.width + c.xoffset - padding * 2, starty + c.yoffset )
        starty = starty + c.height + padding
    end 
end
