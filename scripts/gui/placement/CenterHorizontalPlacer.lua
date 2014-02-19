local padding = 10
local PU = require "gui/placement/util"

return function( components, winWidth, winHeight, posx, posy )
    posx = posx or 0
    posy = posy or 0

    local tw,th = PU.getTotalDimWidth( components, padding, padding )
    local startx = (winWidth-tw)/2 + posx
    local starty = winHeight/2 + posy
    for _,c in pairs( components ) do 
        c:setPosition( startx + c.xoffset, starty - c.height/2 + c.yoffset )
        startx = startx + c.width + padding
    end 
end
