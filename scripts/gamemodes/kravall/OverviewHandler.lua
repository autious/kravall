local OverviewHandler = {
                            onEnterOverview = function()
                                core.log.error("No function set for onEnterOverview in OverviewHandler")
                            end,
                            onExitOverview = function()
                                core.log.error("No function set for onExitOverview in OverviewHandler")
                            end,
}


local keyboard = core.input.keyboard
local mouse = core.input.mouse

function OverviewHandler:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self

    o.inOverview = false

    return o
end


function OverviewHandler:EnterOverview()
    self.inOverview = true
    self.onEnterOverview()
end

function OverviewHandler:ExitOverview(pos)
    self.inOverview = false
    self.onExitOverview(pos)
end

function OverviewHandler:update(delta)

    if not self.inOverview then
        if keyboard.isKeyDownOnce(keyboard.key.G) then
            self:EnterOverview()        
        end
    else
        if mouse.isButtonDownOnce(mouse.button.Left) then
            local mouseX, mouseY = mouse.getPosition()
            local x,y,z = core.system.picking.getGroundHit(mouseX, mouseY);
            self:ExitOverview(core.glm.vec3.new(x,y,z))
        end
    end
end

return OverviewHandler
