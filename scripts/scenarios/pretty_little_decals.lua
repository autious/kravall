local scenario = require "scenario"
local scen = scenario.new()

scen.gamemode = require "gamemodes/normal":new()
scen:registerUpdateCallback( function(delta) scen.gamemode:update(delta) end )
scen:registerDestroyCallback( function() scen.gamemode:destroy() end )
core.gfx.setGamma(2.2)
core.gfx.setExposure(1)




--Plane
scen:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, 10, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry,layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
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


local dude2 = scen.asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 10, 10, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry,layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/model/animated/rioter/rioter-male_00.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/texture/animated/rioter/rioter_00.material", false }
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
		data = { scale = 1.0 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { 0, 0, 0, 1 }}
	}
}
)


--Cube
--asm:loadAssembly( 
--{
--	{
--		type = core.componentType.WorldPositionComponent,
--		data = { position = { 200, 10, 0 } }
--	},
--	{
--		type = core.componentType.GraphicsComponent,
--		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry },
--		load = { 
--					mesh = { core.loaders.GnomeLoader, "assets/cube.bgnome", false },
--					material = { core.loaders.MaterialLoader, "assets/material/light_test.material", false }
--			   }
--	},
--	{
--		type = core.componentType.ScaleComponent,
--		data = { scale = 10.0 }
--	},
--	{
--		type = core.componentType.RotationComponent,
--		--data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
--		data = { rotation = { 1*math.sin( -3.14/4 ), 0, 0, math.cos( -3.14/4 ) } }
--	}
--}
--)


--Cube
scen:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, 9.5, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/model/dev/cube.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/asphalt.material", false }
			   }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 10.0 }
	},
	{
		type = core.componentType.RotationComponent,
		--data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
		--data = { rotation = { 1*math.sin( -3.14/4 ), 0, 0, math.cos( -3.14/4 ) } }
		data = { rotation = { 0, 0, 0, 1 } }
	}
}
)

scen:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 20, 20, -20 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.DecalGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {1, 1, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/model/dev/cube.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/asphalt.material", false }
			   }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 5.0 }
	},
	{
		type = core.componentType.RotationComponent,
		--data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
		--data = { rotation = { 1*math.sin( 3.14/4 ), 0, 0, math.cos( 3.14/4 ) } }
		data = { rotation = { 0, 0, 0, 1 } }
	}
}
)

scen:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { -20, 5, 20 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.DecalGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {10, 10, 10, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/model/dev/cube.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/dev/decal_test.material", false }
			   }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 20.0 }
	},
	{
		type = core.componentType.RotationComponent,
		--data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
		--data = { rotation = { 1*math.sin( 3.14/4 ), 0, 0, math.cos( 3.14/4 ) } }
		data = { rotation = { 0, math.sin( 3.14/6 ), 0, math.cos( 3.14/6 ) } }
	}
}
)

scen:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { -15, 5, 20 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.DecalGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {25, 25, 25, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/model/dev/cube.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/dev/decal_test.material", false }
			   }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 20.0 }
	},
	{
		type = core.componentType.RotationComponent,
		--data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
		--data = { rotation = { 1*math.sin( 3.14/4 ), 0, 0, math.cos( 3.14/4 ) } }
		data = { rotation = { 0, math.sin( 3.14/6 ), 0, math.cos( 3.14/6 ) } }
	}
}
)


--Teapot
--scen:loadAssembly( 
--{
--	{
--		type = core.componentType.WorldPositionComponent,
--		data = { position = { 0, 10, 0 } }
--	},
--	{
--		type = core.componentType.GraphicsComponent,
--		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry,layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
--		load = { 
--					mesh = { core.loaders.GnomeLoader, "assets/teapot.bgnome", false },
--					material = { core.loaders.MaterialLoader, "assets/material/light_test.material", false }
--			   }
--	},
--	{
--		type = core.componentType.ScaleComponent,
--		data = { scale = 10.0 }
--	},
--	{
--		type = core.componentType.RotationComponent,
--		--data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
--		data = { rotation = { 1*math.sin( -3.14/4 ), 0, 0, math.cos( -3.14/4 ) } }
--	}
--}
--)
--Torus
scen:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { -20, 12.5, -20 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry,layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/torus.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/light_test.material", false }
			   }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 5.0 }
	},
	{
		type = core.componentType.RotationComponent,
		--data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
		data = { rotation = { 1*math.sin( -3.14/4 ), 0, 0, math.cos( -3.14/4 ) } }
	}
}
)
--Sphere
scen:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 20, 15, -20 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry,layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/sphere.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/light_test.material", false }
			   }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 12.0 }
	},
	{
		type = core.componentType.RotationComponent,
		--data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
		data = { rotation = { 1*math.sin( -3.14/4 ), 0, 0, math.cos( -3.14/4 ) } }
	}
}
)
--Cone
scen:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { -30, 10, -5 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry,layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/cone.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/light_test.material", false }
			   }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 10.0 }
	},
	{
		type = core.componentType.RotationComponent,
		--data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
		data = { rotation = { 1*math.sin( -3.14/4 ), 0, 0, math.cos( -3.14/4 ) } }
	}
}
)
--Pyramid
scen:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 30, 10, -10 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry,layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/pyramid.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/light_test.material", false }
			   }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 10.0 }
	},
	{
		type = core.componentType.RotationComponent,
		--data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
		data = { rotation = { 1*math.sin( -3.14/4 ), 0, 0, math.cos( -3.14/4 ) } }
	}
}
)
--Knot
scen:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, 14, -30 } }
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
		type = core.componentType.ScaleComponent,
		data = { scale = 3.0 }
	},
	{
		type = core.componentType.RotationComponent,
		--data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
		data = { rotation = { 1*math.sin( -3.14/4 ), 0, 0, math.cos( -3.14/4 ) } }
	}
}
)
-- Pointlight
scen:loadAssembly( 
    {
        {
            type = core.componentType.LightComponent,
            data =  { 
                        color = { 1.0, 0.0, 0.0 },
                        speccolor = { 1.0, 0.0, 0.0 },
                        intensity = 1.0,
                        type = core.gfx.objectTypes.Light,
                        lighttype = core.gfx.lightTypes.Point
                    },
			ignoreHard = true
        },
        {
            type = core.componentType.WorldPositionComponent,
            data = { position = { 10.0, 13.0, 10.0 } }
        },
        {
            type = core.componentType.ScaleComponent,
            data = { scale = 15.0 }
        },
        {
            type = core.componentType.RotationComponent,
            data = { rotation = { 0.0, 0.0, 0.0, 0.0 } }
        }
    } 
    )
	
for i = 0, 255 do
	local color = { math.random(), math.random(), math.random() }
    scen:loadAssembly( 
    {
        {
            type = core.componentType.LightComponent,
            data =  { 
                        color = color,
                        speccolor = color,
                        intensity = 1.0,
                        type = core.gfx.objectTypes.Light,
                        lighttype = core.gfx.lightTypes.Point
                    }
			, ignoreHard = true
        },
        {
            type = core.componentType.WorldPositionComponent,
            data = { position = { -50.0 + 100.0 * math.random(), 13 + 2.5 * math.random(), -50.0 + 100.0 * math.random() } }
        },
        {
            type = core.componentType.ScaleComponent,
            data = { scale = 5.0 + 5.0 * math.random() }
        },
        {
            type = core.componentType.RotationComponent,
            data = { rotation = { 0.0,0.0,0.0,0 } }
        }
    } 
    )
end
-- Spotlight
for i = 0, 255 do
	local color = { math.random(), math.random(), math.random() }
    scen:loadAssembly( 
    {
        {
            type = core.componentType.LightComponent,
            data =  { 
                        color = color,
                        speccolor = color,
                        intensity = 1.0,
						spotangle = 3.14/4.0,
						spotpenumbra = math.random(),
                        type = core.gfx.objectTypes.Light,
                        lighttype = core.gfx.lightTypes.Spot
                    }
        },
        {
            type = core.componentType.WorldPositionComponent,
            data = { position = { -50.0 + 100.0 * math.random(), 11 + 2.5 * math.random(), -50.0 + 100.0 * math.random() } }
        },
        {
            type = core.componentType.ScaleComponent,
            data = { scale = 5.0 + 5.0 * math.random() }
        },
        {
            type = core.componentType.RotationComponent,
            data = { rotation = { math.random(),-math.random() - 0.1,math.random(),0 } }
        }
    } 
    )
end

scen:loadAssembly( 
{
	{
		type = core.componentType.LightComponent,
		data =  { 
					color = { 0.8, 0.4, 0.2 },
					speccolor = { 0.8, 0.4, 0.2 },
					intensity = 15.0,
					spotangle = 3.14/4.0,
					spotpenumbra = 0.03,
					type = core.gfx.objectTypes.Light,
					lighttype = core.gfx.lightTypes.Spot
				}
	},
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 20, 15, 20 } }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 150.0 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { -1,-1,-1,0 } }
	}
} 
)
scen:loadAssembly( 
{
	{
		type = core.componentType.LightComponent,
		data =  { 
					color = { 0.4, 0.8, 0.2 },
					speccolor = { 0.4, 0.8, 0.2 },
					intensity = 15.0,
					spotangle = 3.14/7.0,
					spotpenumbra = 0.03,
					type = core.gfx.objectTypes.Light,
					lighttype = core.gfx.lightTypes.Spot
				}
	},
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { -20, 15, 20 } }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 50.0 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { 1,-0.5,-1,0 } }
	}
} 
)
scen:loadAssembly( 
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
		data = { position = { -20, 12, -5 } }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 50.0 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { 1,0,0,0 } }
	}
} 
)


-- Directional light
scen:loadAssembly( 
{
	{
		type = core.componentType.LightComponent,
		data =  { 
					color = { 1.0, 1.0, 1.0 },
					speccolor = { 1.0, 1.0, 1.0 },
					intensity = 0.1,
					type = core.gfx.objectTypes.Light,
					lighttype = core.gfx.lightTypes.DirShadow
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
-- Ambient light
scen:loadAssembly( 
{
	{
		type = core.componentType.LightComponent,
		data =  { 
					color = { 1.0, 1.0, 1.0 },
					intensity = 0.01,
					type = core.gfx.objectTypes.Light,
					lighttype = core.gfx.lightTypes.Ambient
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
		data = { rotation = { 0,0,0,0 } }
	}
} 
)
return scen;
