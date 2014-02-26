local entities = require "entities"
local scenario = require "scenario"
local scen = scenario.new()

scen.name = "Default Scenario Name"
scen.description = "Default Description"

scen.cameras = scen.cameras or {}

local script = dofile "scripts/scenarios/testtest.lua"( scen )

function scen:load()
    local ambient = entities.get "ambientLight"
    local directional = entities.get "directionalLight"
    local pointLight = entities.get "pointLight"
    local spotLight = entities.get "spotLight"
    local area = entities.get "area"
    local rioter = entities.get "rioter"
    local staticModel = entities.get "staticModel"
	local squad = entities.get "policeSquad"
    
    scen.asm:specific_content( core.contentmanager.load( core.loaders.NavigationMeshLoader, "extremeScenario.txt", function( value ) end, false ) )

	local function genF( e, f )
        if type( script[f] ) == "function"  then
            return function() script[ f ]( e ) end
        else
            error( "Function " .. f .. " does not exist in script file" )
        end
    end
	
    scen:registerInitCallback( genF( ent, "makeASquadYo" ) )
	
    directional( scen, 0.6643835306167603, -0.6150288581848145, 0.42465752363204956, 1.0, 0.9453219771385193, 0.7452670335769653, 0.7205882668495178 )
    ambient( scen, 0.6720433235168457, 0.8547968864440918, 1.0, 0.017941176295280457 )
	

end
return scen
