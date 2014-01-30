local entities = require "entities"
local scenario = require "scenario"
local scen = scenario.new()

local ambient = entities.get "ambientLight"
local directional = entities.get "directionalLight"
local pointLight = entities.get "pointLight"
local area = entities.get "area"
local rioter = entities.get "rioter"
local staticModel = entities.get "staticModel"

scen.asm:specific_content( core.contentmanager.load( core.loaders.NavigationMeshLoader, "scripts/scenarios/scenario_00.nav", function( value ) end, false ) )


local script = dofile "scripts/scenarios/scenario_00_script.lua"( scen )

local function genF( e, f )
    if type( script[f] ) == "function"  then
        return function() script[ f ]( e ) end
    else
        error( "Function " .. f .. " does not exist in script file" )
    end
end

directional( scen, -0.545976996421814, -0.7968841791152954, -0.2586206793785095, 1.0, 0.8689313530921936, 0.69304358959198, 1.5 )
ambient( scen, 0.5842457413673401, 0.857562243938446, 1.0, 0.7137178182601929 )

local ent = area( scen, {-37.92256546020508, 0, -5.602817535400391}, {-7.091899394989014, 6.968003273010254, -7.091899394989014, -7.051133155822754, 7.091899394989014, -7.051133155822754, 7.091899394989014, 6.968003273010254}, "" )
scen:registerInitCallback( genF( ent, "goal_1" ) )

local ent = area( scen, {-5.740078926086426, 0, 22.64487648010254}, {-7.091899394989014, 6.968003273010254, -7.091899394989014, -7.051133155822754, 7.091899394989014, -7.051133155822754, 7.091899394989014, 6.968003273010254}, "" )
scen:registerInitCallback( genF( ent, "goal_0" ) )

local ent = area( scen, {8.611710548400879, 0, -5.511226654052734}, {-7.091899394989014, 6.968003273010254, -7.091899394989014, -7.051133155822754, 7.091899394989014, -7.051133155822754, 7.091899394989014, 6.968003273010254}, "" )
scen:registerInitCallback( genF( ent, "createRioter_1" ) )

local ent = area( scen, {-5.883894920349121, 0, -20.169673919677734}, {-7.091899394989014, 6.968003273010254, -7.091899394989014, -7.051133155822754, 7.091899394989014, -7.051133155822754, 7.091899394989014, 6.968003273010254}, "" )
scen:registerInitCallback( genF( ent, "createRioter_0" ) )

local ent = pointLight( scen, -11.520719528198242, 3.6668026447296143, -12.090536117553711, 0.9776182174682617, 0.0, 1.0, 8.420000076293945, 10.0 )
local ent = pointLight( scen, 0.17929601669311523, 3.6668028831481934, -5.546976566314697, 0.9776182174682617, 0.0, 1.0, 8.420000076293945, 10.0 )
local ent = pointLight( scen, -3.8115334510803223, 3.6668028831481934, -0.7558661699295044, 0.9776182174682617, 0.0, 1.0, 8.420000076293945, 10.0 )

local ent = staticModel( scen, -9.068596839904785, 0.0, 2.993535041809082, 0.0, 0.0, 0.0, 1.0, 6.863753795623779, "assets/model/animated/police/cop/cop-light_00.bgnome", "assets/material/animated/police/cop/cop-light_00.material", "per" )


return scen
