local entities = require "entities"
local scenario = require "scenario"
local scen = scenario.new()

scen.name = "Default Scenario Name"
scen.description = "Default Description"

scen.cameras = scen.cameras or {}

local script = dofile "scripts/scenarios/max-police-place_script.lua"( scen )

function scen:load()
    local ambient = entities.get "ambientLight"
    local directional = entities.get "directionalLight"
    local pointLight = entities.get "pointLight"
    local spotLight = entities.get "spotLight"
    local area = entities.get "area"
    local rioter = entities.get "rioter"
    local staticModel = entities.get "staticModel"
    local empty = entities.get "empty"
    local ent
    
    scen.asm:specific_content( core.contentmanager.load( core.loaders.NavigationMeshLoader, "scripts/scenarios/max-police-place.nav", function( value ) end, false ) )

    
    local function genF( e, f )
        if type( script[f] ) == "function"  then
            return function( delta ) script[ f ]( e, delta ) end
        else
            error( "Function " .. f .. " does not exist in script file" )
        end
    end
    
    directional( scen, 0.03658536449074745, -0.2655065953731537, 0.9634146094322205, 1.0, 1.0, 1.0, 0.3571428656578064 )
    ambient( scen, 1.0, 1.0, 1.0, 0.2077922224998474 )

    ent = area( scen, {-46.2037353515625, 0, 54.88370895385742}, {-10.428420066833496, 10.428420066833496, -10.428420066833496, -10.428420066833496, 10.428420066833496, -10.428420066833496, 10.428420066833496, 10.428420066833496}, "" )
    scen:registerInitCallback( genF( ent, "registerSpawn" ) )

    ent = area( scen, {-45.56719207763672, 0, 11.533400535583496}, {-8.532986640930176, 8.532986640930176, -8.532986640930176, -8.532986640930176, 8.532986640930176, -8.532986640930176, 8.532986640930176, 8.532986640930176}, "" )
    scen:registerInitCallback( genF( ent, "registerSpawn" ) )

    ent = area( scen, {-44.694461822509766, 0, 169.2928466796875}, {-9.7553071975708, 10.207505226135254, -9.7553071975708, -7.405930519104004, 7.858128547668457, -7.405930519104004, 7.858128547668457, 10.207505226135254}, "riot-group1" )



end
return scen
