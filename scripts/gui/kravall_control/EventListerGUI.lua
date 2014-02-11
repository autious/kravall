local GUI = require "gui/GUI"

local EventListerGUI = GUI:new( {} )

function EventListerGUI:new(o)
    GUI.new(self,o)

    o.compData = {} 

    return o
end

-- Disable the default constrict function
-- Add delta value to allow for animations
function EventListerGUI:constrict( delta )
    if delta then 
        for i,v in pairs( self.components ) do
            self.compData[v] = self.compData[v] or {}
            self.compData[v].life = self.compData[v].life or 0

            local yoffset = self.compData[v].life  * 10

            v:setPosition( self.x, self.y + self.height - v.height - yoffset ) 

            self.compData[v].life = self.compData[v].life + delta
        end
    else
         
    end
end

function EventListerGUI:destroy()
    GUI.destroy( self )
end

return EventListerGUI
