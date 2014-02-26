local ent = require "entities"

local function createMember( scen, pos, groupID, weaponType, memberDef )

    assert( weaponType )
    assert( pos )
    assert( groupID, "groupID is nil" )
    assert( memberDef )

    local T = {}
    
    function T.getAbilities()
        return memberDef.abilities
    end    
    local formationOffsetX,_, formationOffsetZ = unpack( memberDef.positionOffset )

    T.entity = scen:loadAssembly( 
		{
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { pos:get() } }
			},
			{
				type = core.componentType.GraphicsComponent,
				data = { render = true, mesh = 2000, material = 2000, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 1, 0, 1} },
				load = { 
							mesh = { core.loaders.GnomeLoader, memberDef.mesh },
							material = { core.loaders.MaterialLoader, memberDef.material }
					   }
			},
			{
				type = core.componentType.ScaleComponent,
				data = { scale = 1.0 }
			},
			{
				type = core.componentType.RotationComponent,
				data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
			},
			{
				type = core.componentType.UnitTypeComponent,
				data = { unitType = core.UnitType.Police }
			},
			{
				type = core.componentType.MovementComponent,
				data = { direction = { 0, 0, 0 }, newDirection = { 0, 0, 0 }, speed = 5.8, 
			    desiredSpeed = 5.8, goal = false },
                ignoreHard = true 
			},
			{
				type = core.componentType.AttributeComponent,
				data = 
                    { 
                        health = memberDef.health,
                        stamina = memberDef.stamina,
                        morale = memberDef.morale,
                        stancePolice = core.PoliceStance.Passive,
                        statePolice = core.PoliceState.Attacking, 
                        defense = 0, 
                        mobility = 0, 
                        squadID = groupID 
                    },

                ignoreHard = true
			},
			{
				type = core.componentType.TargetingComponent,
				data = { weapon = weaponType or -1 },
				ignoreHard = true
			},
			{
				type = core.componentType.BoundingVolumeComponent,
				data = { sphereOffset = { 0, 0, 0 }, sphereRadius = 0.7, 
						collisionModel = core.BoundingVolumeCollisionModel.DynamicResolution, 
						type = core.BoundingVolumeType.SphereBoundingType }
			},
			{
				type = core.componentType.AnimationComponent,
				data = { 
					animationID = 0,
					currentTime = 0.0,
					loop = false,
					playing = false,
					speed = 1.6,
					currentFrame = 0,
					queuedAnimationID = 0
				}
			},
			{
				type = core.componentType.MovementDataComponent,
				data = { prevDt = 1.0 },
				ignoreHard = true
			},
			{
				type = core.componentType.FlowfieldComponent,
				data = { node = -1 }
			},
            {
                type = core.componentType.FormationComponent,
                data = { relativePosition = { formationOffsetX, formationOffsetZ } }
            }
		}
	)

    return T
end

return function( scen, squadInstance, weaponList )

    local T = {}

    T.members = {}
    T.groupId = core.system.groups.createGroup()

    scen:loadAssembly(
            {
                {
                    type = core.componentType.SquadComponent,
                    data = {squadID = T.groupId, 
                            squadGoal = {squadInstance.position:get()},
                            squadMoveInFormation = false,
                            squadTargetForward = {0, -1}},                        
                        ignoreHard = true
                }
            })
	
    for i,v in pairs( squadInstance.squadDef.setup ) do
        table.insert( T.members, createMember(scen, squadInstance.position + core.glm.vec3.new( unpack( v.positionOffset ) ), T.groupId, weaponList[v.weapon], v ))
    end

    return T
end
