return function ( asm, position, vertices, name )
    asm:loadAssembly(
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
end





