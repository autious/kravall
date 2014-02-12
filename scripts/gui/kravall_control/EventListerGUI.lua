local GUI = require "gui/GUI"
local U = require "utility"

local EventListerGUI = GUI:new( { eventLifeTime = 20, moveSpeed = 4 } )

function EventListerGUI:new(o)
    GUI.new(self,o)

    o.compData = {} 

    return o
end

-- Disable the default constrict function
-- Add delta value to allow for animations
function EventListerGUI:constrict( delta )
    if delta then 
        local newComponents = {} 
        local prevComponent = nil
        local totYs = {}
        totYs[0] = 0

        local s = 0
        totYs[0] = 0

        for i,v in U.ripairs( self.components ) do
            totYs[s+1] = totYs[s] + v.height
            s=s+1
        end
        
        s = #self.components
        for i,v in ipairs( self.components ) do
            self.compData[v] = self.compData[v] or {}
            local cd = self.compData[v]  --Make shorter
            cd.life = cd.life or 0
            cd.x = cd.x or self.x
            cd.y = cd.y or self.y + self.height --Starting position
            cd.gx = cd.gx or cd.x
            cd.gy = cd.gy or cd.y

            local yoffset = totYs[s] or 0
            s = s - 1

            local nx = self.x
            local ny = self.y + self.height - yoffset

            -- Smooth into new position for Y
            if ny ~= cd.gy then
                cd.gy = ny --goal position
                cd.sy = cd.y --starting pos
                cd.mvPosY = 0  --set movement position to a starting pos
            end

            if cd.mvPosY then
                cd.y = cd.sy + (cd.gy-cd.sy) * U.smoothStep(cd.mvPosY)
                cd.mvPosY = cd.mvPosY + delta * self.moveSpeed
                
                if cd.mvPosY > 1 then
                    cd.y = cd.gy
                    cd.mvPosY = nil -- Reached target, stop movement
                end
            end
            
            -- Always accept x position
            cd.x = nx

            --Set new position
            v:setPosition(cd.x, cd.y) 

            cd.life = cd.life + delta

            if cd.life < self.eventLifeTime and cd.y > self.y then
                newComponents[#newComponents+1] = v 
                prevComponent = v
            else   
                self.compData[v] = nil
                v:destroy()
            end
        end
        self.components = newComponents
    else
         
    end
end

function EventListerGUI:destroy()
    GUI.destroy( self )
end

return EventListerGUI
