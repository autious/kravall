local ent = require "entities"

return function(asm, posX, posY, posZ, rotation, weaponType)

    local police = ent.get "police"
    local members = {}
    local groupId = core.system.groups.createGroup()
	print(groupId)
    asm:loadAssembly(
            {
                {
                    type = core.componentType.SquadComponent,
                    data = {squadID = groupId, 
                            squadGoal = {posX, posY, posZ},
                            squadMoveInFormation = false,
                            squadTargetForward = {math.sin(rotation), -math.cos(rotation)}},                        
                        ignoreHard = true
                }
            })
	
	-- only create one police per squad
	-- local x_p = math.cos(rotation) - math.sin(rotation)
	-- local z_p = math.sin(rotation) + math.cos(rotation)
	-- members[0] = police(asm, posX, 0, posZ, x_p, z_p, groupId)
    
	for i=0, 1 do
        for k=0, 2+i do
            local x = 1.6 * (k-1) - 0.9 * i
            local z = 1.4 * i            
    
            local x_p = x*math.cos(rotation) - z*math.sin(rotation)
            local z_p = x*math.sin(rotation) + z*math.cos(rotation)
    
            members[i*3+k] = police(asm, posX + x_p, 0, posZ + z_p, x_p, z_p, groupId, weaponType)
        end
    end
end
