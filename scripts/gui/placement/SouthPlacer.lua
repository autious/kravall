local PU = require "gui/placement/util"
local padding = 10

return function( components, winWidth, winHeight, posx, posy )
    posx = posx or 0
    posy = posy or 0

    local tw,th = PU.getTotalDimHeight( components, padding, padding )
    local startx = winWidth/2 + posx
    local starty = winHeight - th + posy
    for _,c in pairs( components ) do 
        if c.ignoreConstrict then
            c:setPosition( posx + c.xoffset, posy + c.yoffset )
        else
            c:setPosition( startx - c.width/2 + c.xoffset, starty + c.yoffset )
        end
        starty = starty + c.height + padding
    end
end
