--local function CreateEnemyTankBody(position, group)
--	return scen:loadAssembly( 
--	{
--	{
--		type = core.componentType.WorldPositionComponent,
--		data = { position = { position[1], 0, position[2] } }
--	},
--	--{
--	--	type = core.componentType.UnitTypeComponent,
--	--	data = { unitType = core.UnitType.Rioter }
--	--},
--	--{
--	--	type = core.componentType.MovementComponent,
--	--	data = { direction = { 0, 0, 0 }, newDirection = { 0, 0, 0 }, speed = 5.0, 
--	--	desiredSpeed = { 5.0, 5.0, 5.0, 5.0 }, goal = false }
--	--	,ignoreHard = true
--	--},
--	--{
--	--	type = core.componentType.BoundingVolumeComponent,
--	--	data = { sphereOffset = { 0, 0, 0 }, sphereRadius = 3.0, pickingRadius = 0.0,
--	--			collisionModel = core.BoundingVolumeCollisionModel.DynamicResolution, 
--	--			type = core.BoundingVolumeType.SphereBoundingType }
--	--},
--	{
--		type = core.componentType.GraphicsComponent,
--		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
--		load = { 
--					mesh = { core.loaders.GnomeLoader, "assets/model/tankwars/leopard-body.bgnome", false },
--					material = { core.loaders.MaterialLoader, "assets/texture/tankwars/leopard-body.material", false }
--			   }
--	},
--	--{
--	--	type = core.componentType.AttributeComponent,
--	--	data = { health = 100, stamina = 100, morale = 2.0, 
--	--		   alignment = core.RioterAlignment.Anarchist, rage = 0, pressure = 0, groupID = group, stanceRioter = core.RioterStance.Normal}
--	--	,
--	--	ignoreHard = true
--	--},
--	--{
--	--	type = core.componentType.TargetingComponent,
--	--	data = { weapon = -1 },
--	--	ignoreHard = true
--	--},
--	{
--		type = core.componentType.ScaleComponent,
--		data = { scale = 1.0 }
--	},
--	--{
--	--	type = core.componentType.FlowfieldComponent,
--	--	data = { node = -1 }
--	--},
--	{
--		type = core.componentType.RotationComponent,
--		data = { rotation = { 0, 0, 0, 1 } }
--	}
--})
--end

local function CreateEnemyTankTurret(position)
	return scen:loadAssembly( 
	{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { position[1], 0, position[2] } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/model/tankwars/leopard-turret.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/texture/tankwars/leopard-turret.material", false }
			   }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 1.0 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { 0, 0, 0, 1 } }
	}
}
	)
end