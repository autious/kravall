local ASML = require "assembly_loader" 
local asm = ASML.loadPack( {} )

camera:lookAt( core.glm.vec3.new( 0, 30, 60 ), core.glm.vec3.new( 0, 20, 0 ) )

--Plane
asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, 0, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = false },
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

--Pole
asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, 0, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/teapole4.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/animated.material", false }
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
			currentFrame = 0
		}
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 1.0 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { 0, 0, 0, 0 } }
	}
}
)
--asm:loadAssembly( 
--{
--	{
--		type = core.componentType.WorldPositionComponent,
--		data = { position = { -10, 11.65, 0 } }
--	},
--	{
--		type = core.componentType.GraphicsComponent,
--		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
--		load = { 
--					mesh = { core.loaders.GnomeLoader, "assets/teapole.bgnome", false },
--					material = { core.loaders.MaterialLoader, "assets/material/animated.material", false }
--			   }
--	},
--	{
--		type = core.componentType.AnimationComponent,
--		data = { 
--			animationID = 0,
--			currentTime = 0.0,
--			loop = true,
--			playing = true,
--			speed = 1.0,
--			currentFrame = 0
--		}
--	},
--	{
--		type = core.componentType.ScaleComponent,
--		data = { scale = 1.0 }
--	},
--	{
--		type = core.componentType.RotationComponent,
--		data = { rotation = { 0, 0, 0, 0 } }
--	}
--}
--)
--
--asm:loadAssembly( 
--{
--	{
--		type = core.componentType.WorldPositionComponent,
--		data = { position = { 10, 11.65, 0 } }
--	},
--	{
--		type = core.componentType.GraphicsComponent,
--		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
--		load = { 
--					mesh = { core.loaders.GnomeLoader, "assets/teapole.bgnome", false },
--					material = { core.loaders.MaterialLoader, "assets/material/light_test.material", false }
--			   }
--	},
--	{
--		type = core.componentType.ScaleComponent,
--		data = { scale = 1.0 }
--	},
--	{
--		type = core.componentType.RotationComponent,
--		data = { rotation = { 0, 0, 0, 0 } }
--	}
--}
--)
-- Spotlight
asm:loadAssembly( 
{
	{
		type = core.componentType.LightComponent,
		data =  { 
					color = { 1, 1, 1 },
					speccolor = { 1, 1, 1 },
					intensity = 100.0,
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
