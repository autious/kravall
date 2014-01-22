local entities = require "entities"
local scenario = require "scenario"
local scen = scenario.new()

local ambient = entities.get "ambientLight"
local directional = entities.get "directionalLight"
local street_light = entities.get "streetLight"
local area = entities.get "area"
local rioter = entities.get "rioter"

local script = dofile "scripts/scenarios/scenario_00_script.lua"( scen )

local function genF( e, f )
    if type( script[f] ) == "function"  then
        return function() script[ f ]( e ) end
    else
        error( "Function " .. f .. " does not exist in script file" )
    end
end

scen.asm:specific_content( core.contentmanager.load( core.loaders.NavigationMeshLoader, "scripts/scenarios/scenario_00.nav", function( value ) end, false ) )

local ent = area( scen, {-37.92256546020508, 0, -5.602817535400391}, {-7.091899394989014, 6.968003273010254, -7.091899394989014, -7.051133155822754, 7.091899394989014, -7.051133155822754, 7.091899394989014, 6.968003273010254}, "" )
scen:registerInitCallback( genF( ent, "goal_1" ) )

local ent = area( scen, {-5.740078926086426, 0, 22.64487648010254}, {-7.091899394989014, 6.968003273010254, -7.091899394989014, -7.051133155822754, 7.091899394989014, -7.051133155822754, 7.091899394989014, 6.968003273010254}, "" )
scen:registerInitCallback( genF( ent, "goal_0" ) )

local ent = area( scen, {8.611710548400879, 0, -5.511226654052734}, {-7.091899394989014, 6.968003273010254, -7.091899394989014, -7.051133155822754, 7.091899394989014, -7.051133155822754, 7.091899394989014, 6.968003273010254}, "" )
scen:registerInitCallback( genF( ent, "createRioter_1" ) )

local ent = area( scen, {-5.883894920349121, 0, -20.169673919677734}, {-7.091899394989014, 6.968003273010254, -7.091899394989014, -7.051133155822754, 7.091899394989014, -7.051133155822754, 7.091899394989014, 6.968003273010254}, "" )
scen:registerInitCallback( genF( ent, "createRioter_0" ) )


return scen
