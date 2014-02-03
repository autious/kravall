local ent = require "entities"

return function(asm, posX, posY, posZ, rotation)

    local police = ent.get "police"
    local members = {}
    local groupId = core.system.groups.createGroup()
    asm:loadAssembly(
            {
                {
                    type = core.componentType.SquadComponent,
                    data = {squadID = groupID, 
                            squadGoal = {posX, posY, posZ},                                
                            squadMoveInFormation = false,
                            squadTargetForward = {math.sin(rotation), -math.cos(rotation)}},                        
                        ignoreHard = true
                }
            })

    for i=0, 1 do
        for k=0, 2+i do
            local x = 1.5 * (k-1) - 0.7 * i
            local z = -1.0 * i
                


            local x_p = x*math.cos(rotation) - z*math.sin(rotation)
            local z_p = x*math.sin(rotation) + z*math.cos(rotation)

            members[i*3+k] = police(asm, posX + x_p, 0, posZ + z_p, x_p, z_p, groupId)
        end
    end
end
