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
    12.47426986694336, -117.69866943359375, 13.420125961303711, 1.0 ),
    fov = 0.8575560450553894
}
scen.cameras["credits"] = {
    view = core.glm.mat4.new(
    0.794392466545105, -0.05317002534866333, 0.6050732135772705, -0.0,
    0.271955281496048, 0.9218692779541016, -0.27603834867477417, 0.0,
    -0.5431214570999146, 0.38383567333221436, 0.7467859387397766, -0.0,
    -19.223499298095703, -31.135475158691406, 46.4231071472168, 1.0 ),
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
    7.47083458918496e-06, -51.0379524230957, -1.5608543157577515, 1.0 ),
    fov = 0.8575560450553894
}
scen.cameras["setup"] = {
    view = core.glm.mat4.new(
    1.0, 1.5099580252808664e-07, -1.5099580252808664e-07, -0.0,
    -1.5099580252808664e-07, 1.0, 4.371138828673793e-08, 0.0,
    1.5099578831723193e-07, -4.3711366970455856e-08, 1.0, -0.0,
    1.5433632825079258e-06, -11.782085418701172, -1.5608525276184082, 1.0 ),
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


    local ent = staticModel( scen, -25.067977905273438, 43.6976318359375, -176.21974182128906, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apt-a-0-corners_00.bgnome", "assets/texture/dev/default.material", "" )
    local ent = staticModel( scen, 78.6101303100586, 66.74439239501953, -124.31913757324219, 0.7071068286895752, 0.0, 0.0, 0.7071068286895752, 2.061257839202881, "assets/model/dev/block-building_00.bgnome", "assets/texture/dev/test.material", "" )
    local ent = staticModel( scen, -15.090109825134277, 45.471405029296875, -33.91289138793945, 0.0, -0.9728877544403076, 0.0, 0.23127798736095428, 1.0, "assets/model/animated/police/cop/cop-light_00.bgnome", "assets/texture/animated/police/cop/cop-light_00.material", "" )
    local ent = staticModel( scen, -100.81935119628906, 95.04367065429688, -179.70816040039062, 0.7071068286895752, 0.0, 0.0, 0.7071068286895752, 3.508004983266195, "assets/model/dev/tall-building_00.bgnome", "assets/texture/dev/test.material", "" )

end
return scen
