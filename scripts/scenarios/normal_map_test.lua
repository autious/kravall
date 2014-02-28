local ASML = require "assembly_loader" 

local asm = ASML.loadPack( {} )

camera:lookAt( core.glm.vec3.new( -20, 35, 20 ), core.glm.vec3.new( 0, 0, 20 ) )

--Plane
asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, 10, 0 } }
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
		data = { scale = 100.0 }
	},
	{
		type = core.componentType.RotationComponent,
		data = { rotation = { -math.sin(math.pi/4.0), 0, 0, math.cos(math.pi/4.0) } }
	}
}
)

--Cube
asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, 10, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/cube.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/normalsTest.material", false }
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

--Cube
asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 15, 10, 0 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/cube.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/normalsTest.material", false }
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

--Cube
asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 0, 10, 15 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/cube.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/normalsTest.material", false }
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

--Cube
asm:loadAssembly( 
{
	{
		type = core.componentType.WorldPositionComponent,
		data = { position = { 15, 10, 15 } }
	},
	{
		type = core.componentType.GraphicsComponent,
		data = { mesh = 0, material = 0, type = core.gfx.objectTypes.OpaqueGeometry, layer = core.gfx.layerTypes.MeshLayer, outlineColor = {0, 0, 1, 1}, render = true },
		load = { 
					mesh = { core.loaders.GnomeLoader, "assets/cube.bgnome", false },
					material = { core.loaders.MaterialLoader, "assets/material/normalsTest.material", false }
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

asm:loadAssembly( 
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
asm:loadAssembly( 
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
asm:loadAssembly( 
{
	{
		type = core.componentType.LightComponent,
		data =  { 
					color = { 0.2, 0.7, 0.8 },
					speccolor = { 0.2, 0.7, 0.8 },
					intensity = 150.0,
					spotangle = 3.14/3.0,
					spotpenumbra = 0.1,
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


---- Directional light
--asm:loadAssembly( 
--{
--	{
--		type = core.componentType.LightComponent,
--		data =  { 
--					color = { 1.0, 1.0, 1.0 },
--					speccolor = { 1.0, 1.0, 1.0 },
--					intensity = 0.05,
--					type = core.gfx.objectTypes.Light,
--					lighttype = core.gfx.lightTypes.Dir
--				}
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
---- Ambient light
--asm:loadAssembly( 
--{
--	{
--		type = core.componentType.LightComponent,
--		data =  { 
--					color = { 1.0, 1.0, 1.0 },
--					intensity = 0.01,
--					type = core.gfx.objectTypes.Light,
--					lighttype = core.gfx.lightTypes.Ambient
--				}
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
--		data = { rotation = { 0,0,0,0 } }
--	}
--} 
--)
return asm;