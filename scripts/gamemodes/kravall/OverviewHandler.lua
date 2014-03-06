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

    o.outlinedGroups = {}
    o.inOverview = false

    return o
end

function OverviewHandler:EnterOverview()
    self.inOverview = true
    self.onEnterOverview()
end

function OverviewHandler:ExitOverview(pos)
    self.inOverview = false
    self:RemoveOutlines()
    self.onExitOverview(pos)
end

function OverviewHandler:SetOutlines()
    local groupCount = core.system.groups.getNumberOfGroups()
    
    for i=0, groupCount-1 do 
        local members = core.system.groups.getMembersInGroup(i)
        
        for _,v in pairs(members) do
            local attrbComponent = v:get(core.componentType.AttributeComponent)
            local utc = v:get(core.componentType.UnitTypeComponent)
            if utc.unitType == core.UnitType.Rioter then
                self.outlinedGroups[#self.outlinedGroups] = attrbComponent.groupID
                core.system.squad.enableMoodOutline({attrbComponent.groupID})
            elseif utc.unitType == core.UnitType.Police then
                core.system.squad.enableOutline({attrbComponent.squadID}, 1, 1, 1, 1)
                self.outlinedGroups[#self.outlinedGroups] = attrbComponent.squadID    
            end
        end
    end
end

function OverviewHandler:RemoveOutlines()
    core.system.squad.disableOutline(self.outlinedGroups)
end

function OverviewHandler:update(delta)

    if not self.inOverview then
        if keyboard.isKeyDownOnce(keyboard.key.G) then
            self:EnterOverview()        
        end
    else
        self:SetOutlines() 
        if mouse.isButtonDownOnce(mouse.button.Left) then
            local mouseX, mouseY = mouse.getPosition()
            local x,y,z = core.system.picking.getGroundHit(mouseX, mouseY);
            self:ExitOverview(core.glm.vec3.new(x,y,z))
        end
    end
end

return OverviewHandler
