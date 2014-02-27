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
    -22.561710357666016, -38.03951644897461, 179.59658813476562, 1.0 ),
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
    local empty = entities.get "empty"
    local decal = entities.get "decal"
    local ent
    
    local function genF( e, f )
        if type( script[f] ) == "function"  then
            return function( delta ) script[ f ]( e, delta ) end
        else
            error( "Function " .. f .. " does not exist in script file" )
        end
    end
    
    directional( scen, 0.5034013390541077, -0.671129047870636, 0.5442177057266235, 0.5165932178497314, 0.48119795322418213, 1.0, 0.24817520380020142 )
    ambient( scen, 1.0, 0.898456335067749, 0.9377175569534302, 0.16423356533050537 )

    ent = pointLight( scen, -104.37144470214844, 115.81341552734375, -147.37832641601562, 1.0, 0.48000001907348633, 0.48000001907348633, 22.98400115966797, 20.0, "" )
    ent = pointLight( scen, -104.37144470214844, 106.87908172607422, -147.37832641601562, 1.0, 0.48000001907348633, 0.48000001907348633, 22.98400115966797, 20.0, "" )
    ent = pointLight( scen, -104.37144470214844, 95.90730285644531, -147.37832641601562, 1.0, 0.48000001907348633, 0.48000001907348633, 22.98400115966797, 20.0, "" )
    ent = pointLight( scen, -104.37144470214844, 83.24391174316406, -147.37832641601562, 1.0, 0.48000001907348633, 0.48000001907348633, 22.98400115966797, 20.0, "" )
    ent = pointLight( scen, 43.0689582824707, 92.84103393554688, -120.31990051269531, 1.0, 1.0, 1.0, 50.62799835205078, 1000.0, "" )
    ent = spotLight( scen, 22.47854995727539, -127.205322265625, -45.28828430175781, -0.19656385481357574, -0.9521231055259705, -0.23414583504199982, 1.0, 1.0, 1.0, 0.39269909262657166, 0.19634954631328583, 39.01600646972656, 1000.0, "" )
    ent = spotLight( scen, 2.4068355560302734, -127.99591064453125, -43.04051208496094, 0.0010626527946442366, -0.9938949346542358, -0.11032560467720032, 1.0, 1.0, 1.0, 0.39269909262657166, 0.19634954631328583, 39.01600646972656, 1000.0, "" )
    ent = spotLight( scen, -15.07491683959961, -128.57705688476562, -38.71961975097656, 0.1712392270565033, -0.9767237305641174, -0.12918159365653992, 1.0, 1.0, 1.0, 0.39269909262657166, 0.19634954631328583, 39.01600646972656, 1000.0, "" )
    ent = spotLight( scen, 17.585102081298828, -523.7137451171875, -50.27806854248047, -0.1480923444032669, 0.9867357015609741, -0.06649278849363327, 1.0, 1.0, 1.0, 0.4770510196685791, 0.23852550983428955, 36.164005279541016, 1000.0, "" )
    ent = spotLight( scen, -10.454227447509766, -524.4407348632812, -34.23618698120117, 0.0, 0.9867357015609741, -0.16233494877815247, 1.0, 1.0, 1.0, 0.4770510196685791, 0.23852550983428955, 36.164005279541016, 1000.0, "" )

    ent = staticModel( scen, 17.751018524169922, 39.82673263549805, -141.3573455810547, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-f/1-valve.bgnome", "assets/texture/static/building/brickwall-32_00.material", "" )
    ent = staticModel( scen, 17.751018524169922, 39.82673263549805, -141.3573455810547, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-f/1-main2.bgnome", "assets/texture/static/building/window-brickwall.material", "" )
    ent = staticModel( scen, 17.751018524169922, 39.82673263549805, -141.3573455810547, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-f/1-main.bgnome", "assets/texture/static/building/brickwall-darkred_00.material", "" )
    ent = staticModel( scen, 17.751018524169922, 39.82673263549805, -141.3573455810547, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-f/1-frame.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    ent = staticModel( scen, 74.01870727539062, 39.77765655517578, -132.9451446533203, 0.0, 0.7078686356544495, 0.0, 0.7063441276550293, 1.0, "assets/model/static/building/apartment/s01-e/1-windows_00.bgnome", "assets/texture/static/building/concrete-pattern-with-window_00.material", "" )
    ent = staticModel( scen, 74.01870727539062, 39.77765655517578, -132.9451446533203, 0.0, 0.7078686356544495, 0.0, 0.7063441276550293, 1.0, "assets/model/static/building/apartment/s01-e/1-frame_02.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    ent = staticModel( scen, 74.01870727539062, 39.77765655517578, -132.9451446533203, 0.0, 0.7078686356544495, 0.0, 0.7063441276550293, 1.0, "assets/model/static/building/apartment/s01-e/1-frame_01.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    ent = staticModel( scen, 74.01870727539062, 39.77765655517578, -132.9451446533203, 0.0, 0.7078686356544495, 0.0, 0.7063441276550293, 1.0, "assets/model/static/building/apartment/s01-e/1-frame_00.bgnome", "assets/texture/static/building/concrete-pattern-lightred_00.material", "" )
    ent = staticModel( scen, -3.061244487762451, -524.4407348632812, -53.85068130493164, 0.0, 0.6086393594741821, 0.0, 0.7934471368789673, 1.0, "assets/model/static/building/apartment/s01-e/2-windows_00.bgnome", "assets/texture/static/building/window-128_00.material", "" )
    ent = staticModel( scen, -3.061244487762451, -524.4407348632812, -53.85068130493164, 0.0, 0.6086393594741821, 0.0, 0.7934471368789673, 1.0, "assets/model/static/building/apartment/s01-e/2-main_00.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    ent = staticModel( scen, -3.061244487762451, -524.4407348632812, -53.85068130493164, 0.0, 0.6086393594741821, 0.0, 0.7934471368789673, 1.0, "assets/model/static/building/apartment/s01-e/2-frame_01.bgnome", "assets/texture/static/building/concrete-sand-32_00.material", "" )
    ent = staticModel( scen, -3.061244487762451, -524.4407348632812, -53.85068130493164, 0.0, 0.6086393594741821, 0.0, 0.7934471368789673, 1.0, "assets/model/static/building/apartment/s01-e/2-frame_00.bgnome", "assets/texture/static/building/concrete-64_01.material", "" )
    ent = staticModel( scen, -0.651397705078125, -163.6644287109375, -61.524173736572266, 0.0, 0.10180707275867462, 0.0, 0.9948042035102844, 1.0, "assets/model/static/building/apartment/s00-a/0-winFrames_00.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    ent = staticModel( scen, -0.651397705078125, -163.6644287109375, -61.524173736572266, 0.0, 0.10180707275867462, 0.0, 0.9948042035102844, 1.0, "assets/model/static/building/apartment/s00-a/0-windows_00.bgnome", "assets/texture/static/building/wincolor-64_00.material", "" )
    ent = staticModel( scen, -0.651397705078125, -163.6644287109375, -61.524173736572266, 0.0, 0.10180707275867462, 0.0, 0.9948042035102844, 1.0, "assets/model/static/building/apartment/s00-a/0-main_00.bgnome", "assets/texture/static/building/concrete-orange-32_00.material", "" )
    ent = staticModel( scen, -0.651397705078125, -163.6644287109375, -61.524173736572266, 0.0, 0.10180707275867462, 0.0, 0.9948042035102844, 1.0, "assets/model/static/building/apartment/s00-a/0-corners_00.bgnome", "assets/texture/static/building/concrete-64_01.material", "" )
    ent = staticModel( scen, -104.37144470214844, 76.68377685546875, -176.21974182128906, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s00-a/1-roof_00.bgnome", "assets/texture/static/building/concrete-128_02.material", "" )
    ent = staticModel( scen, -104.37144470214844, 76.68377685546875, -176.21974182128906, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s00-a/1-main_00.bgnome", "assets/texture/static/building/window-128_03.material", "" )
    ent = staticModel( scen, -104.37144470214844, 76.68377685546875, -176.21974182128906, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s00-a/1-corners_00.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )

    ent = empty( scen, "", -3.061244487762451, -524.4407348632812, -53.85068130493164, 0.5610517859458923, 0.43037301301956177, -0.4303729832172394, 0.5610518455505371, 2.061257839202881, 2.061257839202881, 2.061257839202881 )

    ent = empty( scen, "", 78.6101303100586, 66.74439239501953, -124.31913757324219, 0.7071068286895752, 0.0, 0.0, 0.7071068286895752, 2.061257839202881, 2.061257839202881, 2.061257839202881 )

end
return scen
