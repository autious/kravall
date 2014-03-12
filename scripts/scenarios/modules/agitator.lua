local entity = require "entities"
local group = entity.get "group"
local vec4 = require( "utility" ).expandMixxedHexToVec4

return function( scen, T )
    local agitatorGroups = {}
    local fists = core.weaponData.pushWeapon( 1.0, 0.75, 10, 0.05, 0.01, 3.2, 2.9, 0.05, 0.5, "punch" )
    local initialSize = {10,5}

    scen:registerTickCallback( function()
        if T.getDeserterGroup then
            if T.getDeserterGroup() then
                for i,v in  pairs( agitatorGroups ) do
                    core.system.groups.setGroupGoal( v, core.system.groups.getGroupMedianPosition(T.getDeserterGroup()))
                end     
            end
        else
            core.log.error("missing the deserter module")
        end
    end)

    function T.getAgitatorGroups()
        return agitatorGroups
    end

    -- Create rioter on area:
    function T.createAgitators( ents, xsize, ysize )

        local grp = core.system.groups.createGroup(2)

        for i,ent in pairs( ents ) do
            local wpc = ent:get( core.componentType.WorldPositionComponent )
            local ac = ent:get( core.componentType.AreaComponent )
            verts = ac.vertices

            -- Make vertex positions from local space to world space
            for i = 1, 8, 2 do
                verts[i] = verts[i] + wpc.position[1]
                verts[i + 1] = verts[i + 1] + wpc.position[3]
            end
            group( scen, ac.vertices, grp, {xsize, ysize}, fists, nil, 1, 1, core.RioterAlignment.Anarchist, core.movementData.Jogging )
        end

        return grp
    end

    function T.spawnAgitatorsOnMain( ent )
        scen.gamemode:registerOnStateChange( function( stateName )
            if stateName == "Main" then
                table.insert( agitatorGroups, T.createAgitators( {ent},unpack(initialSize) ) )
                return false -- return false to indicate that we have served our purpose and wish no longer to be called.
            end
            return true
        end)
    end

    for i = 1,9 do
        T["registerAg"..i.."Spawn"] = function(ent) 
            local wpc = ent:get(core.componentType.WorldPositionComponent)
            print( "Registering Ag"..i )

            local groupId = core.gameMetaData.registerEscapePoint( wpc.position[1], wpc.position[2], wpc.position[3] )

            scen:registerTickCallback(function()
                if ent:isValid() then
                    local rioters = core.system.area.getAreaRioters(ent)

                    for _,v in pairs( rioters ) do
                        local alignmentComponent = v:get( core.componentType.AttributeComponent )
                        print( alignmentComponent.groupID )
                        if alignmentComponent.groupID == groupId then
                            scen.asm:destroyEntity( v )
                        end  
                    end
                end    
            end)

            if T["ag"..i.."spawn"] == nil then
                T["ag"..i.."spawn"] = {}
            end

            table.insert( T["ag"..i.."spawn"], ent )
        end

        T["checkAg"..i.. "Enter"] = function( ent )
            local spawns = T["ag"..i.."spawn"]

            if #spawns > 0 and core.system.area.getAreaRioterCount( ent, deserterGroup ) > 0 then
                table.insert( agitatorGroups, T.createAgitators( spawns, 10,10 ) ) 
                T["ag"..i.."spawn"] = {}
            end
        end

        for k = 1,9 do
            T["checkAg"..i.."And"..k.."Enter"] = function( ent )
                T["checkAg"..i.."Enter"](ent)
                T["checkAg"..k.."Enter"](ent)
            end
        end
    end
end
