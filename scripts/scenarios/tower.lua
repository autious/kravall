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
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
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
--for i=1,10000 do
--r0 = { math.sin(math.pi/4.0), 0, 0, math.cos(math.pi/4.0) }
--rnd = math.random()
--r1 = { 0, -math.sin(math.pi/rnd), 0, math.cos(math.pi/rnd) }
--x=2
--y=3
--z=4
--w=1
--rotation = { 	r0[w]*r1[w] - r0[x]*r1[x] - r0[y]*r1[y] - r0[z]*r1[z],
--				r0[w]*r1[x] + r0[x]*r1[w] + r0[y]*r1[z] - r0[z]*r1[y],
--				r0[w]*r1[y] + r0[y]*r1[w] + r0[z]*r1[x] - r0[x]*r1[z],
--				r0[w]*r1[z] + r0[z]*r1[w] + r0[x]*r1[y] - r0[y]*r1[x] }
--asm:loadAssembly( 
--{
--	{
--		type = core.componentType.WorldPositionComponent,
--		data = { position = { -50  + 1*(i%100), 0, -50  + 1*(i/100) } }
--		--data = { position = { 0, 0, 0 } }
--	},
--	{
--		type = core.componentType.GraphicsComponent,
--		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
--		load = { 
--					mesh = { core.loaders.GnomeLoader, "assets/flamingo.bgnome", false },
--					material = { core.loaders.MaterialLoader, "assets/material/animated.material", false }
--			   }
--	},
--	{
--		type = core.componentType.AnimationComponent,
--		data = { 
--			animationID = 0,
--			currentTime = 100 * math.random(),
--			loop = true,
--			playing = true,
--			speed = 1.0,
--			currentFrame = 0
--		}
--	},
--	{
--		type = core.componentType.ScaleComponent,
--		data = { scale = 0.1 }
--	},
--	{
--		type = core.componentType.RotationComponent,
--		data = { rotation = rotation }
--		--data = { rotation = { 0, -math.sin(math.pi/4.0), 0, math.cos(math.pi/4.0) }}
--		--data = { rotation = { -math.sin(math.pi/4.0), 0, 0, math.cos(math.pi/4.0) }}
--	}
--}
--)
--end
asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 5, 0, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/rioter-female_00.bgnome", false },
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
		data = { scale = 6.0 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { 0,0,0,1 }}
	}
}
)
asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { -5, 0, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/rioter-male_00.bgnome", false },
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
		data = { scale = 6.0 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { 0,0,0,1 }}
	}
}
)
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
					mesh = { core.loaders.GnomeLoader, "assets/flamingo4.bgnome", false },
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
		data = { scale = 0.01 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { 0,0,0,1 }}
	}
}
)

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
asm:loadAssembly( 
{
	{
		type = core.componentType.LightComponent,
		data =  { 
					color = { 1.0, 1.0, 1.0 },
					speccolor = { 1.0, 1.0, 1.0 },
					intensity = 0.05,
					type = core.gfx.objectTypes.Light,
					lighttype = core.gfx.lightTypes.Dir
				},
		ignoreHard = true
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
		data = { rotation = { 2,-3,-1,0 } } -- Lights uses rotation component as a direction vector, not a quaternion
	}
} 
)
return asm;
