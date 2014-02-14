local padding = 10
local PU = require "gui/placement/util"

return function( components, winWidth, winHeight, posx, posy )
    posx = posx or 0
    posy = posy or 0

    local tw,th = PU.getTotalDimHeight( components, padding, padding )
    local startx = winWidth/2 + posx
    local starty = (winHeight - th)/2 + posy
    for _,c in pairs( components ) do 
        c:setPosition( startx - c.width/2 + c.xoffset, starty + c.yoffset )
        starty = starty + c.height + padding
    end 
end
