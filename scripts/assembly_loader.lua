local M = {}
local ASM = {}

function M.loadPack( asmpack )
    local asm = {}

    asm.block_loads = false
    asm.entities = {}

    setmetatable( asm, {__index = ASM} )

    for k,v in pairs( asmpack ) do
        asm:loadAssembly( v )
    end

    return asm
end

function ASM:loadAssembly( asmtable )  
    if self.entities ~= nil then
        local passedload = false
        local assets = {}

        local componentTypes = {}
        -- Put the entity into the entity handler.
        
        local function apply( asmtable )
            -- If we have called destroy we block any future incoming loads.
            if self.block_loads then
                for k,v in pairs( assets ) do
                    core.contentmanager.free( v[1], v[2] )
                end
            else

                local entity = core.entity.create( unpack( componentTypes ) )

                for k,v in ipairs( asmtable ) do 
                    entity:set( v.type, v.data ) 
                end

                self.entities[entity] = assets
            end

        end
        
        local count = 0

        for component_index, component in ipairs( asmtable ) do
            componentTypes[#componentTypes+1] = component.type;

            if component.load ~= nil then
                for index,loader_pair in pairs( component.load ) do 
                    count = count + 1

                    local async_load = true

                    if type( loader_pair[3] ) == "boolean" then
                        async_load = loader_pair[3]
                    end
                    
                    core.contentmanager.load( loader_pair[1], loader_pair[2], function( value )
                        component.data[index] = value
                        count = count - 1
                        
                        -- If this is a later callback and we've passed the loadAssembly function
                        -- consider applying the metatable
                        if( count == 0 and passedload == true ) then
                            apply( asmtable )
                        end
                    end, async_load)
                    assets[#assets+1] = loader_pair
                end
            end
        end

        -- If no loaders have been create or all loaders where blocking,
        -- directly apply the entity into game
        if count == 0 then 
            apply( asmtable )
        else
            passedload = true
        end
    else
        error( "unable to load entity into destroyed assembly" )
    end
end

function ASM:destroy( )
    self.block_loads = true
    if self.entities ~= nil then
        for entity, assets in pairs( self.entities ) do
            entity:destroy()
            for k, loader_pair in pairs( assets ) do
                core.contentmanager.free( loader_pair[1], loader_pair[2] )
            end
        end
    end
    self.entities = nil
end

return M
