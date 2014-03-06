local entities = require "entities"
local scenario = require "scenario"
local scen = scenario.new()

scen.name = "Default Scenario Name"
scen.description = "Default Description"

scen.cameras = scen.cameras or {}
scen.cameras["settings1"] = {
    view = core.glm.mat4.new(
    -1.3907092011322675e-07, -7.549795810746218e-08, -1.0, 0.0,
    -3.8941436741879443e-07, 1.0, -7.549789415861596e-08, 0.0,
    1.0, 3.8941436741879443e-07, -1.3907094853493618e-07, -0.0,
    155.24668884277344, -2.733031988143921, 4.756381034851074, 1.0 ),
    fov = 0.8575560450553894
}
scen.cameras["settings"] = {
    view = core.glm.mat4.new(
    -1.0, -3.01991519791045e-07, 5.642600058308744e-07, -0.0,
    5.642599489874556e-07, 1.5099580252808664e-07, 1.0, -0.0,
    -3.019916050561733e-07, 1.0, -1.5099563199783006e-07, -0.0,
    -1.4431178569793701, 156.48533630371094, -15.641578674316406, 1.0 ),
    fov = 0.8575560450553894
}
scen.cameras["credits"] = {
    view = core.glm.mat4.new(
    -1.0, 1.1399866612488412e-14, -1.5099580252808664e-07, -0.0,
    -8.470329472543003e-22, 1.0, 7.549790126404332e-08, 0.0,
    1.5099580252808664e-07, 7.549790126404332e-08, -1.0, -0.0,
    -12.687170028686523, -1.486014485359192, -149.8191375732422, 1.0 ),
    fov = 0.8575560450553894
}
scen.cameras["main"] = {
    view = core.glm.mat4.new(
    -0.7218981981277466, -0.12016446143388748, -0.6814862489700317, 0.0,
    -1.4901161193847656e-07, 0.9848077297210693, -0.17364810407161713, 0.0,
    0.6919993162155151, -0.12535615265369415, -0.7109309434890747, -0.0,
    88.76730346679688, -37.09142303466797, -115.53237915039062, 1.0 ),
    fov = 0.8575560450553894
}
scen.cameras["scen"] = {
    view = core.glm.mat4.new(
    -0.6177421808242798, 0.13655304908752441, 0.7744339108467102, -0.0,
    -1.6391277313232422e-07, 0.9848078489303589, -0.17364764213562012, 0.0,
    -0.7863807082176208, -0.10726958513259888, -0.6083573698997498, -0.0,
    -110.9959716796875, -12.147113800048828, -68.14321899414062, 1.0 ),
    fov = 0.8575560450553894
}
scen.cameras["setup"] = {
    view = core.glm.mat4.new(
    -1.0, -7.528547030233312e-08, 3.052437875794567e-07, -0.0,
    8.742277657347586e-08, 0.8660253882408142, 0.5000000596046448, -0.0,
    -3.019916050561733e-07, 0.5000000596046448, -0.8660253882408142, -0.0,
    -21.882179260253906, 47.416229248046875, -109.710205078125, 1.0 ),
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
    
    scen.asm:specific_content( core.contentmanager.load( core.loaders.NavigationMeshLoader, "scripts/scenarios/main_menu_johan.nav", function( value ) end, false ) )

    
    local function genF( e, f )
        if type( script[f] ) == "function"  then
            return function( delta ) script[ f ]( e, delta ) end
        else
            error( "Function " .. f .. " does not exist in script file" )
        end
    end
    
    directional( scen, 0.22932331264019012, -0.8682990074157715, 0.4398496448993683, 0.5165932178497314, 0.48119795322418213, 1.0, 0.25 )
    ambient( scen, 1.0, 0.898456335067749, 0.9377175569534302, 0.16423356533050537 )

    ent = area( scen, {-56.9846076965332, 0, -115.30433654785156}, {-53.02605056762695, 8.605384826660156, -53.02605056762695, -8.605384826660156, 53.026058197021484, -8.605384826660156, 53.026058197021484, 8.605384826660156}, "" )
    scen:registerInitCallback( genF( ent, "spawn" ) )

    ent = area( scen, {-101.01323699951172, 0, -67.68685913085938}, {-9.243232727050781, 7.0267791748046875, -9.243232727050781, -7.0267791748046875, 9.243232727050781, -7.0267791748046875, 9.243232727050781, 7.0267791748046875}, "" )
    scen:registerInitCallback( genF( ent, "registerEnd" ) )
    scen:registerTickCallback( genF( ent, "checkEndEnter" ) )

    ent = area( scen, {12.515791893005371, 0, -114.99401092529297}, {-7.12856388092041, 8.605384826660156, -7.12856388092041, -8.605384826660156, 7.128564834594727, -8.605384826660156, 7.128564834594727, 8.605384826660156}, "" )
    scen:registerInitCallback( genF( ent, "registerStart" ) )

    ent = spotLight( scen, 12.627717018127441, 4.227061748504639, -148.62586975097656, 0.0, -1.0, -0.0, 1.0, 1.0, 1.0, 0.890117883682251, 0.4450589418411255, 8.649999618530273, 5.0, "" )
    ent = spotLight( scen, 12.627717018127441, 4.227061748504639, -151.23536682128906, 0.0, -1.0, -0.0, 1.0, 1.0, 1.0, 0.890117883682251, 0.4450589418411255, 8.649999618530273, 5.0, "" )
    ent = spotLight( scen, 12.627717018127441, 4.227061748504639, -160.33950805664062, 0.0, -1.0, -0.0, 1.0, 1.0, 1.0, 0.890117883682251, 0.4450589418411255, 8.649999618530273, 5.0, "" )
    ent = spotLight( scen, 12.627717018127441, 4.227061748504639, -156.88363647460938, 0.0, -1.0, -0.0, 1.0, 1.0, 1.0, 0.890117883682251, 0.4450589418411255, 8.649999618530273, 5.0, "" )
    ent = spotLight( scen, 12.627717018127441, 4.227061748504639, -153.9913330078125, 0.0, -1.0, -0.0, 1.0, 1.0, 1.0, 0.890117883682251, 0.4450589418411255, 8.649999618530273, 5.0, "" )
    ent = pointLight( scen, 11.377090454101562, -0.4581270217895508, -150.3014373779297, 0.0, 0.6769230365753174, 0.0, 3.0, 5.0, "" )
    ent = pointLight( scen, -14.167986869812012, -5000.0, -142.3576202392578, 0.5165929794311523, 0.4811980128288269, 1.0, 20000.0, 1200000.0, "" )
    ent = pointLight( scen, -14.084942817687988, 14.715034484863281, -124.16606903076172, 0.0, 1.0, 0.0, 5.0, 2.0, "" )
    ent = pointLight( scen, -14.084942817687988, 12.71286392211914, -124.16606903076172, 0.0, 1.0, 1.0, 5.0, 2.0, "" )
    ent = pointLight( scen, -14.084942817687988, 16.281099319458008, -124.17384338378906, 1.0, 0.0, 0.0, 5.0, 2.0, "" )
    ent = pointLight( scen, -14.084942817687988, 14.270798683166504, -124.48368835449219, 1.0, 0.0, 0.9130401611328125, 10.270000457763672, 5.0, "" )
    ent = pointLight( scen, -14.084942817687988, 20.900819778442383, -124.48368835449219, 1.0, 1.0, 0.0, 10.270000457763672, 5.0, "" )
    ent = spotLight( scen, -4.880334854125977, 3.6279456615448, -165.8069305419922, 0.0, -1.0, -0.0, 1.0, 1.0, 1.0, 1.0646507740020752, 0.5323253870010376, 8.829999923706055, 20.0, "" )
    ent = pointLight( scen, -2.267425298690796, 1.850060224533081, -163.9417266845703, 0.0, 0.6769230365753174, 0.0, 1.0, 5.0, "" )
    ent = pointLight( scen, -8.45417594909668, 0.14230048656463623, -148.06021118164062, 0.0, 0.6769230365753174, 0.0, 1.0, 5.0, "" )
    ent = pointLight( scen, -8.567768096923828, 0.44745561480522156, -147.95579528808594, 0.0, 0.6769230365753174, 0.0, 1.0, 5.0, "" )
    ent = pointLight( scen, -8.296972274780273, -0.45056721568107605, -148.11923217773438, 0.0, 0.6769230365753174, 0.0, 1.0, 5.0, "" )
    ent = pointLight( scen, -9.768746376037598, -0.46908846497535706, -149.0665283203125, 0.0, 0.6769230365753174, 0.0, 1.0, 5.0, "" )
    ent = spotLight( scen, -7.813108444213867, 4.71922492980957, -156.65074157714844, 0.0, -1.0, -0.0, 1.0, 1.0, 1.0, 1.0821040868759155, 0.5410520434379578, 11.769999504089355, 100.0, "" )
    ent = spotLight( scen, 6.458413124084473, 4.71922492980957, -156.65074157714844, 0.0, -1.0, -0.0, 1.0, 1.0, 1.0, 1.0821040868759155, 0.5410520434379578, 11.769999504089355, 100.0, "" )
    ent = spotLight( scen, -11.715843200683594, 2.630194664001465, -146.9362335205078, 0.0, -1.0, -0.0, 1.0, 1.0, 1.0, 0.6544984579086304, 0.3272492289543152, 5.0, 20.0, "" )
    ent = pointLight( scen, -13.942705154418945, 3.153475761413574, -102.88065338134766, 0.9692310094833374, 0.9538459777832031, 0.6948720216751099, 10.0, 2.5, "" )
    ent = pointLight( scen, -13.942705154418945, 4.629233360290527, -98.64557647705078, 0.9692310094833374, 0.9538459777832031, 0.6948720216751099, 10.0, 2.5, "" )
    ent = pointLight( scen, -29.992416381835938, 4.629233360290527, -98.64557647705078, 0.9692310094833374, 0.9538459777832031, 0.6948720216751099, 10.0, 2.5, "" )
    ent = pointLight( scen, -29.992416381835938, 3.153475761413574, -102.88065338134766, 0.9692310094833374, 0.9538459777832031, 0.6948720216751099, 10.0, 2.5, "" )
    ent = pointLight( scen, -34.598358154296875, 2.245314598083496, -104.84877014160156, 0.9692310094833374, 0.9538459777832031, 0.6948720216751099, 10.0, 2.5, "" )
    ent = pointLight( scen, -39.16261672973633, 2.245314598083496, -104.84877014160156, 0.9692310094833374, 0.9538459777832031, 0.6948720216751099, 10.0, 2.5, "" )
    ent = pointLight( scen, -43.13515090942383, 2.245314598083496, -104.84877014160156, 0.9692310094833374, 0.9538459777832031, 0.6948720216751099, 10.0, 2.5, "" )
    ent = pointLight( scen, -9.081269264221191, 2.245314598083496, -104.84877014160156, 0.9692310094833374, 0.9538459777832031, 0.6948720216751099, 10.0, 2.5, "" )
    ent = pointLight( scen, -5.108734130859375, 2.245314598083496, -104.84877014160156, 0.9692310094833374, 0.9538459777832031, 0.6948720216751099, 10.0, 2.5, "" )
    ent = pointLight( scen, -0.5444750785827637, 2.245314598083496, -104.84877014160156, 0.9692310094833374, 0.9538459777832031, 0.6948720216751099, 10.0, 2.5, "" )
    ent = spotLight( scen, -2.054462432861328, 18.69040298461914, -99.8989028930664, 0.0, 0.8660253882408142, 0.4999999701976776, 1.0, 1.0, 1.0, 0.6021386384963989, 0.30106931924819946, 10.889999389648438, 100.0, "" )
    ent = spotLight( scen, -41.37831115722656, 18.69040298461914, -99.8989028930664, 0.0, 0.8660253882408142, 0.4999999701976776, 1.0, 1.0, 1.0, 0.6021386384963989, 0.30106931924819946, 10.889999389648438, 100.0, "" )
    ent = spotLight( scen, -15.517455101013184, 2.569117546081543, -95.9466323852539, 0.0, 0.9953004121780396, 0.0968351811170578, 0.9692307114601135, 0.9538461565971375, 0.6948717832565308, 0.09599314630031586, 0.04799657315015793, 24.739999771118164, 25000.0, "" )
    ent = spotLight( scen, -15.415824890136719, 2.569117546081543, -93.82017517089844, 0.0, 0.9969947934150696, -0.07746827602386475, 0.9692307114601135, 0.9538461565971375, 0.6948717832565308, 0.09599314630031586, 0.04799657315015793, 24.739999771118164, 25000.0, "" )
    ent = spotLight( scen, -14.462173461914062, 2.569117546081543, -94.79899597167969, -0.07746827602386475, 0.9969947934150696, 7.11608938441799e-10, 0.9692307114601135, 0.9538461565971375, 0.6948717832565308, 0.09599314630031586, 0.04799657315015793, 24.739999771118164, 25000.0, "" )
    ent = spotLight( scen, -16.58863067626953, 2.569117546081543, -94.79899597167969, 0.0968351811170578, 0.9953004121780396, 7.617010355787102e-10, 0.9692307114601135, 0.9538461565971375, 0.6948717832565308, 0.09599314630031586, 0.04799657315015793, 24.739999771118164, 25000.0, "" )
    ent = spotLight( scen, -20.324331283569336, 2.569117546081543, -94.79899597167969, 0.0968351811170578, 0.9953004121780396, 7.617010355787102e-10, 0.9692307114601135, 0.9538461565971375, 0.6948717832565308, 0.09599314630031586, 0.04799657315015793, 24.739999771118164, 25000.0, "" )
    ent = spotLight( scen, -18.197874069213867, 2.569117546081543, -94.79899597167969, -0.07746827602386475, 0.9969947934150696, 7.11608938441799e-10, 0.9692307114601135, 0.9538461565971375, 0.6948717832565308, 0.09599314630031586, 0.04799657315015793, 24.739999771118164, 25000.0, "" )
    ent = spotLight( scen, -19.151525497436523, 2.569117546081543, -93.82017517089844, 0.0, 0.9969947934150696, -0.07746827602386475, 0.9692307114601135, 0.9538461565971375, 0.6948717832565308, 0.09599314630031586, 0.04799657315015793, 24.739999771118164, 25000.0, "" )
    ent = spotLight( scen, -19.30672836303711, 2.569117546081543, -95.9466323852539, 0.0, 0.9953004121780396, 0.0968351811170578, 0.9692307114601135, 0.9538461565971375, 0.6948717832565308, 0.09599314630031586, 0.04799657315015793, 24.739999771118164, 25000.0, "" )
    ent = spotLight( scen, -27.953760147094727, 2.569117546081543, -95.9466323852539, 0.0, 0.9953004121780396, 0.0968351811170578, 0.9692307114601135, 0.9538461565971375, 0.6948717832565308, 0.09599314630031586, 0.04799657315015793, 24.739999771118164, 25000.0, "" )
    ent = spotLight( scen, -27.920957565307617, 2.569117546081543, -93.82017517089844, 0.0, 0.9969947934150696, -0.07746827602386475, 0.9692307114601135, 0.9538461565971375, 0.6948717832565308, 0.09599314630031586, 0.04799657315015793, 24.739999771118164, 25000.0, "" )
    ent = spotLight( scen, -26.96730613708496, 2.569117546081543, -94.79899597167969, -0.07746827602386475, 0.9969947934150696, 7.11608938441799e-10, 0.9692307114601135, 0.9538461565971375, 0.6948717832565308, 0.09599314630031586, 0.04799657315015793, 24.739999771118164, 25000.0, "" )
    ent = spotLight( scen, -29.09376335144043, 2.569117546081543, -94.79899597167969, 0.0968351811170578, 0.9953004121780396, 7.617010355787102e-10, 0.9692307114601135, 0.9538461565971375, 0.6948717832565308, 0.09599314630031586, 0.04799657315015793, 24.739999771118164, 25000.0, "" )
    ent = spotLight( scen, -25.358062744140625, 2.569117546081543, -94.79899597167969, 0.0968351811170578, 0.9953004121780396, 7.617010355787102e-10, 0.9692307114601135, 0.9538461565971375, 0.6948717832565308, 0.09599314630031586, 0.04799657315015793, 24.739999771118164, 25000.0, "" )
    ent = spotLight( scen, -23.231605529785156, 2.569117546081543, -94.79899597167969, -0.07746827602386475, 0.9969947934150696, 7.11608938441799e-10, 0.9692307114601135, 0.9538461565971375, 0.6948717832565308, 0.09599314630031586, 0.04799657315015793, 24.739999771118164, 25000.0, "" )
    ent = spotLight( scen, -24.185256958007812, 2.569117546081543, -93.82017517089844, 0.0, 0.9969947934150696, -0.07746827602386475, 0.9692307114601135, 0.9538461565971375, 0.6948717832565308, 0.09599314630031586, 0.04799657315015793, 24.739999771118164, 25000.0, "" )
    ent = spotLight( scen, -24.233837127685547, 2.569117546081543, -95.9466323852539, 0.0, 0.9953004121780396, 0.0968351811170578, 0.9692307114601135, 0.9538461565971375, 0.6948717832565308, 0.09599314630031586, 0.04799657315015793, 24.739999771118164, 25000.0, "" )
    ent = spotLight( scen, -7.569960594177246, 23.819705963134766, -145.07205200195312, 0.0, -1.0, -0.0, 1.0, 1.0, 1.0, 0.39269915223121643, 0.19634957611560822, 7.3600006103515625, 300.0, "" )
    ent = spotLight( scen, -11.728903770446777, 23.73929214477539, -137.86912536621094, 0.0, -1.0, -0.0, 1.0, 1.0, 1.0, 0.39269915223121643, 0.19634957611560822, 7.3600006103515625, 300.0, "" )
    ent = spotLight( scen, -9.696067810058594, 23.75985336303711, -141.4661865234375, 0.0, -1.0, -0.0, 1.0, 1.0, 1.0, 0.39269915223121643, 0.19634957611560822, 7.3600006103515625, 300.0, "" )
    ent = spotLight( scen, -21.915454864501953, 7.971678733825684, -147.21881103515625, 0.0, -1.0, -0.0, 1.0, 1.0, 1.0, 1.1170107126235962, 0.5585053563117981, 23.5, 300.0, "" )
    ent = spotLight( scen, -21.913799285888672, 7.983826637268066, -129.47755432128906, 0.0, -1.0, -0.0, 1.0, 1.0, 1.0, 1.1170107126235962, 0.5585053563117981, 23.5, 300.0, "" )

    ent = staticModel( scen, 12.754565238952637, 3.8747591972351074, -148.62506103515625, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/props/lamp-0_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, 12.754565238952637, 3.8747591972351074, -151.23455810546875, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/props/lamp-0_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, 12.754565238952637, 3.8747591972351074, -160.3386993408203, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/props/lamp-0_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, 12.754565238952637, 3.8747591972351074, -156.88282775878906, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/props/lamp-0_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, 12.754565238952637, 3.8747591972351074, -153.9905242919922, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/props/lamp-0_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, -13.10014533996582, 24.34073257446289, -134.21604919433594, 0.0, 0.12712323665618896, 0.0, 0.9918869733810425, 1.0, "assets/model/static/props/antenn-grid_00.bgnome", "assets/texture/static/props/fence_00.material", "" )
    ent = staticModel( scen, -13.10014533996582, 24.34073257446289, -134.21604919433594, 0.0, 0.12712323665618896, 0.0, 0.9918869733810425, 1.0, "assets/model/static/props/antenn-foot_00.bgnome", "assets/texture/static/building/metal-black_00.material", "" )
    ent = staticModel( scen, -13.238912582397461, -0.5013793706893921, -145.9585723876953, 0.0, -0.13212117552757263, 0.0, 0.9912336468696594, 1.0, "assets/model/static/props/tyre_00.bgnome", "assets/texture/static/props/tyre_00.material", "" )
    ent = staticModel( scen, -13.238912582397461, -0.5064764022827148, -141.45286560058594, 0.0, -0.13212117552757263, 0.0, 0.9912336468696594, 1.0, "assets/model/static/props/tyre_00.bgnome", "assets/texture/static/props/tyre_00.material", "" )
    ent = staticModel( scen, -13.238912582397461, -0.5063811540603638, -140.54400634765625, 0.0, -0.22706623375415802, 0.0, 0.973879337310791, 1.0, "assets/model/static/props/tyre-pile_00.bgnome", "assets/texture/static/props/tyre_00.material", "" )
    ent = staticModel( scen, 12.004500389099121, -0.47547364234924316, -154.42466735839844, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/props/pipe/pipe_00.bgnome", "assets/texture/static/props/pipe/pipe_00.material", "" )
    ent = staticModel( scen, -17.16390609741211, 2.0615453720092773, -92.79691314697266, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/props/double-door_00.bgnome", "assets/texture/static/props/door_00.material", "" )
    ent = staticModel( scen, -26.37900161743164, 2.0406060218811035, -92.79691314697266, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/props/double-door_00.bgnome", "assets/texture/static/props/door_00.material", "" )
    ent = staticModel( scen, -21.704269409179688, 2.0406060218811035, -92.79691314697266, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/props/double-door_00.bgnome", "assets/texture/static/props/door_00.material", "" )
    ent = staticModel( scen, -6.433355331420898, 13.391511917114258, -146.8525848388672, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/props/neon-bigb-picture_01.bgnome", "assets/texture/static/props/neon-bigb_01.material", "" )
    ent = staticModel( scen, -6.433355331420898, 13.391511917114258, -146.8525848388672, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/props/neon-bigb-frame_00.bgnome", "assets/texture/static/props/rusty-metal_00.material", "" )
    ent = staticModel( scen, -13.512086868286133, 11.48553466796875, -123.81108093261719, 0.0, -0.3826834261417389, 0.0, 0.9238795638084412, 1.0, "assets/model/static/props/neon-frame_00.bgnome", "assets/texture/static/building/metal-black_00.material", "" )
    ent = staticModel( scen, -13.512086868286133, 11.48553466796875, -123.81108093261719, 0.0, -0.3826834261417389, 0.0, 0.9238795638084412, 1.0, "assets/model/static/props/neon-picture_02.bgnome", "assets/texture/static/props/neon_signs.material", "" )
    ent = staticModel( scen, -13.512086868286133, 17.43429183959961, -123.81108093261719, 0.0, -0.3826834261417389, 0.0, 0.9238795638084412, 1.0, "assets/model/static/props/neon-picture_00.bgnome", "assets/texture/static/props/neon_signs.material", "" )
    ent = staticModel( scen, -13.512086868286133, 17.43429183959961, -123.81108093261719, 0.0, -0.3826834261417389, 0.0, 0.9238795638084412, 1.0, "assets/model/static/props/neon-frame_00.bgnome", "assets/texture/static/building/metal-black_00.material", "" )
    ent = staticModel( scen, -4.878889083862305, 3.3078620433807373, -165.87132263183594, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/props/lamp-0_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, 4.667056083679199, -0.48589229583740234, -163.82772827148438, 0.0, 0.09019181877374649, 0.0, 0.9959244132041931, 1.0, "assets/model/static/props/crate_00.bgnome", "assets/texture/static/props/crate_00.material", "" )
    ent = staticModel( scen, 5.78018856048584, -0.48589229583740234, -163.2451629638672, 0.0, 0.4801289439201355, 0.0, 0.8771979212760925, 1.0, "assets/model/static/props/crate_00.bgnome", "assets/texture/static/props/crate_00.material", "" )
    ent = staticModel( scen, -1.1159332990646362, 0.7783305644989014, -163.51596069335938, -0.5282564163208008, 0.19594942033290863, -0.1345527023077011, 0.8151347041130066, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -1.8227019309997559, 0.9730224609375, -163.80538940429688, 0.08843308687210083, -0.4118688702583313, -0.26885467767715454, 0.8661759495735168, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -2.561147689819336, 0.8818552494049072, -164.07650756835938, -0.1778564751148224, 0.1095791682600975, 0.1327870935201645, 0.9688792824745178, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -9.679037094116211, 0.3733450770378113, -148.2616729736328, 0.0, 0.7870278358459473, 0.0, 0.6169174909591675, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -9.994349479675293, -0.47565555572509766, -148.36566162109375, 0.0, 0.33693721890449524, 0.0, 0.9415271282196045, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -9.427231788635254, -0.5060380101203918, -148.5188751220703, 0.0, 0.9895609617233276, 0.0, 0.1441153883934021, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -9.632245063781738, -0.5060380101203918, -147.89720153808594, 0.0, 0.2796798050403595, 0.0, 0.9600934386253357, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -8.772928237915039, -0.5060380101203918, -147.1021728515625, 0.0, 0.2796798050403595, 0.0, 0.9600934386253357, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -8.567914962768555, -0.5060380101203918, -147.72384643554688, 0.0, 0.9895609617233276, 0.0, 0.1441153883934021, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -9.135032653808594, -0.5060380101203918, -147.5706329345703, 0.0, 0.33693721890449524, 0.0, 0.9415271282196045, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -8.819720268249512, 0.3733450770378113, -147.46664428710938, 0.0, 0.7870278358459473, 0.0, 0.6169174909591675, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, 6.02634334564209, 0.4876558780670166, -164.0121307373047, 0.6784793734550476, -0.6784793734550476, -0.19916261732578278, 0.19916261732578278, 1.0, "assets/model/static/props/container-empty_00.bgnome", "assets/texture/static/props/container_00.material", "" )
    ent = staticModel( scen, -2.3320467472076416, -0.3462996482849121, -163.9323272705078, 0.0, -0.7553961873054504, 0.0, 0.6552683711051941, 1.0, "assets/model/static/props/container-filled_00.bgnome", "assets/texture/static/props/container_00.material", "" )
    ent = staticModel( scen, -6.8481574058532715, -0.3462996482849121, -162.97605895996094, 0.0, -0.9906652569770813, 0.0, 0.13631752133369446, 1.0, "assets/model/static/props/container-filled_00.bgnome", "assets/texture/static/props/container_00.material", "" )
    ent = staticModel( scen, 9.8760347366333, 0.6510933637619019, -163.6197509765625, 0.0, -0.6674691438674927, 0.0, 0.7446374893188477, 1.0, "assets/model/static/props/crate_00.bgnome", "assets/texture/static/props/crate_00.material", "" )
    ent = staticModel( scen, 9.365440368652344, -0.3462997078895569, -162.85536193847656, 0.0, -0.33157533407211304, 0.0, 0.9434287548065186, 1.0, "assets/model/static/props/crate_00.bgnome", "assets/texture/static/props/crate_00.material", "" )
    ent = staticModel( scen, 9.31208610534668, -0.3462997078895569, -164.32118225097656, 0.0, -0.5045278072357178, 0.0, 0.8633954524993896, 1.0, "assets/model/static/props/crate_00.bgnome", "assets/texture/static/props/crate_00.material", "" )
    ent = staticModel( scen, 10.574291229248047, -0.3462997078895569, -164.037353515625, 0.0, -0.0860016867518425, 0.0, 0.9962950348854065, 1.0, "assets/model/static/props/crate_00.bgnome", "assets/texture/static/props/crate_00.material", "" )
    ent = staticModel( scen, -9.448602676391602, -0.03664170205593109, -163.7997283935547, 0.0, -0.8236848711967468, 0.0, 0.5670478343963623, 1.0, "assets/model/static/props/crate_01.bgnome", "assets/texture/static/props/crate_01.material", "" )
    ent = staticModel( scen, -9.435407638549805, -0.3462997078895569, -163.43714904785156, 0.0, 0.1248961016535759, 0.0, 0.9921698570251465, 1.0, "assets/model/static/props/crate_01.bgnome", "assets/texture/static/props/crate_01.material", "" )
    ent = staticModel( scen, -9.310093879699707, -0.3462997078895569, -164.13987731933594, 0.0, -0.06618897616863251, 0.0, 0.9978071451187134, 1.0, "assets/model/static/props/crate_01.bgnome", "assets/texture/static/props/crate_01.material", "" )
    ent = staticModel( scen, 11.845440864562988, -0.3462997078895569, -161.90884399414062, 0.0, 0.12831640243530273, 0.0, 0.9917333126068115, 1.0, "assets/model/static/props/crate_00.bgnome", "assets/texture/static/props/crate_00.material", "" )
    ent = staticModel( scen, 6.461875915527344, 5.114901542663574, -156.64028930664062, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/props/hanging-street-light-short_00.bgnome", "assets/texture/static/props/hanging-street-light_00.material", "" )
    ent = staticModel( scen, -7.873801231384277, 5.114901542663574, -156.64028930664062, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/props/hanging-street-light-short_00.bgnome", "assets/texture/static/props/hanging-street-light_00.material", "" )
    ent = staticModel( scen, -10.913235664367676, -0.47565555572509766, -161.56146240234375, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/props/fence-bars_00.bgnome", "assets/texture/static/props/rust-32_00.material", "" )
    ent = staticModel( scen, -10.913235664367676, -0.47565555572509766, -161.56146240234375, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/props/fence-grid_00.bgnome", "assets/texture/static/props/grid-32_00.material", "" )
    ent = staticModel( scen, 8.183589935302734, -0.49974536895751953, -186.28811645507812, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-b/1-frame.bgnome", "assets/texture/static/building/concrete-sand-32_00.material", "" )
    ent = staticModel( scen, 8.183589935302734, -0.49974536895751953, -186.28811645507812, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-b/1-main.bgnome", "assets/texture/static/building/brickwall-32_01.material", "" )
    ent = staticModel( scen, 8.183589935302734, -0.49974536895751953, -186.95057678222656, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-b/1-windows.bgnome", "assets/texture/static/building/window-sandframe-64_00.material", "" )
    ent = staticModel( scen, -34.471858978271484, 43.9116096496582, -27.171674728393555, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-b/1-windows.bgnome", "assets/texture/static/building/window-sandframe-64_00.material", "" )
    ent = staticModel( scen, -34.471858978271484, 43.9116096496582, -27.171674728393555, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-b/1-main.bgnome", "assets/texture/static/building/brickwall-32_01.material", "" )
    ent = staticModel( scen, -34.471858978271484, 43.9116096496582, -27.171674728393555, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-b/1-frame.bgnome", "assets/texture/static/building/concrete-sand-32_00.material", "" )
    ent = staticModel( scen, -12.836615562438965, -0.2449960708618164, -137.7711181640625, 0.42898884415626526, -0.03052837960422039, 0.5158399939537048, 0.740908682346344, 1.0, "assets/model/static/props/garbage-bin_00.bgnome", "assets/texture/static/props/garbage-bin_00.material", "" )
    ent = staticModel( scen, -12.836615562438965, -0.5077714920043945, -138.43582153320312, 0.0, -0.359243243932724, 0.0, 0.9332439303398132, 1.0, "assets/model/static/props/garbage-bin_00.bgnome", "assets/texture/static/props/garbage-bin_00.material", "" )
    ent = staticModel( scen, -12.846076011657715, -0.5077714920043945, -139.27252197265625, 0.0, 0.23387359082698822, 0.0, 0.9722670316696167, 1.0, "assets/model/static/props/garbage-bin_00.bgnome", "assets/texture/static/props/garbage-bin_00.material", "" )
    ent = staticModel( scen, -129.79592895507812, -0.5221681594848633, -45.6254768371582, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-top-flat_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-top-flat_00.material", "" )
    ent = staticModel( scen, -129.79592895507812, -0.5221681594848633, -45.6254768371582, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-top-edge_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-top-edge_00.material", "" )
    ent = staticModel( scen, -129.79592895507812, -0.5221681594848633, -45.6254768371582, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-angle_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-angle_00.material", "" )
    ent = staticModel( scen, -129.79592895507812, -0.5221681594848633, -45.6254768371582, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-main-3-windows_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-main-3-windows_00.material", "" )
    ent = staticModel( scen, -129.79592895507812, -0.5221681594848633, -45.6254768371582, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-main-2-windows_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-main-2-windows_00.material", "" )
    ent = staticModel( scen, -129.79592895507812, -0.5221681594848633, -45.6254768371582, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-foundation_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-foundation_00.material", "" )
    ent = staticModel( scen, -129.79592895507812, -0.5221681594848633, -45.6254768371582, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-doors_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-doors_00.material", "" )
    ent = staticModel( scen, -129.79592895507812, -0.5221681594848633, -45.6254768371582, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-corners_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-corners_00.material", "" )
    ent = staticModel( scen, 8.479747772216797, 20.92618179321289, -44.97372817993164, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s00-a/0-winFrames_00.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    ent = staticModel( scen, 8.479747772216797, 20.92618179321289, -44.97372817993164, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s00-a/0-windows_00.bgnome", "assets/texture/static/building/wincolor-64_00.material", "" )
    ent = staticModel( scen, 8.479747772216797, 20.92618179321289, -44.97372817993164, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s00-a/0-main_00.bgnome", "assets/texture/static/building/concrete-orange-32_00.material", "" )
    ent = staticModel( scen, 8.479747772216797, 20.92618179321289, -44.97372817993164, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s00-a/0-corners_00.bgnome", "assets/texture/static/building/concrete-64_01.material", "" )
    ent = staticModel( scen, -138.0887451171875, -0.47565555572509766, -102.54994201660156, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-f/1-frame.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    ent = staticModel( scen, -138.0887451171875, -0.47565555572509766, -102.54994201660156, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-f/1-main.bgnome", "assets/texture/static/building/brickwall-darkred_00.material", "" )
    ent = staticModel( scen, -138.0887451171875, -0.47565555572509766, -102.54994201660156, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-f/1-main2.bgnome", "assets/texture/static/building/window-brickwall.material", "" )
    ent = staticModel( scen, -64.61976623535156, -0.47565555572509766, -57.117820739746094, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-f/1-valve.bgnome", "assets/texture/static/building/brickwall-32_00.material", "" )
    ent = staticModel( scen, -64.61976623535156, -0.47565555572509766, -57.117820739746094, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-f/1-main2.bgnome", "assets/texture/static/building/window-brickwall.material", "" )
    ent = staticModel( scen, -64.61976623535156, -0.47565555572509766, -57.117820739746094, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-f/1-main.bgnome", "assets/texture/static/building/brickwall-darkred_00.material", "" )
    ent = staticModel( scen, -64.61976623535156, -0.47565555572509766, -57.117820739746094, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-f/1-frame.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    ent = staticModel( scen, -27.088611602783203, -0.5532236099243164, -142.83042907714844, 0.0, 0.999954879283905, 0.0, 0.009501270018517971, 1.0, "assets/model/static/props/car_00.bgnome", "assets/texture/static/props/car_00.material", "" )
    ent = staticModel( scen, -27.088611602783203, -0.5532236099243164, -135.72738647460938, 0.0, -0.9997078776359558, 0.0, 0.02417367324233055, 1.0, "assets/model/static/props/car_00.bgnome", "assets/texture/static/props/car_00.material", "" )
    ent = staticModel( scen, -10.184270858764648, 0.3733450770378113, -147.21035766601562, 0.0, 0.2762138247489929, 0.0, 0.9610962271690369, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -10.184270858764648, -0.47565555572509766, -147.54237365722656, 0.0, -0.2787208557128906, 0.0, 0.960372269153595, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -9.861145973205566, -0.47565555572509766, -147.05177307128906, 0.0, 0.7173934578895569, 0.0, 0.696668267250061, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -10.515746116638184, -0.47565555572509766, -147.05177307128906, 0.0, -0.3359968364238739, 0.0, 0.9418631196022034, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -10.913235664367676, -0.47565555572509766, -151.291748046875, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/props/fence-grid_00.bgnome", "assets/texture/static/props/grid-32_00.material", "" )
    ent = staticModel( scen, -10.913235664367676, -0.47565555572509766, -151.291748046875, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/props/fence-bars_00.bgnome", "assets/texture/static/props/rust-32_00.material", "" )
    ent = staticModel( scen, -11.717288970947266, 2.3101110458374023, -146.87184143066406, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/props/lamp-0_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, -13.769952774047852, 2.9665021896362305, -102.88330078125, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/props/lamp-1_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, -13.769952774047852, 4.442259788513184, -98.64822387695312, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/props/lamp-1_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, -30.16516876220703, 4.442259788513184, -98.64292907714844, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/props/lamp-1_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, -30.16516876220703, 2.9665021896362305, -102.87800598144531, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/props/lamp-1_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, -34.5957145690918, 2.0583410263061523, -104.67601776123047, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/props/lamp-1_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, -39.159976959228516, 2.0583410263061523, -104.67601776123047, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/props/lamp-1_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, -43.132511138916016, 2.0583410263061523, -104.67601776123047, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/props/lamp-1_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, -9.078628540039062, 2.0583410263061523, -104.67601776123047, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/props/lamp-1_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, -5.106092929840088, 2.0583410263061523, -104.67601776123047, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/props/lamp-1_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, -0.5418338775634766, 2.0583410263061523, -104.67601776123047, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/props/lamp-1_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, -110.0176010131836, -0.39760684967041016, -131.25242614746094, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-i/4-windows_00.bgnome", "assets/texture/static/building/window-cracked-128_00.material", "" )
    ent = staticModel( scen, 35.55451202392578, 40.23910903930664, -87.41168212890625, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-i/4-roof_00.bgnome", "assets/texture/static/building/roof-tiles-64_00.material", "" )
    ent = staticModel( scen, 35.55451202392578, 40.23910903930664, -87.41168212890625, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-i/4-main_00.bgnome", "assets/texture/static/building/brickwall-32_00.material", "" )
    ent = staticModel( scen, 35.55451202392578, 40.23910903930664, -87.41168212890625, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-i/4-frame_00.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    ent = staticModel( scen, -21.915454864501953, 8.327589988708496, -147.2177734375, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/props/hanging-street-light-short_00.bgnome", "assets/texture/static/props/hanging-street-light_00.material", "" )
    ent = staticModel( scen, -21.915454864501953, 8.327589988708496, -129.4613494873047, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/props/hanging-street-light-short_00.bgnome", "assets/texture/static/props/hanging-street-light_00.material", "" )
    ent = staticModel( scen, -51.009212493896484, -0.5221681594848633, -151.8763427734375, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-corners_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-corners_00.material", "" )
    ent = staticModel( scen, -51.009212493896484, -0.5221681594848633, -151.8763427734375, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-doors_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-doors_00.material", "" )
    ent = staticModel( scen, -51.009212493896484, -0.5221681594848633, -151.8763427734375, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-foundation_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-foundation_00.material", "" )
    ent = staticModel( scen, -51.009212493896484, -0.5221681594848633, -151.8763427734375, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-main-2-windows_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-main-2-windows_00.material", "" )
    ent = staticModel( scen, -51.009212493896484, -0.5221681594848633, -151.8763427734375, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-main-3-windows_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-main-3-windows_00.material", "" )
    ent = staticModel( scen, -51.009212493896484, -0.5221681594848633, -151.8763427734375, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-angle_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-angle_00.material", "" )
    ent = staticModel( scen, -51.009212493896484, -0.5221681594848633, -151.8763427734375, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-top-edge_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-top-edge_00.material", "" )
    ent = staticModel( scen, -51.009212493896484, -0.5221681594848633, -151.8763427734375, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-top-flat_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-top-flat_00.material", "" )
    ent = staticModel( scen, 6.054241180419922, -0.5221681594848633, -228.08370971679688, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-top-flat_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-top-flat_00.material", "" )
    ent = staticModel( scen, 6.054241180419922, -0.5221681594848633, -228.08370971679688, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-top-edge_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-top-edge_00.material", "" )
    ent = staticModel( scen, 6.054241180419922, -0.5221681594848633, -228.08370971679688, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-angle_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-angle_00.material", "" )
    ent = staticModel( scen, 6.054241180419922, -0.5221681594848633, -228.08370971679688, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-main-3-windows_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-main-3-windows_00.material", "" )
    ent = staticModel( scen, 6.054241180419922, -0.5221681594848633, -228.08370971679688, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-main-2-windows_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-main-2-windows_00.material", "" )
    ent = staticModel( scen, 6.054241180419922, -0.5221681594848633, -228.08370971679688, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-foundation_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-foundation_00.material", "" )
    ent = staticModel( scen, 6.054241180419922, -0.5221681594848633, -228.08370971679688, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-doors_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-doors_00.material", "" )
    ent = staticModel( scen, 6.054241180419922, -0.5221681594848633, -228.08370971679688, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-h/0-corners_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-corners_00.material", "" )
    ent = staticModel( scen, -21.915454864501953, -1.2453088760375977, -84.24113464355469, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/building/special/s-01/embassy-windows_00.bgnome", "assets/texture/static/building/window-brown-128_00.material", "" )
    ent = staticModel( scen, -21.915454864501953, -1.2453088760375977, -84.24113464355469, 7.549790126404332e-08, 1.0, -1.2300169461795536e-14, -1.6292068494294654e-07, 1.0, "assets/model/static/building/special/s-01/embassy-roof_00.bgnome", "assets/texture/static/building/concrete-64_01.material", "" )
    ent = staticModel( scen, -21.915454864501953, -1.2453088760375977, -84.24113464355469, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/building/special/s-01/embassy-pillars_00.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    ent = staticModel( scen, -21.915454864501953, -1.2453088760375977, -84.24113464355469, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/building/special/s-01/embassy-main_00.bgnome", "assets/texture/static/building/brickwall_00.material", "" )
    ent = staticModel( scen, -21.915454864501953, -1.2453088760375977, -84.24113464355469, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/building/special/s-01/embassy-golditems_00.bgnome", "assets/texture/static/building/metal-gold-32_00.material", "" )
    ent = staticModel( scen, -21.915454864501953, -1.2453088760375977, -84.24113464355469, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/building/special/s-01/embassy-flags_00.bgnome", "assets/texture/static/building/wincolor-64_00.material", "" )
    ent = staticModel( scen, 19.8924560546875, -0.19059467315673828, -144.46153259277344, 0.0, 1.0, 0.0, -7.549790126404332e-08, 1.0, "assets/model/static/building/apartment/s01-i/2-windows_00.bgnome", "assets/texture/static/building/window-with-bricks_00.material", "" )
    ent = staticModel( scen, 19.8924560546875, -0.19059467315673828, -144.46153259277344, 0.0, 1.0, 0.0, -7.549790126404332e-08, 1.0, "assets/model/static/building/apartment/s01-i/2-main_00.bgnome", "assets/texture/static/building/concrete-orange-32_00.material", "" )
    ent = staticModel( scen, 19.8924560546875, -0.19059467315673828, -144.46153259277344, 0.0, 1.0, 0.0, -7.549790126404332e-08, 1.0, "assets/model/static/building/apartment/s01-i/2-frame_00.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    ent = staticModel( scen, -8.211770057678223, 17.111446380615234, -140.67234802246094, 0.0, 0.49952176213264465, 0.0, 0.8663013577461243, 1.0, "assets/model/static/props/billboard-picture_01.bgnome", "assets/texture/static/props/billboard_01.material", "" )
    ent = staticModel( scen, -8.211770057678223, 17.111446380615234, -140.67234802246094, 0.0, 0.49952176213264465, 0.0, 0.8663013577461243, 1.0, "assets/model/static/props/billboard-frame_00.bgnome", "assets/texture/static/props/rust-32_00.material", "" )
    ent = staticModel( scen, -0.1317920684814453, -0.532322883605957, -129.74147033691406, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-d/3-windows.bgnome", "assets/texture/static/building/window-plain-brown_00.material", "" )
    ent = staticModel( scen, -0.1317920684814453, -0.532322883605957, -129.74147033691406, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-d/3-roof.bgnome", "assets/texture/static/building/concrete-64_01.material", "" )
    ent = staticModel( scen, -0.1317920684814453, -0.532322883605957, -129.74147033691406, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-d/3-pillars.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    ent = staticModel( scen, -0.1317920684814453, -0.532322883605957, -129.74147033691406, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-d/3-main.bgnome", "assets/texture/static/building/brickwall-32_00.material", "" )
    ent = staticModel( scen, -0.1317920684814453, -0.532322883605957, -129.74147033691406, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-d/3-door.bgnome", "assets/texture/static/building/garage-port-64_01.material", "" )
    ent = staticModel( scen, 0.24690628051757812, -0.5741472244262695, -141.3160858154297, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-d/2-windows.bgnome", "assets/texture/static/building/window-plain-grey_00.material", "" )
    ent = staticModel( scen, 0.24690628051757812, -0.5741472244262695, -141.3160858154297, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-d/2-roofedge.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    ent = staticModel( scen, 0.24690628051757812, -0.5741472244262695, -141.3160858154297, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-d/2-main.bgnome", "assets/texture/static/building/concrete-sand-32_00.material", "" )
    ent = staticModel( scen, 0.24690628051757812, -0.5741472244262695, -141.3160858154297, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-d/2-doorhandle.bgnome", "assets/texture/static/building/metal-grey_00.material", "" )
    ent = staticModel( scen, 0.24690628051757812, -0.5741472244262695, -141.3160858154297, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-d/2-dooredge.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    ent = staticModel( scen, 0.24690628051757812, -0.5741472244262695, -141.3160858154297, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-d/2-door.bgnome", "assets/texture/static/building/garage-port-64_01.material", "" )
    ent = staticModel( scen, 74.0203628540039, -0.5619993209838867, -132.96031188964844, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/street/scenario1-street_00.bgnome", "assets/texture/static/street/street_00.material", "" )
    ent = staticModel( scen, 74.01870727539062, -0.5741472244262695, -132.9451446533203, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/street/scenario1-square_00.bgnome", "assets/texture/static/street/asphalt_00.material", "" )
    ent = staticModel( scen, 74.01870727539062, -0.5741472244262695, -132.9451446533203, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/street/scenario1-sidewalk_00.bgnome", "assets/texture/static/street/sidewalk_00.material", "" )
    ent = staticModel( scen, 74.01870727539062, -0.5741472244262695, -132.9451446533203, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/street/scenario1-grounds_00.bgnome", "assets/texture/static/street/asphalt_00.material", "" )
    ent = staticModel( scen, 35.997650146484375, 15.920489311218262, -121.02941131591797, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s00-a/1-roof_00.bgnome", "assets/texture/static/building/concrete-128_02.material", "" )
    ent = staticModel( scen, 35.997650146484375, 15.920489311218262, -121.02941131591797, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s00-a/1-main_00.bgnome", "assets/texture/static/building/window-128_03.material", "" )
    ent = staticModel( scen, 35.997650146484375, 15.920489311218262, -121.02941131591797, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s00-a/1-corners_00.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )

    ent = decal( scen, 11.44291877746582, -0.5058501958847046, -150.23513793945312, 0.0, -0.5533955693244934, 0.0, 0.8329185843467712, 1.0, 0.07287715375423431, 1.0, "assets/texture/decal/radioactive-goo-decal.material", "" )
    ent = decal( scen, -2.4609405994415283, 1.8023370504379272, -163.87542724609375, 0.0, -0.5533955693244934, 0.0, 0.8329185843467712, 1.0, 1.0, 1.0, "assets/texture/decal/radioactive-goo-decal.material", "" )
    ent = decal( scen, -9.769536972045898, -0.5113381743431091, -149.06044006347656, 0.0, -0.25022706389427185, 0.0, 0.968187153339386, 1.0, 0.010000050067901611, 1.0, "assets/texture/decal/radioactive-goo-decal.material", "" )
    ent = decal( scen, -8.399545669555664, -0.07053568959236145, -148.10012817382812, 0.0, 0.3176756203174591, 0.0, 0.948199450969696, 1.0, 1.0, 1.0, "assets/texture/decal/radioactive-goo-decal.material", "" )
    ent = decal( scen, -11.71988582611084, 1.0919675827026367, -146.75645446777344, 2.8378977390275395e-07, -0.7071068286895752, 0.7071068286895752, -2.837898023244634e-07, 1.0927678346633911, 1.0927678346633911, 2.1855356693267822, "assets/texture/decal/names.material", "" )
end
return scen
