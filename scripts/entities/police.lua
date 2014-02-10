return function(asm, posX, posY, posZ, formationOffsetX, formationOffsetZ, groupID)
	asm:loadAssembly( 
		{
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { posX, posY, posZ } }
			},
			{
				type = core.componentType.GraphicsComponent,
				data = { render = true, mesh = 2000, material = 2000, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 1, 0, 1} },
				load = { 
							mesh = { core.loaders.GnomeLoader, "assets/model/animated/rioter/rioter_00.bgnome" },
							material = { core.loaders.MaterialLoader, "assets/material/cop.material" }
					   }
			},
			{
				type = core.componentType.ScaleComponent,
				data = { scale = 3.0 }
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
				data = { health = 100, stamina = 0, morale = 2.0, 
					   stancePolice = core.PoliceStance.Aggressive,
					   statePolice = core.PoliceState.Attacking, 
					   defense = 0, mobility = 0, squadID = groupID },
                ignoreHard = true
			},
			{
				type = core.componentType.TargetingComponent,
				data = { },
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
					loop = true,
					playing = true,
					speed = 1.6,
					currentFrame = 0,
					queuedAnimationID = 0
				}
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
end
