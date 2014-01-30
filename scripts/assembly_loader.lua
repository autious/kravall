local table_utils = require( "table_utils" )
local M = {}
local ASM = {}

function M.loadPack( asmpack )
    local asm = {}

    asm.block_loads = false
    asm.entities = {}
	asm.specific_loads = {}
    asm.active_load_count = 0

    setmetatable( asm, {__index = ASM} )

    for k,v in pairs( asmpack ) do
        asm:loadAssembly( v )
    end

    return asm
end

--dummy function to allow backward compatability to old scenarios
function ASM:update( delta )
end

function ASM:isLoading( )
    return self.active_load_count ~= 0
end

function ASM:loadAssembly( asmtable )  
    if self.entities ~= nil then
        local passedload = false
        local assets = {}
        local entity = nil

        local componentTypes = {}
        -- Put the entity into the entity handler.
        
        local function apply( asmtable )
            -- If we have called destroy we block any future incoming loads.
            if self.block_loads then
                for k,v in pairs( assets ) do
                    core.contentmanager.free( v[1], v[2] )
                end
            else

                entity = core.entity.create( unpack( componentTypes ) )

                for k,v in ipairs( asmtable ) do 
                    entity:set( v.type, v.data, v.ignoreHard ) 
                end

                self.entities[entity] = assets
            end

        end
        
        local count = 0

        for component_index, component in ipairs( asmtable ) do
            componentTypes[#componentTypes+1] = component.type;

            if component.load ~= nil then
                count = table_utils.count( component.load )
                for index,loader_pair in pairs( component.load ) do 

                    local async_load = false

                    if type( loader_pair[3] ) == "boolean" then
                        async_load = loader_pair[3]
                    end

                    assets[#assets+1] = core.contentmanager.load( loader_pair[1], 
                    loader_pair[2], 
                    function( value )
                        component.data[index] = value
                        count = count - 1
                        
                        -- If this is a later callback and we've passed the loadAssembly function
                        -- consider applying the metatable
                        if( count == 0 and passedload == true ) then
                            apply( asmtable )
                        end
                        -- Decreasse the active load count.
                        self.active_load_count = self.active_load_count - 1
                    end, 
                    async_load)
                    -- Increase the load queue count.
                    self.active_load_count = self.active_load_count + 1
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

        return entity
    else
        error( "unable to load entity into destroyed assembly" )
    end
end

function ASM:specific_content( entity )
	self.specific_loads[#self.specific_loads+1] = entity
end
	

function ASM:destroy( )
    self.block_loads = true
    self.active_load_count = 0
    if self.entities ~= nil then
        for entity, assets in pairs( self.entities ) do
            entity:destroy()
            for k, asset in pairs( assets ) do
                core.contentmanager.free( asset )
            end
        end
    end
    self.entities = nil
	
	if self.specific_loads ~= nil then
        for _,levelSpecifics in pairs( self.specific_loads ) do
			core.contentmanager.free( levelSpecifics )
        end
    end
    self.specific_loads = nil
end

function ASM:destroyEntity( softHandle )
    for entity, assets in pairs( self.entities ) do
        if entity:isSameEntity( softHandle ) then
            entity:destroy()
            for k, asset in pairs( assets ) do
                core.contentmanager.free( asset )
            end
        end
    end
end

return M
