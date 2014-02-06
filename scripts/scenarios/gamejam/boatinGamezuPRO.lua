local entities = require "entities"
local scenario = require "scenario"
local input = require "input" 
local scen = scenario.new()

local mouse = core.input.mouse
local keyboard = core.input.keyboard
local key = keyboard.key
local camera = require "rts_camera".new()

local player
local playerLight
local playerSpotLight
local cave
local Zombies = { }
local group
local treasurePositions
local treasurePos
local treasure

function scen:load()

	scen.asm:specific_content( core.contentmanager.load( 
			core.loaders.NavigationMeshLoader, "scripts/scenarios/gamejam/gameJamu.nav", function( value ) end, false ) )


	local IdForNavMesh = core.system.groups.createGroup()
	player = scen:loadAssembly( 
	{
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = { 0, 0, 0 } }
		},
		{
			type = core.componentType.GraphicsComponent,
			data = { render = true, mesh = 2000, material = 2000, type = core.gfx.objectTypes.OpaqueGeometry },
			load = { 
						mesh = { core.loaders.GnomeLoader, "assets/model/animated/rioter/rioter_00.bgnome" },
						material = { core.loaders.MaterialLoader, "assets/material/gameJam_john/player.material" }
					}
		},
		{
			type = core.componentType.AnimationComponent,
			data = { 
				animationID = 0,
				currentTime = 0.0,
				loop = true,
				playing = true,
				speed = 1.0,
				currentFrame = 0,
				queuedAnimationID = 0
			}
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = 3.0 }
		},
		{
			type = core.componentType.RotationComponent,
			data = { rotation = { 0,0, math.sin( 0 ), math.cos( 0 ) } }
		},
		{
			type = core.componentType.UnitTypeComponent,
			data = { unitType = core.UnitType.Object }
		},
		{
			type = core.componentType.MovementComponent,
			data = { direction = { 1, 0, 0 }, newDirection = { 0, 0, 0 }, speed = 5.8, 
			desiredSpeed = 5.8, goal = { 0, 0, 0 } },
			ignoreHard = true 
		},
		{
			type = core.componentType.AttributeComponent,
			data = { health = 0, stamina = 0, morale = 2.0, 
					stancePolice = core.PoliceStance.Aggressive, defense = 0, mobility = 0, squadID = IdForNavMesh },
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
			type = core.componentType.FlowfieldComponent,
			data = { node = -1 }
		}
	}
	)

	playerLight = scen:loadAssembly( 
	{
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = { 0, 1.5, 0 } }
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = 20.0 }
		},
		{
			type = core.componentType.RotationComponent,
			data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
		},
		{
			type = core.componentType.LightComponent,
			data =  { 
				color = { 1.0, 1.0, 1.0 },
				intensity = 1.0,
				type = core.gfx.objectTypes.Light,
				lighttype = core.gfx.lightTypes.Point,
				spotangle = 0,
				spotpenumbra = 0,
				speccolor = {0,0,0}
			}
		}
	}
	)

	playerSpotLight = scen:loadAssembly( 
	{
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = { 0, 1, 0 } }
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = 20.0 }
		},
		{
			type = core.componentType.RotationComponent,
			data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
		},
		{
			type = core.componentType.LightComponent,
			data =  { 
				color = { 1.0, 1.0, 1.0 },
				speccolor = {0,0,0},
				intensity = 3.0,
				spotangle = 3.14/4.0,
				spotpenumbra = 0.21,
				type = core.gfx.objectTypes.Light,
				lighttype = core.gfx.lightTypes.Spot
			}
		}
	}
	)




	-- cave mesh
	cave = scen:loadAssembly( 
	{
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = { -6.4, 0, -5.7 } }
		},
		{
			type = core.componentType.GraphicsComponent,
			data = { render = true, mesh = 2000, material = 2000, type = core.gfx.objectTypes.OpaqueGeometry },
			load = { 
						mesh = { core.loaders.GnomeLoader, "assets/material/gameJam_john/gameJam_cave_ty_johan.bgnome" },
						material = { core.loaders.MaterialLoader, "assets/material/gameJam_john/cave.material" }
					}
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = 1.0 }
		},
		{
			type = core.componentType.RotationComponent,
			--data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
			data = { rotation = { -math.sin(math.pi/4.0), 0, 0, math.cos(math.pi/4.0) } }
		}
	}
	)


	---- Ambient light
	scen:loadAssembly( 
	{
		{
			type = core.componentType.LightComponent,
			data =  { 
						color = { 1.0, 1.0, 1.0 },
						intensity = 0.0,
						type = core.gfx.objectTypes.Light,
						lighttype = core.gfx.lightTypes.Ambient,
						spotangle = 0,
						spotpenumbra = 0,
						speccolor = {0,0,0}
					}
		},
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = { 0, 0, 0 } }
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = 1.0 }
		},
		{
			type = core.componentType.RotationComponent,
			data = { rotation = { 0,0,0,0 } }
		}
	} 
	)

	-- zombies

	group = core.system.groups.createGroup()

	local posX = -3
	local posZ = 75

	for i = 0, 100 do
		Zombies[ #Zombies + 1 ] = scen:loadAssembly( 
		{
			{
				type = core.componentType.GraphicsComponent,
				data = { render = true, mesh = 2000, material = 2000, type = core.gfx.objectTypes.OpaqueGeometry },
				load = { 
							mesh = { core.loaders.GnomeLoader, "assets/model/animated/rioter/rioter_00.bgnome" },
							material = { core.loaders.MaterialLoader, "assets/material/gameJam_john/player.material" }
						}
			},
			{
				type = core.componentType.AnimationComponent,
				data = { 
					animationID = 0,
					currentTime = math.random( 2 ),
					loop = true,
					playing = true,
					speed = 1.0,
					currentFrame = 0,
					queuedAnimationID = 0
				}
			},
			{
				type = core.componentType.WorldPositionComponent,
				data = { position = { posX, 0, posZ} }
			},
			{
				type = core.componentType.RotationComponent,
				data = { rotation = { 0, 0, 0, 0 } }
			},
			{
				type = core.componentType.ScaleComponent,
				data = { scale = 3.0 }
			},
			{
				type = core.componentType.UnitTypeComponent,
				data = { unitType = core.UnitType.Rioter }
			},
			{
				type = core.componentType.MovementComponent,
				data = { direction = { 0, 0, 0 }, newDirection = { 0, 0, 0 }, speed = 1.5, 
				desiredSpeed = 3.5, goal = false }
				,ignoreHard = true
			},
			{
				type = core.componentType.AttributeComponent,
				data = { health = 100, stamina = 100, morale = 2.0, 
						alignment = core.RioterAlignment.Anarchist, rage = 0, pressure = 0, groupID = group, stanceRioter = core.RioterStance.Normal}
				,
				ignoreHard = true
			},
			{
				type = core.componentType.TargetingComponent,
				data = { },
				ignoreHard = true
			},
			{
				type = core.componentType.BoundingVolumeComponent,
				data = { sphereOffset = { 0, 0, 0 }, sphereRadius = 0.3, 
						collisionModel = core.BoundingVolumeCollisionModel.DynamicResolution, 
						type = core.BoundingVolumeType.SphereBoundingType }
			},
			{
				type = core.componentType.FlowfieldComponent,
				data = { node = -1 }
			}
				
		}
		)

	end



	-- treasure
	treasurePositions = { { -20, -16 }, { 8, 44 }, { 47, 5 }, { 53, 26 }, { 42, 80 }, { 25, 65 }, { 53, 41 } }
	treasurePos = treasurePositions[ math.random( #treasurePositions ) ]
	treasure = scen:loadAssembly( 
	{
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = { treasurePos[1], 0, treasurePos[2] } }
		},
		{
			type = core.componentType.GraphicsComponent,
			data = { render = true, mesh = 2000, material = 2000, type = core.gfx.objectTypes.OpaqueGeometry },
			load = { 
						mesh = { core.loaders.GnomeLoader, "assets/material/gameJam_john/minecart.bgnome" },
						material = { core.loaders.MaterialLoader, "assets/material/gameJam_john/cart.material" }
					}
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = 1.0 }
		},
		{
			type = core.componentType.RotationComponent,
			data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
		}
	}
	)

end -- function load


-- utility variables...
local playerGotEatenCondition = false
local playerFoundTheTreasure = false
local enterWasDown = true
local isCurrentlyWalking = false;


function move( dt, entity, playerGotEaten )
	local playerPos = entity:get(core.componentType.WorldPositionComponent)
	local playerMovement = entity:get(core.componentType.MovementComponent)
	local playerDirection = playerMovement.direction
	local playerPosVector = core.glm.vec3.new( playerPos.position[1], playerPos.position[2], playerPos.position[3] )


	if core.input.mouse.isButtonDown(core.input.mouse.button.Right) and playerGotEaten == false then
        
        local mouseX, mouseY = core.input.mouse.getPosition()
        local groundX, groundY, groundZ = core.system.picking.getGroundHit(mouseX, mouseY);
		local normalizedDir = core.glm.vec3.normalize( core.glm.vec3.subtract( core.glm.vec3.new( groundX, groundY, groundZ ), playerPosVector ) )
		local newDirX, newDirY, newDirZ = core.glm.vec3.get( normalizedDir )

        playerMovement.newDirection = { newDirX, newDirY, newDirZ }
		playerMovement.desiredSpeed = 5.8			
		isCurrentlyWalking = true

	else

		playerMovement.desiredSpeed = playerMovement.desiredSpeed - dt * 15.0
		if playerMovement.desiredSpeed < 0 then
			isCurrentlyWalking = false
			playerMovement.desiredSpeed = 0
		end

    end

	entity:set( core.componentType.MovementComponent, playerMovement )
end


function CopyXZPos( parent, child )

	local parentPos = parent:get(core.componentType.WorldPositionComponent)
	local childPos = child:get(core.componentType.WorldPositionComponent)

	childPos.position[1] = parentPos.position[1]
	childPos.position[3] = parentPos.position[3]

	child:set( core.componentType.WorldPositionComponent, childPos )

 end

 function UpdateSpotLight( parent, light )

	-- calc player angle
	local parentMovement = parent:get(core.componentType.MovementComponent)
	local parentDir = parentMovement.direction

	if parentDir[1] + parentDir[2] + parentDir[3] ~= 0 then
		local normalizedDir = core.glm.vec3.normalize( core.glm.vec3.new( parentDir[1], parentDir[2], parentDir[3] ))
		local xDir, yDir, zDir = core.glm.vec3.get( normalizedDir )
		
		local playerDirection = math.acos( xDir )

		if zDir < 0 then 
			playerDirection = -playerDirection
		end

		local rotComp = light:get( core.componentType.RotationComponent )
		
		rotComp.rotation[1] = math.cos( playerDirection )
		rotComp.rotation[2] = 0		    			    
		rotComp.rotation[3] = math.sin( playerDirection )
		rotComp.rotation[4] = 0
		light:set( core.componentType.RotationComponent, rotComp )
	end
	
 end



 function updateLight( parent, child )
	
	local parentPos = parent:get(core.componentType.WorldPositionComponent)
	local parentMovement = parent:get(core.componentType.MovementComponent)
	local parentDir = parentMovement.direction
	
	if parentDir[1] + parentDir[2] + parentDir[3] ~= 0 then

		local normalizedDir = core.glm.vec3.normalize( core.glm.vec3.new( parentDir[1], parentDir[2], parentDir[3] ))
		local xDir, yDir, zDir = core.glm.vec3.get( normalizedDir )
		
		local offset = 1.5
		local childPos = child:get(core.componentType.WorldPositionComponent)

		

		childPos.position[1] = parentPos.position[1] + xDir * offset
		childPos.position[3] = parentPos.position[3] + zDir * offset

		child:set( core.componentType.WorldPositionComponent, childPos )


	end
 end

 function UpdateZombies( dudeToFollow )
	
	local parentPos = dudeToFollow:get(core.componentType.WorldPositionComponent)
	core.system.groups.setGroupGoal( group, parentPos.position[1], 0, parentPos.position[3] )

 end

 

 function CheckIfEaten( dudeToEat )
	
	local playerPos = dudeToEat:get(core.componentType.WorldPositionComponent)
	local playerPosVector = core.glm.vec3.new( playerPos.position[1], playerPos.position[2], playerPos.position[3] )

	for i = 1, #Zombies do
		local zombiePos = Zombies[i]:get(core.componentType.WorldPositionComponent)
		local zombiePosVector = core.glm.vec3.new( zombiePos.position[1], zombiePos.position[2], zombiePos.position[3] )
		
		local distance = core.glm.vec3.length( core.glm.vec3.subtract( zombiePosVector, playerPosVector ) )
		if distance < 1 then
			playerGotEatenCondition = true
		end
	end
 end



 function CheckIfFoundTreasure( TheBraveExplorer )

	local playerPos = TheBraveExplorer:get(core.componentType.WorldPositionComponent)
	local playerPosVector = core.glm.vec3.new( playerPos.position[1], playerPos.position[2], playerPos.position[3] )

	local treasurePos = treasure:get(core.componentType.WorldPositionComponent)
	local treasurePosVector = core.glm.vec3.new( treasurePos.position[1], treasurePos.position[2], treasurePos.position[3] )

	local distance = core.glm.vec3.length( core.glm.vec3.subtract( treasurePosVector, playerPosVector ) )
	if distance < 1 then
		playerFoundTheTreasure = true
	end

 end
 
 
 function HandleCamera( dudeToFollow )

	local parentPos = dudeToFollow:get(core.componentType.WorldPositionComponent)

	camera:lookAt( core.glm.vec3.new( parentPos.position[1], 20, parentPos.position[3] ), core.glm.vec3.new( parentPos.position[1], 0, parentPos.position[3] ) )
	core.camera.gameCamera:setView( camera:getView( ) )
	core.camera.gameCamera:setProjection( camera:getProjection( ) )
	
 end

 function HandlePlayerAnimation( dude )
	
	local animation = dude:get(core.componentType.AnimationComponent)

	--animation.playing = isCurrentlyWalking
	if isCurrentlyWalking == false then
		animation.currentTime = 0.33
	end

	dude:set( core.componentType.AnimationComponent, animation )

 end


function Update(dt)
	
	local enterIsDown = false
	if keyboard.isKeyDown( key.Enter ) then
		if not enterWasDown then
			enterIsDown = true
		end
		enterWasDown = true
	else
		enterWasDown = false
	end


	local gameFinished = false
	if playerGotEatenCondition == true or playerFoundTheTreasure == true then
		move( dt, player, true )
	else
		move( dt, player, false )
	end

	updateLight( player, playerLight )
	CopyXZPos( player, playerSpotLight )
	UpdateSpotLight( player, playerSpotLight )
	HandleCamera( player )
	HandlePlayerAnimation( player )

	
	if playerGotEatenCondition == true then

		core.draw.drawText( 405, 340, "Can't handle the ice ppl can't handle the real world!" )
		core.draw.drawText( 580, 360, "You just died!" )
		core.draw.drawText( 590, 400, "PRESS ENTER!" )

		if enterIsDown == true then
			openscenario( "gamejam/boatinGamezuPRO"  )
		end

	elseif playerFoundTheTreasure == true then
		
		core.draw.drawText( 490, 340, "Hurray! The treasure has been found!" )
		core.draw.drawText( 545, 360, "You are a good person!" )
		core.draw.drawText( 590, 400, "PRESS ENTER!" )

		if enterIsDown == true then
			openscenario( "gamejam/boatinGamezuPRO"  )
		end

	else
		UpdateZombies( player )
		CheckIfEaten( player )
		CheckIfFoundTreasure( player )
	end
end
  




 
scen:registerUpdateCallback( Update )

return scen;