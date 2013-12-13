print( "Starting" )


local function callback( meshid )
    print( meshid )
end

core.contentmanager.load( core.loaders.GnomeLoader, "assets/tomte.gnome", callback, false )
core.contentmanager.load( core.loaders.GnomeLoader, "assets/tomte.gnome", callback, false )
core.contentmanager.load( core.loaders.GnomeLoader, "assets/tomte.gnome", callback, false )
core.contentmanager.free( core.loaders.GnomeLoader, "assets/tomte.gnome" )
core.contentmanager.free( core.loaders.GnomeLoader, "assets/tomte.gnome" )
core.contentmanager.free( core.loaders.GnomeLoader, "assets/tomte.gnome" )

value = {}

for k = 1, 20 do 
    for i = 1, 4000 do
        value[i] = core.entity.create( core.componentType.WorldPositionComponent );

        value[i]:set( core.componentType.WorldPositionComponent, { position = {1,2,3} } )
--        value[i]:set( core.componentType.GraphicsComponent, { mesh = 0 } )

        local outval = value[i]:get( core.componentType.WorldPositionComponent )
    end

    for i = 1, 4000 do
        value[i]:destroy( )
    end
    
    print( "It:" .. k )
end

print( "Done" )
