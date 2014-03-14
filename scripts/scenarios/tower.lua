local ASML = require "assembly_loader" 
local scen = require "scenario":new()
local keyboard = core.input.keyboard
local key = keyboard.key

scen.gamemode = require "gamemodes/empty":new()

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

for i=1,10 do
local scl = 2
scen.asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 20*math.sin(2*math.pi*i*0.1), scl/2, 20*math.cos(2*math.pi*i*0.1) } }
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
		data = { position = { 0, 1, 5 } }
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
scen.asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 5*(0.866), 1, -2.5 } }
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
scen.asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 5*(-0.866), 1, -2.5 } }
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
-- Dirlight
light = scen.asm:loadAssembly( 
{
	{
		type = core.componentType.LightComponent,
		data =  { 
					color = { 1, 1, 1 },
					speccolor = { 1, 1, 1},
					intensity = 0.1,
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
		data = { rotation = {1,-0.7,-1,0 } }
	}
} 
)
-- Ambientlight
light = scen.asm:loadAssembly( 
{
	{
		type = core.componentType.LightComponent,
		data =  { 
					color = { 1, 1, 1 },
					speccolor = { 1, 1, 1},
					intensity = 0.01,
					spotangle = 3.14/4.0,
					spotpenumbra = 0.03,
					type = core.gfx.objectTypes.Light,
					lighttype = core.gfx.lightTypes.Ambient
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
		data = { rotation = {1,-0.7,-1,0 } }
	}
} 
)

pointLight = scen:loadAssembly( 
{
	{
		type = core.componentType.LightComponent,
		data =  { 
					color = { 1, 1, 0.8 },
					speccolor = { 1, 1, 0.8 },
					intensity = 10.0,
					spotangle = 3.14/3.0,
					spotpenumbra = 0.5,
					type = core.gfx.objectTypes.Light,
					lighttype = core.gfx.lightTypes.Point
				}
	},
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 10, 5, 0 } }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 50.0 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { 0,-1,0,0 } }
	}
} 
)
spotlight = scen:loadAssembly( 
{
	{
		type = core.componentType.LightComponent,
		data =  { 
					color = { 0.2, 0.7, 0.8 },
					speccolor = { 0.2, 0.7, 0.8 },
					intensity = 15.0,
					spotangle = 3.14/3.0,
					spotpenumbra = 0.5,
					type = core.gfx.objectTypes.Light,
					lighttype = core.gfx.lightTypes.Spot
				}
	},
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { -10, 0.1, 0 } }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 50.0 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { 0,-1,0,0 } }
	}
} 
)
local rot = 10
local tick = 0
local baseIntensity = pointLight:get(core.componentType.LightComponent).intensity
local x = 200
local y = 300
local offs = 0.0
local function Update(delta)
	tick = tick + delta
	if keyboard.isKeyDown( key.R ) then
		rot = rot + delta;
	end
	
	if keyboard.isKeyDown( key.Left ) then
		x = x - 100*delta;
		if x < 25 then x = 25 end
	end
	if keyboard.isKeyDown( key.Right ) then
		x = x + 100*delta;
	end
	if keyboard.isKeyDown( key.Down ) then
		y = y + 100*delta;
	end
	if keyboard.isKeyDown( key.Up ) then
		y = y - 100*delta;
		if y < 25 then y = 25 end
	end
	if keyboard.isKeyDown( key.Q ) then
		offs = offs - 100*delta
	end
	if keyboard.isKeyDown( key.E ) then
		offs = offs + 100*delta
	end
	
	local rc;
    rc = spotlight:get(core.componentType.RotationComponent)
	rc.rotation = { math.cos(rot), math.sin(rot), 0, 0 }
	spotlight:set(core.componentType.RotationComponent, rc)
	
	local flicker = math.random()*(0.5+0.5*math.sin(2*math.sin(tick)))
	if  flicker > 0.95 then
		flicker = 0
	elseif  flicker > 0.9 then
		flicker = 0.1
	else
		flicker = 1
	end
	
	local lc;
    lc = pointLight:get(core.componentType.LightComponent)
	lc.intensity = baseIntensity * flicker;
	pointLight:set(core.componentType.LightComponent, lc)
	
	--core.draw.drawTextbox( 100, 100, 100, 300, 0, "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890" )
	--core.draw.drawRectangle( 100, 100, 100, 300, false )
	--core.draw.drawTextbox( 300, 100, 100, 300, 0, "FIRST. This is a really long text repeated many times. This is a really long text repeated many times. LAST." )
	--core.draw.drawRectangle( 300, 100, 100, 300, false )
	--core.draw.drawTextbox( 500, 100, x, y, offs, "FIRST. ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 This is a really long text repeated many times. ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 This is a really long text repeated many times. LAST." )
	--core.draw.drawRectangle( 500, 100, x, y, false )
end

scen:registerUpdateCallback( Update )

return scen;
