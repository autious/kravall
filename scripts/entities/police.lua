local standardPolice = (require "game_constants").standardPolice

return function(asm, posX, posY, posZ, formationOffsetX, formationOffsetZ, groupID, weaponType)

    assert(groupID, "groupID is nil")

	local meshes = {"assets/model/animated/rioter/rioter-male_00.bgnome", "assets/model/animated/rioter/rioter-female_00.bgnome" }
    local gender = math.random(1, #meshes)

    local T = {}
    
    function T.getAbilities()
        local abilities = core.system.squad.abilities
        return {abilities.Attack, abilities.ArrestGroup, abilities.Sprints, abilities.Sprint, abilities.Rout }
    end    

    T.entity = asm:loadAssembly( 
		{
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { posX, posY, posZ } }
			},
			{
				type = core.componentType.GraphicsComponent,
				data = { render = true, mesh = 2000, material = 2000, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 1, 0, 1} },
				load = { 
							mesh = { core.loaders.GnomeLoader, meshes[gender] },
							material = { core.loaders.MaterialLoader, "assets/texture/animated/police/cop/cop-light_00.material" }
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
                        health = standardPolice.maxHealth, 
                        stamina = standardPolice.maxStamina, 
                        morale = standardPolice.maxMorale, 
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
