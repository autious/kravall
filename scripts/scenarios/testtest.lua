-- attempts to load a navmesh from lua

--load = { mesh = { core.loaders.GnomeLoader, "assets/cube.bgnome" } }
--load = { mesh = { core.loaders.NavigationMeshLoader, "testNaveMesh.txt" } }

core.contentmanager.load( 
		core.loaders.NavigationMeshLoader, "testNaveMesh.txt", function( value ) end, false )


collectgarbage()
