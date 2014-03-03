return function( scen, posX, posY, posZ, rotX, rotY, rotZ, rotW, scaleX, scaleY, scaleZ, material, name )

	local components =
	{
		{
			type = core.componentType.WorldPositionComponent,
			data = { position = { posX, posY, posZ } }
		},
		{
			type = core.componentType.GraphicsComponent,
			data = { 	
						mesh = 0,
						material = 0,
						type = core.gfx.objectTypes.DecalGeometry,
						layer = core.gfx.layerTypes.MeshLayer,
						outlineColor = {scaleX, scaleY, scaleZ, 1},
						render = true 
			    	},
			load = { 
						mesh = { core.loaders.GnomeLoader, "assets/model/dev/cube.bgnome", false },
						material = { core.loaders.MaterialLoader, material, false }
				   }
		},
		{
			type = core.componentType.ScaleComponent,
			data = { scale = { scaleX, scaleY, scaleZ } }
		},
		{
			type = core.componentType.RotationComponent,
			data = { rotation = { rotX, rotY, rotZ, rotW } }
		}
	}

	return scen:loadAssembly( components )

end