local ASML = require "assembly_loader" 
local asm = ASML.loadPack( {} )

--Plane
asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, 0, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/plane.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/light_test.material", false }
			   }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 100.0 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { -math.sin(math.pi/4.0), 0, 0, math.cos(math.pi/4.0) } }
	}
}
)

--Flamingo
asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, 11.5, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/flamingo.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/animated.material", false }
			   }
	},
	{
		type = core.componentType.AnimationComponent,
		data = { 	skeletonID = -1, 
					animationID = -1, 
					currentTime = 0.0,
					currentFrame = 0,
					speed = 1.0,
					loop = false,
					playing = false
					},
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 0.1 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { 0, 0, 0, 0 } }
	}
}
)
-- Spotlight
asm:loadAssembly( 
{
	{
		type = core.componentType.LightComponent,
		data =  { 
					color = { 1, 1, 1 },
					speccolor = { 1, 1, 1 },
					intensity = 10.0,
					spotangle = 3.14/4.0,
					spotpenumbra = 0.03,
					type = core.gfx.objectTypes.Light,
					lighttype = core.gfx.lightTypes.Spot
				}
	},
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, 50, 0 } }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 150 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = {0,-1,0,0 } }
	}
} 
)
return asm;
