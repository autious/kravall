local PU = require "gui/placement/util"
local leftOffset = 20
local padding = 10

return function( components, winWidth, winHeight, posx, posy )
    posx = posx or 0
    posy = posy or 0

    local tw,th = PU.getTotalDimHeight( components, padding, padding )
    local startx = winWidth/2
    local starty = winHeight - th
    for _,c in pairs( components ) do 
        c:setPosition( startx - c.width/2 + c.xoffset + posx, starty + c.yoffset + posy )
        starty = starty + c.height + padding
    end 
end
