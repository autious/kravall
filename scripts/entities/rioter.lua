return function(asm, posX, posY, posZ, group, gender, material, weaponType, outlineColor, rage, pressure, alignment)
    rage = rage or 1
    pressure = pressure or 1
    alignment = alignment or core.RioterAlignment.Anarchist
    
    if alignment == core.RioterAlignment.Anarchist then 
       -- print( "ALIGNMENT: anarchist" )
    elseif alignment == core.RioterAlignment.Pacifist then
       -- print( "ALIGNMENT: pacifist" )
    end

    local meshes = {"assets/model/animated/rioter/rioter-male_00.bgnome","assets/model/animated/rioter/rioter-female_00.bgnome" }
    local materials = {"assets/texture/animated/rioter/rioter_00.material","assets/texture/animated/rioter/rioter_01.material","assets/texture/animated/rioter/rioter_02.material" }
    gender = gender or (math.random(1,#meshes))
    material = material or (math.random(1,#materials))

    local base = 
       {
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { posX, posY, posZ} }
			},
			{
				type = core.componentType.RotationComponent,
				data = { rotation = { 0, 0, 0, 0 } }
			},
			{
				type = core.componentType.ScaleComponent,
				data = { scale = 1.0 }
			},
			{
				type = core.componentType.UnitTypeComponent,
				data = { unitType = core.UnitType.Rioter }
			},
			{
				type = core.componentType.MovementComponent,
				data = { direction = { 0, 0, 0 }, newDirection = { 0, 0, 0 }, speed = 1.5, 
				desiredSpeed = 1.5, goal = false }
                ,ignoreHard = true
			},
			{
				type = core.componentType.AttributeComponent,
				data = { health = 100, stamina = 100, morale = 2.0, 
					   alignment = alignment, 
                        rage = rage, 
                        pressure = pressure, 
                        groupID = group, stanceRioter = core.RioterStance.Normal}
                ,
                ignoreHard = true
			},
			{
				type = core.componentType.TargetingComponent,
				data = { weapon = weaponType or -1 },
				ignoreHard = true
			},
			{
				type = core.componentType.BoundingVolumeComponent,
				data = { sphereOffset = { 0, 0, 0 }, sphereRadius = 0.3, pickingRadius = 1.0, 
						collisionModel = core.BoundingVolumeCollisionModel.DynamicResolution, 
						type = core.BoundingVolumeType.SphereBoundingType }
			},
			{
				type = core.componentType.AnimationComponent,
				data = { 
					animationID = 0,
					--currentTime = 0.0,
					currentTime = math.random() * 10,
					loop = true,
					playing = true,
					speed = 1.6, -- this is irrelevant in kravall mode, see movementAnimationSystem.
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
			}
        }

        local meshType
        if outlineColor then
            meshType = core.gfx.layerTypes.OutlineLayer
        else
            meshType = core.gfx.layerTypes.MeshLayer
            outlineColor = {0, 0, 1, 1}
        end

        base[#base+1] =
			{
				type = core.componentType.GraphicsComponent,
				data = { render = true, mesh = 2000, material = 2000, type = core.gfx.objectTypes.OpaqueGeometry, layer = meshType, outlineColor = outlineColor },
				load = { 
							mesh = { core.loaders.GnomeLoader, meshes[gender] },
							material = { core.loaders.MaterialLoader, materials[material]  },
					   }
			}

    return asm:loadAssembly( base )
end
