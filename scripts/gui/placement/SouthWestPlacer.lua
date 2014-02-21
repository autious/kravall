local PU = require "gui/placement/util"
local padding = 10

return function ( components, winWidth, winHeight, posx, posy )
    posx = posx or 0
    posy = posy or 0
    
    local tw,th = PU.getTotalDimHeight( components, padding, padding )
    local startx = posx
    local starty = posy + winHeight - th
    for _,c in pairs( components ) do 
        if c.ignoreConstrict then
            c:setPosition( posx + c.xoffset, posy + c.yoffset )
        else
            c:setPosition( startx + c.xoffset, starty + c.yoffset )
        end
        starty = starty + c.height + padding
    end 
end
