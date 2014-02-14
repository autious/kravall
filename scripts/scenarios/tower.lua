local ASML = require "assembly_loader" 
local scen = require "scenario":new()
local keyboard = core.input.keyboard
local key = keyboard.key

scen.gamemode = require "gamemodes/kravall":new()

scen:registerUpdateCallback( function(delta) scen.gamemode:update(delta) end )
scen:registerDestroyCallback( function() scen.gamemode:destroy() end )

scen.gamemode.camera:lookAt( core.glm.vec3.new( 0, 60, 120 ), core.glm.vec3.new( 0, 20, 50 ) )
core.gfx.setWhitepoint(1.0, 1.0, 1.0)
--Plane
scen.asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, 0, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/plane.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/light_test.material", false }
			   }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 250.0 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { -math.sin(math.pi/4.0), 0, 0, math.cos(math.pi/4.0) } }
	}
}
)

local dude = scen.asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { -10, 0, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/model/animated/rioter/rioter_00.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/rioter_00.material", false }
			   }
	},
	{
		type = core.componentType.AnimationComponent,
		data = { 
			animationID = 0,
			queuedAnimationID = 0,
			currentTime = 0.0,
			loop = false,
			playing = false,
			speed = 1.0,
			currentFrame = 0
		}
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 10.0 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { 0, 0, 0, 1 }}
	}
}
)
local dude2 = scen.asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 10, 0, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry,layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/model/animated/rioter/rioter_00.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/rioter_00.material", false }
			   }
	},
	{
		type = core.componentType.AnimationComponent,
		data = { 
			animationID = 0,
			queuedAnimationID = 0,
			currentTime = 0.0,
			loop = false,
			playing = false,
			speed = 1.0,
			currentFrame = 0
		}
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 10.0 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { 0, 0, 0, 1 }}
	}
}
)
for i=1,10 do
local scl = 1 + 2 * math.random()
scen.asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { -50 + math.random() * 100, scl/2, -50 + math.random() * 100 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry,layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/cube.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/light_test.material", false }
			   }
	},
	{
		type = core.componentType.AnimationComponent,
		data = { 
			animationID = 0,
			queuedAnimationID = 0,
			currentTime = 0.0,
			loop = false,
			playing = false,
			speed = 1.0,
			currentFrame = 0
		}
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = scl }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { 0, 0, 0, 1 }}
	}
}
)
end
scen.asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, 1, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry,layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/knot.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/light_test.material", false }
			   }
	},
	{
		type = core.componentType.AnimationComponent,
		data = { 
			animationID = 0,
			queuedAnimationID = 0,
			currentTime = 0.0,
			loop = false,
			playing = false,
			speed = 1.0,
			currentFrame = 0
		}
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 1 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { 0, 0, 0, 1 }}
	}
}
)
-- Spotlight
light = scen.asm:loadAssembly( 
{
	{
		type = core.componentType.LightComponent,
		data =  { 
					color = { 1, 1, 1 },
					speccolor = { 1, 1, 1},
					intensity = 0.8,
					spotangle = 3.14/4.0,
					spotpenumbra = 0.03,
					type = core.gfx.objectTypes.Light,
					lighttype = core.gfx.lightTypes.DirShadow
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
		data = { rotation = {1,0,0,0 } }
	}
} 
)
--scen.asm:loadAssembly( 
--{
--	{
--		type = core.componentType.LightComponent,
--		data =  { 
--					color = { 1.0, 1.0, 1.0 },
--					speccolor = { 1.0, 1.0, 1.0 },
--					intensity = 0.05,
--					type = core.gfx.objectTypes.Light,
--					lighttype = core.gfx.lightTypes.Dir
--				},
--		ignoreHard = true
--	},
--	{
--		type = core.componentType.WorldPositionComponent,
--		data = { position = { 0, 0, 0 } }
--	},
--	{
--		type = core.componentType.ScaleComponent,
--		data = { scale = 1.0 } 
--	},
--	{
--		type = core.componentType.RotationComponent,
--		data = { rotation = { 2,-3,-1,0 } } -- Lights uses rotation component as a direction vector, not a quaternion
--	}
--} 
--)
local rot = 10
local function Update(delta)
	if keyboard.isKeyDown( key.R ) then
		rot = rot + delta;
	end
	if keyboard.isKeyDown( key.Left ) then
		core.animations.loop(dude, "rioter-walk_00")
		core.animations.loop(dude2, "rioter-walk_00")
	end
	if keyboard.isKeyDown( key.Right ) then
		core.animations.loop(dude, "rioter-male-wave-walk_00")
		core.animations.loop(dude2, "rioter-male-wave-walk_00")
	end
	if keyboard.isKeyDown( key.Down ) then
		core.animations.setSpeed(dude, 1)
		core.animations.setSpeed(dude2, 1)
	end
	if keyboard.isKeyDown( key.Up ) then
		--core.animations.setSpeed(dude, 0.5)
		core.animations.setTime(dude2, 0.6)
	end
	if keyboard.isKeyDown( key.Q ) then
		core.animations.queue(dude, "rioter-male-wave-walk_00", true)
		core.animations.queue(dude2, "rioter-male-wave-walk_00", false)
	end
	
	local rc;
    rc = light:get(core.componentType.RotationComponent)
	rc.rotation = { math.cos(rot), -1 - (0.1 + math.sin(rot)), math.sin(rot), 0 }
	light:set(core.componentType.RotationComponent, rc)
	
	
end

scen:registerUpdateCallback( Update )

return scen;
