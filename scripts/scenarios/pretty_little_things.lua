local ASML = require "assembly_loader" 

local asm = ASML.loadPack( {} )

--Plane
asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, 10, 0 } }
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
		data = { scale = 5.0 }
	},
	{
		type = core.componentType.RotationComponent,
		--data = { rotation = { 0,0, math.sin( 3.14 ), math.cos(3.14/2.0) } }
		data = { rotation = { 0, 0, 0, 0 } }
	}
}
)
--Cube
asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 200, 10, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/cube.bgnome", false },
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
--Teapot
asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, 10, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/teapot.bgnome", false },
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
--Torus
asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { -25, 12.5, -20 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry },
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
asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 20, 15, -20 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry },
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
asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { -30, 10, -5 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry },
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
asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 30, 10, -10 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry },
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
asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, 14, -30 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry },
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
asm:loadAssembly( 
    {
        {
            type = core.componentType.LightComponent,
            data =  { 
                        color = { 1.0, 0.0, 0.0 },
                        intensity = 1.0,
                        type = core.gfx.objectTypes.Light,
                        lighttype = core.gfx.lightTypes.Point
                    }
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
    asm:loadAssembly( 
    {
        {
            type = core.componentType.LightComponent,
            data =  { 
                        color = { math.random(), math.random(), math.random() },
                        intensity = 1.0,
                        type = core.gfx.objectTypes.Light,
                        lighttype = core.gfx.lightTypes.Point
                    }
        },
        {
            type = core.componentType.WorldPositionComponent,
            data = { position = { -50.0 + 100.0 * math.random(), 13 + 2.5 * math.random(), -50.0 + 100.0 * math.random() } }
        },
        {
            type = core.componentType.ScaleComponent,
            data = { scale = 15.0 + 25.0 * math.random() }
        },
        {
            type = core.componentType.RotationComponent,
            data = { rotation = { math.random(),-1,math.random(),0 } }
        }
    } 
    )
end
-- Spotlight
for i = 0, 255 do
    asm:loadAssembly( 
    {
        {
            type = core.componentType.LightComponent,
            data =  { 
                        color = { math.random(), math.random(), math.random() },
                        intensity = 1.0,
						spotangle = 3.14/4.0,
						spotpenumbra = 0.03,
                        type = core.gfx.objectTypes.Light,
                        lighttype = core.gfx.lightTypes.Spot
                    }
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
            data = { rotation = { math.random(),-1,math.random(),0 } }
        }
    } 
    )
end

asm:loadAssembly( 
{
	{
		type = core.componentType.LightComponent,
		data =  { 
					color = { 0.8, 0.4, 0.2 },
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
		data = { scale = 50.0 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { -1,-1,-1,0 } }
	}
} 
)
asm:loadAssembly( 
{
	{
		type = core.componentType.LightComponent,
		data =  { 
					color = { 0.4, 0.8, 0.2 },
					intensity = 15.0,
					spotangle = 3.14/4.0,
					spotpenumbra = 0.03,
					type = core.gfx.objectTypes.Light,
					lighttype = core.gfx.lightTypes.Spot
				}
	},
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, 30, 0 } }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 50.0 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { 1,-1,0,0 } }
	}
} 
)
asm:loadAssembly( 
{
	{
		type = core.componentType.LightComponent,
		data =  { 
					color = { 0.2, 0.7, 0.8 },
					intensity = 15.0,
					spotangle = 3.14/6.0,
					spotpenumbra = 0.03,
					type = core.gfx.objectTypes.Light,
					lighttype = core.gfx.lightTypes.Spot
				}
	},
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { -10, 30, -5 } }
	},
	{
		type = core.componentType.ScaleComponent,
		data = { scale = 50.0 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { 1,-1,0,0 } }
	}
} 
)


-- Directional light
asm:loadAssembly( 
{
	{
		type = core.componentType.LightComponent,
		data =  { 
					color = { 1.0, 1.0, 1.0 },
					intensity = 0.1,
					type = core.gfx.objectTypes.Light,
					lighttype = core.gfx.lightTypes.Dir
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
		data = { rotation = { 2,-3,-1,0 } } -- Lights uses rotation component as a direction vector, not a quaternion
	}
} 
)
-- Ambient light
asm:loadAssembly( 
{
	{
		type = core.componentType.LightComponent,
		data =  { 
					color = { 1.0, 1.0, 1.0 },
					intensity = 0.01,
					type = core.gfx.objectTypes.Light,
					lighttype = core.gfx.lightTypes.Ambient
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
return asm;
