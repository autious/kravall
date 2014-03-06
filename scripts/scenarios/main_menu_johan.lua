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
    1.4430867433547974, -56.79637145996094, 107.96736145019531, 1.0 ),
    fov = 0.8575560450553894
}
scen.cameras["credits"] = {
    view = core.glm.mat4.new(
    -1.0, 1.1399866612488412e-14, -1.5099580252808664e-07, -0.0,
    -8.470329472543003e-22, 1.0, 7.549790126404332e-08, 0.0,
    1.5099580252808664e-07, 7.549790126404332e-08, -1.0, -0.0,
    -12.687170028686523, -41.83781814575195, -149.8191375732422, 1.0 ),
    fov = 0.8575560450553894
}
scen.cameras["main"] = {
    view = core.glm.mat4.new(
    -0.7218981981277466, -0.12016446143388748, -0.6814862489700317, 0.0,
    -1.4901161193847656e-07, 0.9848077297210693, -0.17364810407161713, 0.0,
    0.6919993162155151, -0.12535615265369415, -0.7109309434890747, -0.0,
    88.76730346679688, -76.83019256591797, -108.52535247802734, 1.0 ),
    fov = 0.8575560450553894
}
scen.cameras["scen"] = {
    view = core.glm.mat4.new(
    -0.6177421808242798, 0.2035302370786667, 0.759585440158844, -0.0,
    -1.4901161193847656e-07, 0.9659258127212524, -0.2588190734386444, 0.0,
    -0.7863807082176208, -0.15988357365131378, -0.5966930389404297, -0.0,
    -110.99596405029297, -57.01685333251953, -56.38139343261719, 1.0 ),
    fov = 0.8575560450553894
}
scen.cameras["setup"] = {
    view = core.glm.mat4.new(
    -1.0, -7.528547030233312e-08, 3.052437875794567e-07, -0.0,
    8.742277657347586e-08, 0.8660253882408142, 0.5000000596046448, -0.0,
    -3.019916050561733e-07, 0.5000000596046448, -0.8660253882408142, -0.0,
    -21.88218116760254, 12.470545768737793, -129.8861083984375, 1.0 ),
    fov = 0.8575560450553894
}

local script = dofile "scripts/scenarios/main_menu_johan_script.lua"( scen )

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
    
    directional( scen, 0.22932331264019012, -0.8682990074157715, 0.4398496448993683, 0.5165932178497314, 0.48119795322418213, 1.0, 0.5 )
    ambient( scen, 1.0, 0.898456335067749, 0.9377175569534302, 0.16423356533050537 )

    ent = spotLight( scen, -11.715843200683594, 42.981998443603516, -146.9362335205078, 0.0, -1.0, -0.0, 1.0, 1.0, 1.0, 0.47996556758880615, 0.23998278379440308, 5.0, 20.0, "" )
    ent = pointLight( scen, -13.942705154418945, 43.505279541015625, -102.88065338134766, 1.0, 1.0, 1.0, 10.0, 2.5, "" )
    ent = pointLight( scen, -13.942705154418945, 44.98103713989258, -98.64557647705078, 1.0, 1.0, 1.0, 10.0, 2.5, "" )
    ent = pointLight( scen, -29.992416381835938, 44.98103713989258, -98.64557647705078, 1.0, 1.0, 1.0, 10.0, 2.5, "" )
    ent = pointLight( scen, -29.992416381835938, 43.505279541015625, -102.88065338134766, 1.0, 1.0, 1.0, 10.0, 2.5, "" )
    ent = pointLight( scen, -34.598358154296875, 42.59711837768555, -104.84877014160156, 1.0, 1.0, 1.0, 10.0, 2.5, "" )
    ent = pointLight( scen, -39.16261672973633, 42.59711837768555, -104.84877014160156, 1.0, 1.0, 1.0, 10.0, 2.5, "" )
    ent = pointLight( scen, -43.13515090942383, 42.59711837768555, -104.84877014160156, 1.0, 1.0, 1.0, 10.0, 2.5, "" )
    ent = pointLight( scen, -9.081269264221191, 42.59711837768555, -104.84877014160156, 1.0, 1.0, 1.0, 10.0, 2.5, "" )
    ent = pointLight( scen, -5.108734130859375, 42.59711837768555, -104.84877014160156, 1.0, 1.0, 1.0, 10.0, 2.5, "" )
    ent = pointLight( scen, -0.5444750785827637, 42.59711837768555, -104.84877014160156, 1.0, 1.0, 1.0, 10.0, 2.5, "" )
    ent = spotLight( scen, -2.054462432861328, 59.042205810546875, -99.8989028930664, 0.0, 0.8660253882408142, 0.4999999701976776, 1.0, 1.0, 1.0, 0.6021386384963989, 0.30106931924819946, 10.889999389648438, 100.0, "" )
    ent = spotLight( scen, -41.37831115722656, 59.042205810546875, -99.8989028930664, 0.0, 0.8660253882408142, 0.4999999701976776, 1.0, 1.0, 1.0, 0.6021386384963989, 0.30106931924819946, 10.889999389648438, 100.0, "" )
    ent = spotLight( scen, -15.415824890136719, 42.920921325683594, -95.9466323852539, 0.0, 0.9953004121780396, 0.0968351811170578, 1.0, 1.0, 1.0, 0.15707969665527344, 0.07853984832763672, 14.359999656677246, 5000.0, "" )
    ent = spotLight( scen, -15.415824890136719, 42.920921325683594, -93.82017517089844, 0.0, 0.9969947934150696, -0.07746827602386475, 1.0, 1.0, 1.0, 0.15707969665527344, 0.07853984832763672, 14.359999656677246, 5000.0, "" )
    ent = spotLight( scen, -14.462173461914062, 42.920921325683594, -94.79899597167969, -0.07746827602386475, 0.9969947934150696, 7.11608938441799e-10, 1.0, 1.0, 1.0, 0.15707969665527344, 0.07853984832763672, 14.359999656677246, 5000.0, "" )
    ent = spotLight( scen, -16.58863067626953, 42.920921325683594, -94.79899597167969, 0.0968351811170578, 0.9953004121780396, 7.617010355787102e-10, 1.0, 1.0, 1.0, 0.15707969665527344, 0.07853984832763672, 14.359999656677246, 5000.0, "" )
    ent = spotLight( scen, -20.324331283569336, 42.920921325683594, -94.79899597167969, 0.0968351811170578, 0.9953004121780396, 7.617010355787102e-10, 1.0, 1.0, 1.0, 0.15707969665527344, 0.07853984832763672, 14.359999656677246, 5000.0, "" )
    ent = spotLight( scen, -18.197874069213867, 42.920921325683594, -94.79899597167969, -0.07746827602386475, 0.9969947934150696, 7.11608938441799e-10, 1.0, 1.0, 1.0, 0.15707969665527344, 0.07853984832763672, 14.359999656677246, 5000.0, "" )
    ent = spotLight( scen, -19.151525497436523, 42.920921325683594, -93.82017517089844, 0.0, 0.9969947934150696, -0.07746827602386475, 1.0, 1.0, 1.0, 0.15707969665527344, 0.07853984832763672, 14.359999656677246, 5000.0, "" )
    ent = spotLight( scen, -19.151525497436523, 42.920921325683594, -95.9466323852539, 0.0, 0.9953004121780396, 0.0968351811170578, 1.0, 1.0, 1.0, 0.15707969665527344, 0.07853984832763672, 14.359999656677246, 5000.0, "" )
    ent = spotLight( scen, -27.920957565307617, 42.920921325683594, -95.9466323852539, 0.0, 0.9953004121780396, 0.0968351811170578, 1.0, 1.0, 1.0, 0.15707969665527344, 0.07853984832763672, 14.359999656677246, 5000.0, "" )
    ent = spotLight( scen, -27.920957565307617, 42.920921325683594, -93.82017517089844, 0.0, 0.9969947934150696, -0.07746827602386475, 1.0, 1.0, 1.0, 0.15707969665527344, 0.07853984832763672, 14.359999656677246, 5000.0, "" )
    ent = spotLight( scen, -26.96730613708496, 42.920921325683594, -94.79899597167969, -0.07746827602386475, 0.9969947934150696, 7.11608938441799e-10, 1.0, 1.0, 1.0, 0.15707969665527344, 0.07853984832763672, 14.359999656677246, 5000.0, "" )
    ent = spotLight( scen, -29.09376335144043, 42.920921325683594, -94.79899597167969, 0.0968351811170578, 0.9953004121780396, 7.617010355787102e-10, 1.0, 1.0, 1.0, 0.15707969665527344, 0.07853984832763672, 14.359999656677246, 5000.0, "" )
    ent = spotLight( scen, -25.358062744140625, 42.920921325683594, -94.79899597167969, 0.0968351811170578, 0.9953004121780396, 7.617010355787102e-10, 1.0, 1.0, 1.0, 0.15707969665527344, 0.07853984832763672, 14.359999656677246, 5000.0, "" )
    ent = spotLight( scen, -23.231605529785156, 42.920921325683594, -94.79899597167969, -0.07746827602386475, 0.9969947934150696, 7.11608938441799e-10, 1.0, 1.0, 1.0, 0.15707969665527344, 0.07853984832763672, 14.359999656677246, 5000.0, "" )
    ent = spotLight( scen, -24.185256958007812, 42.920921325683594, -93.82017517089844, 0.0, 0.9969947934150696, -0.07746827602386475, 1.0, 1.0, 1.0, 0.15707969665527344, 0.07853984832763672, 14.359999656677246, 5000.0, "" )
    ent = spotLight( scen, -24.185256958007812, 42.920921325683594, -95.9466323852539, 0.0, 0.9953004121780396, 0.0968351811170578, 1.0, 1.0, 1.0, 0.15707969665527344, 0.07853984832763672, 14.359999656677246, 5000.0, "" )
    ent = spotLight( scen, -7.569960594177246, 64.1715087890625, -145.07205200195312, 0.0, -1.0, -0.0, 1.0, 1.0, 1.0, 0.39269915223121643, 0.19634957611560822, 7.3600006103515625, 300.0, "" )
    ent = spotLight( scen, -11.728903770446777, 64.09109497070312, -137.86912536621094, 0.0, -1.0, -0.0, 1.0, 1.0, 1.0, 0.39269915223121643, 0.19634957611560822, 7.3600006103515625, 300.0, "" )
    ent = spotLight( scen, -9.696067810058594, 64.11165618896484, -141.4661865234375, 0.0, -1.0, -0.0, 1.0, 1.0, 1.0, 0.39269915223121643, 0.19634957611560822, 7.3600006103515625, 300.0, "" )
    ent = spotLight( scen, -21.915454864501953, 48.323482513427734, -147.21881103515625, 0.0, -1.0, -0.0, 1.0, 1.0, 1.0, 1.1170107126235962, 0.5585053563117981, 23.5, 300.0, "" )
    ent = spotLight( scen, -21.915454864501953, 48.323482513427734, -129.46238708496094, 0.0, -1.0, -0.0, 1.0, 1.0, 1.0, 1.1170107126235962, 0.5585053563117981, 23.5, 300.0, "" )
    ent = pointLight( scen, -58.1622200012207, 79.53890228271484, -114.88455200195312, 1.0, 0.48000001907348633, 0.48000001907348633, 22.98400115966797, 20.0, "" )
    ent = pointLight( scen, -58.1622200012207, 70.60456848144531, -114.88455200195312, 1.0, 0.48000001907348633, 0.48000001907348633, 22.98400115966797, 20.0, "" )
    ent = pointLight( scen, -58.1622200012207, 59.632789611816406, -114.88455200195312, 1.0, 0.48000001907348633, 0.48000001907348633, 22.98400115966797, 20.0, "" )
    ent = pointLight( scen, -58.1622200012207, 46.969398498535156, -114.88455200195312, 1.0, 0.48000001907348633, 0.48000001907348633, 22.98400115966797, 20.0, "" )

    ent = staticModel( scen, -27.088611602783203, 39.798580169677734, -142.83042907714844, 0.0, 0.999954879283905, 0.0, 0.009501270018517971, 1.0, "assets/model/static/props/car_00.bgnome", "assets/texture/static/props/car_00.material", "" )
    ent = staticModel( scen, -27.088611602783203, 39.798580169677734, -135.72738647460938, 0.0, -0.9997078776359558, 0.0, 0.02417367324233055, 1.0, "assets/model/static/props/car_00.bgnome", "assets/texture/static/props/car_00.material", "" )
    ent = staticModel( scen, -10.184270858764648, 40.755531311035156, -147.21035766601562, 0.0, 0.2762138247489929, 0.0, 0.9610962271690369, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -10.184270858764648, 39.87614822387695, -147.54237365722656, 0.0, -0.2787208557128906, 0.0, 0.960372269153595, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -9.861145973205566, 39.87614822387695, -147.05177307128906, 0.0, 0.7173934578895569, 0.0, 0.696668267250061, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -10.515746116638184, 39.87614822387695, -147.05177307128906, 0.0, -0.3359968364238739, 0.0, 0.9418631196022034, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -10.913235664367676, 39.87614822387695, -151.291748046875, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/props/fence-grid_00.bgnome", "assets/texture/static/props/grid-32_00.material", "" )
    ent = staticModel( scen, -10.913235664367676, 39.87614822387695, -151.291748046875, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/props/fence-bars_00.bgnome", "assets/texture/static/props/rust-32_00.material", "" )
    ent = staticModel( scen, -11.717288970947266, 42.66191482543945, -146.87184143066406, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/props/lamp-0_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, -13.769952774047852, 43.31830596923828, -102.88330078125, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/props/lamp-1_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, -13.769952774047852, 44.794063568115234, -98.64822387695312, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/props/lamp-1_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, -30.16516876220703, 44.794063568115234, -98.64292907714844, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/props/lamp-1_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, -30.16516876220703, 43.31830596923828, -102.87800598144531, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/props/lamp-1_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, -34.5957145690918, 42.4101448059082, -104.67601776123047, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/props/lamp-1_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, -39.159976959228516, 42.4101448059082, -104.67601776123047, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/props/lamp-1_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, -43.132511138916016, 42.4101448059082, -104.67601776123047, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/props/lamp-1_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, -9.078628540039062, 42.4101448059082, -104.67601776123047, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/props/lamp-1_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, -5.106092929840088, 42.4101448059082, -104.67601776123047, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/props/lamp-1_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, -0.5418338775634766, 42.4101448059082, -104.67601776123047, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/props/lamp-1_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, -110.0176010131836, 39.95419692993164, -131.25242614746094, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-i/4-windows_00.bgnome", "assets/texture/static/building/window-cracked-128_00.material", "" )
    ent = staticModel( scen, -110.0176010131836, 39.95419692993164, -131.25242614746094, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-i/4-roof_00.bgnome", "assets/texture/static/building/roof-tiles-64_00.material", "" )
    ent = staticModel( scen, -110.0176010131836, 39.95419692993164, -131.25242614746094, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-i/4-main_00.bgnome", "assets/texture/static/building/brickwall-32_00.material", "" )
    ent = staticModel( scen, -110.0176010131836, 39.95419692993164, -131.25242614746094, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-i/4-frame_00.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    ent = staticModel( scen, -21.915454864501953, 48.67939376831055, -147.2177734375, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/props/hanging-street-light-short_00.bgnome", "assets/texture/static/props/hanging-street-light_00.material", "" )
    ent = staticModel( scen, -21.915454864501953, 48.67939376831055, -129.4613494873047, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/props/hanging-street-light-short_00.bgnome", "assets/texture/static/props/hanging-street-light_00.material", "" )
    ent = staticModel( scen, -51.009212493896484, 39.82963562011719, -151.8763427734375, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-corners_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-corners_00.material", "" )
    ent = staticModel( scen, -51.009212493896484, 39.82963562011719, -151.8763427734375, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-doors_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-doors_00.material", "" )
    ent = staticModel( scen, -51.009212493896484, 39.82963562011719, -151.8763427734375, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-foundation_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-foundation_00.material", "" )
    ent = staticModel( scen, -51.009212493896484, 39.82963562011719, -151.8763427734375, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-main-2-windows_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-main-2-windows_00.material", "" )
    ent = staticModel( scen, -51.009212493896484, 39.82963562011719, -151.8763427734375, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-main-3-windows_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-main-3-windows_00.material", "" )
    ent = staticModel( scen, -51.009212493896484, 39.82963562011719, -151.8763427734375, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-angle_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-angle_00.material", "" )
    ent = staticModel( scen, -51.009212493896484, 39.82963562011719, -151.8763427734375, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-top-edge_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-top-edge_00.material", "" )
    ent = staticModel( scen, -51.009212493896484, 39.82963562011719, -151.8763427734375, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-top-flat_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-top-flat_00.material", "" )
    ent = staticModel( scen, 6.054241180419922, 39.82963562011719, -228.08370971679688, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-top-flat_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-top-flat_00.material", "" )
    ent = staticModel( scen, 6.054241180419922, 39.82963562011719, -228.08370971679688, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-top-edge_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-top-edge_00.material", "" )
    ent = staticModel( scen, 6.054241180419922, 39.82963562011719, -228.08370971679688, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-angle_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-angle_00.material", "" )
    ent = staticModel( scen, 6.054241180419922, 39.82963562011719, -228.08370971679688, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-main-3-windows_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-main-3-windows_00.material", "" )
    ent = staticModel( scen, 6.054241180419922, 39.82963562011719, -228.08370971679688, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-main-2-windows_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-main-2-windows_00.material", "" )
    ent = staticModel( scen, 6.054241180419922, 39.82963562011719, -228.08370971679688, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-foundation_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-foundation_00.material", "" )
    ent = staticModel( scen, 6.054241180419922, 39.82963562011719, -228.08370971679688, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-doors_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-doors_00.material", "" )
    ent = staticModel( scen, 6.054241180419922, 39.82963562011719, -228.08370971679688, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-corners_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-corners_00.material", "" )
    ent = staticModel( scen, -21.915454864501953, 39.10649490356445, -84.24113464355469, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/building/special/s-01/embassy-windows_00.bgnome", "assets/texture/static/building/window-brown-128_00.material", "" )
    ent = staticModel( scen, -21.915454864501953, 39.10649490356445, -84.24113464355469, 7.549790126404332e-08, 1.0, -1.2300169461795536e-14, -1.6292068494294654e-07, 1.0, "assets/model/static/building/special/s-01/embassy-roof_00.bgnome", "assets/texture/static/building/concrete-64_01.material", "" )
    ent = staticModel( scen, -21.915454864501953, 39.10649490356445, -84.24113464355469, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/building/special/s-01/embassy-pillars_00.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    ent = staticModel( scen, -21.915454864501953, 39.10649490356445, -84.24113464355469, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/building/special/s-01/embassy-main_00.bgnome", "assets/texture/static/building/brickwall_00.material", "" )
    ent = staticModel( scen, -21.915454864501953, 39.10649490356445, -84.24113464355469, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/building/special/s-01/embassy-golditems_00.bgnome", "assets/texture/static/building/metal-gold-32_00.material", "" )
    ent = staticModel( scen, -21.915454864501953, 39.10649490356445, -84.24113464355469, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/building/special/s-01/embassy-flags_00.bgnome", "assets/texture/static/building/wincolor-64_00.material", "" )
    ent = staticModel( scen, 19.8924560546875, 40.16120910644531, -143.374267578125, 0.0, 1.0, 0.0, -7.549790126404332e-08, 1.0, "assets/model/static/building/apartment/s01-i/2-windows_00.bgnome", "assets/texture/static/building/window-with-bricks_00.material", "" )
    ent = staticModel( scen, 19.8924560546875, 40.16120910644531, -143.374267578125, 0.0, 1.0, 0.0, -7.549790126404332e-08, 1.0, "assets/model/static/building/apartment/s01-i/2-main_00.bgnome", "assets/texture/static/building/concrete-orange-32_00.material", "" )
    ent = staticModel( scen, 19.8924560546875, 40.16120910644531, -143.374267578125, 0.0, 1.0, 0.0, -7.549790126404332e-08, 1.0, "assets/model/static/building/apartment/s01-i/2-frame_00.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    ent = staticModel( scen, -8.211770057678223, 57.46324920654297, -140.67234802246094, 0.0, 0.49952176213264465, 0.0, 0.8663013577461243, 1.0, "assets/model/static/props/billboard-picture_01.bgnome", "assets/texture/static/props/billboard_01.material", "" )
    ent = staticModel( scen, -8.211770057678223, 57.46324920654297, -140.67234802246094, 0.0, 0.49952176213264465, 0.0, 0.8663013577461243, 1.0, "assets/model/static/props/billboard-frame_00.bgnome", "assets/texture/static/props/rust-32_00.material", "" )
    ent = staticModel( scen, -0.1317920684814453, 39.819480895996094, -129.74147033691406, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-d/3-windows.bgnome", "assets/texture/static/building/window-plain-brown_00.material", "" )
    ent = staticModel( scen, -0.1317920684814453, 39.819480895996094, -129.74147033691406, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-d/3-roof.bgnome", "assets/texture/static/building/concrete-64_01.material", "" )
    ent = staticModel( scen, -0.1317920684814453, 39.819480895996094, -129.74147033691406, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-d/3-pillars.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    ent = staticModel( scen, -0.1317920684814453, 39.819480895996094, -129.74147033691406, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-d/3-main.bgnome", "assets/texture/static/building/brickwall-32_00.material", "" )
    ent = staticModel( scen, -0.1317920684814453, 39.819480895996094, -129.74147033691406, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-d/3-door.bgnome", "assets/texture/static/building/garage-port-64_01.material", "" )
    ent = staticModel( scen, 0.24690628051757812, 39.77765655517578, -141.3160858154297, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-d/2-windows.bgnome", "assets/texture/static/building/window-plain-grey_00.material", "" )
    ent = staticModel( scen, 0.24690628051757812, 39.77765655517578, -141.3160858154297, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-d/2-roofedge.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    ent = staticModel( scen, 0.24690628051757812, 39.77765655517578, -141.3160858154297, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-d/2-main.bgnome", "assets/texture/static/building/concrete-sand-32_00.material", "" )
    ent = staticModel( scen, 0.24690628051757812, 39.77765655517578, -141.3160858154297, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-d/2-doorhandle.bgnome", "assets/texture/static/building/metal-grey_00.material", "" )
    ent = staticModel( scen, 0.24690628051757812, 39.77765655517578, -141.3160858154297, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-d/2-dooredge.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    ent = staticModel( scen, 0.24690628051757812, 39.77765655517578, -141.3160858154297, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-d/2-door.bgnome", "assets/texture/static/building/garage-port-64_01.material", "" )
    ent = staticModel( scen, 74.01870727539062, 39.77765655517578, -132.9451446533203, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/street/scenario1-street_00.bgnome", "assets/texture/static/street/street_00.material", "" )
    ent = staticModel( scen, 74.01870727539062, 39.77765655517578, -132.9451446533203, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/street/scenario1-square_00.bgnome", "assets/texture/static/street/asphalt_00.material", "" )
    ent = staticModel( scen, 74.01870727539062, 39.77765655517578, -132.9451446533203, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/street/scenario1-sidewalk_00.bgnome", "assets/texture/static/street/sidewalk_00.material", "" )
    ent = staticModel( scen, 74.01870727539062, 39.77765655517578, -132.9451446533203, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/street/scenario1-grounds_00.bgnome", "assets/texture/static/street/asphalt_00.material", "" )
    ent = staticModel( scen, -78.28092956542969, 40.081138610839844, -143.72596740722656, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s00-a/1-roof_00.bgnome", "assets/texture/static/building/concrete-128_02.material", "" )
    ent = staticModel( scen, -78.28092956542969, 40.081138610839844, -143.72596740722656, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s00-a/1-main_00.bgnome", "assets/texture/static/building/window-128_03.material", "" )
    ent = staticModel( scen, -78.28092956542969, 40.081138610839844, -143.72596740722656, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s00-a/1-corners_00.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )

    ent = decal( scen, -11.71988582611084, 41.44377136230469, -146.75645446777344, 2.8378977390275395e-07, -0.7071068286895752, 0.7071068286895752, -2.837898023244634e-07, 1.0927678346633911, 1.0927678346633911, 2.1855356693267822, "assets/texture/decal/names.material", "" )
    ent = decal( scen, -58.136470794677734, 63.25982666015625, -121.54927062988281, 0.7071068286895752, -1.685874053691805e-07, -6.181724643283815e-08, 0.7071067690849304, 11.616280555725098, 11.616280555725098, 23.232561111450195, "assets/texture/decal/names.material", "" )
end
return scen
