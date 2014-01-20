function genfunction( entity, callback )
    return function()
        callback( entity )  
    end
end

return function ( scen, position, vertices, name, callback_init, callback_tick )
    scen:loadAssembly(
    {
        {
            type = core.componentType.WorldPositionComponent,
            data = { position = position }
        },
        {
            type = core.componentType.AreaComponent,
            data = { vertices = vertices }
        },
        {
            type = core.componentType.NameComponent,
            data = { name = name }

        }
    })

    if callback_init ~= nil then
        scen:registerInitCallback( genfunction( ent, callback_init ) )
    end

    if callback_tick ~= nil then 
        scen:registerTickCallback( genfunction( ent, callback_tick ) )
    end
end
