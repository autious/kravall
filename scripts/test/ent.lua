print( "Starting" )

value = {}
for i = 1, 4000 do
    value[i] = core.entity.create( core.componentType.WorldPositionComponent );

    value[i]:set( core.componentType.WorldPositionComponent, { position = {1,2,3} } )

    local outval = value[i]:get( core.componentType.WorldPositionComponent )

    print( value[i] )
end

for i = 1, 4000 do
    value[i]:destroy( )
end

print( "Done" )
