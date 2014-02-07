local entities = require "entities"
local scenario = require "scenario"
local scen = scenario.new()

scen.name = "Default Scenario Name"
scen.description = "Default Description"

scen.cameras = scen.cameras or {}
scen.cameras["settings"] = {
    view = core.glm.mat4.new(
    1.0, 1.5099580252808664e-07, -1.5099580252808664e-07, -0.0,
    -1.5099580252808664e-07, 1.0, 4.371138828673793e-08, 0.0,
    1.5099578831723193e-07, -4.3711366970455856e-08, 1.0, -0.0,
    -81.00611114501953, -56.796382904052734, 88.70008850097656, 1.0 ),
    fov = 0.8575560450553894
}
scen.cameras["credits"] = {
    view = core.glm.mat4.new(
    0.794392466545105, -0.05317002534866333, 0.6050732135772705, -0.0,
    0.271955281496048, 0.9218692779541016, -0.27603834867477417, 0.0,
    -0.5431214570999146, 0.38383567333221436, 0.7467859387397766, -0.0,
    10.752090454101562, -74.53640747070312, 90.53107452392578, 1.0 ),
    fov = 0.8575560450553894
}
scen.cameras["main"] = {
    view = core.glm.mat4.new(
    1.0, 1.5099580252808664e-07, -1.5099580252808664e-07, -0.0,
    -1.5099580252808664e-07, 1.0, 4.371138828673793e-08, 0.0,
    1.5099578831723193e-07, -4.3711366970455856e-08, 1.0, -0.0,
    1.3397865586739499e-05, -90.29090881347656, -1.5608559846878052, 1.0 ),
    fov = 0.8575560450553894
}
scen.cameras["scen"] = {
    view = core.glm.mat4.new(
    1.0, 1.5099580252808664e-07, -1.5099580252808664e-07, -0.0,
    -1.5099580252808664e-07, 1.0, 4.371138828673793e-08, 0.0,
    1.5099578831723193e-07, -4.3711366970455856e-08, 1.0, -0.0,
    -2.214122832810972e-05, 145.07388305664062, -1.5608457326889038, 1.0 ),
    fov = 0.8575560450553894
}
scen.cameras["setup"] = {
    view = core.glm.mat4.new(
    1.0, 1.5099580252808664e-07, -1.5099580252808664e-07, -0.0,
    -1.5099580252808664e-07, 1.0, 4.371138828673793e-08, 0.0,
    1.5099578831723193e-07, -4.3711366970455856e-08, 1.0, -0.0,
    -7.778785948175937e-05, 513.6048583984375, -1.5608296394348145, 1.0 ),
    fov = 0.8575560450553894
}

local script = dofile "scripts/scenarios/main_menu_script.lua"( scen )

function scen:load()
    local ambient = entities.get "ambientLight"
    local directional = entities.get "directionalLight"
    local pointLight = entities.get "pointLight"
    local spotLight = entities.get "spotLight"
    local area = entities.get "area"
    local rioter = entities.get "rioter"
    local staticModel = entities.get "staticModel"
    
    local function genF( e, f )
        if type( script[f] ) == "function"  then
            return function() script[ f ]( e ) end
        else
            error( "Function " .. f .. " does not exist in script file" )
        end
    end
    
    directional( scen, -0.0, -1.0, 0.0, 1.0, 1.0, 1.0, 1.0 )
    ambient( scen, 1.0, 1.0, 1.0, 0.5 )


    local ent = staticModel( scen, 6.528011798858643, -508.095458984375, -102.48731231689453, 0.7071068286895752, 0.0, 0.0, 0.7071068286895752, 2.061257839202881, "assets/model/dev/block-building_00.bgnome", "assets/texture/dev/test.material", "" )
    local ent = staticModel( scen, 5.5598907470703125, -150.8646240234375, -100.6944580078125, 0.7071068286895752, 0.0, 0.0, 0.7071068286895752, 2.061257839202881, "assets/model/dev/block-building_00.bgnome", "assets/texture/dev/test.material", "" )
    local ent = staticModel( scen, 78.6101303100586, 66.74439239501953, -124.31913757324219, 0.7071068286895752, 0.0, 0.0, 0.7071068286895752, 2.061257839202881, "assets/model/dev/block-building_00.bgnome", "assets/texture/dev/test.material", "" )
    local ent = staticModel( scen, -67.89867401123047, 89.50486755371094, -33.91289138793945, 0.0, -0.9728877544403076, 0.0, 0.23127798736095428, 1.0, "assets/model/animated/police/cop/cop-light_00.bgnome", "assets/texture/animated/police/cop/cop-light_00.material", "" )
    local ent = staticModel( scen, -100.81935119628906, 95.04367065429688, -179.70816040039062, 0.7071068286895752, 0.0, 0.0, 0.7071068286895752, 3.508004983266195, "assets/model/dev/tall-building_00.bgnome", "assets/texture/dev/test.material", "" )

end
return scen
