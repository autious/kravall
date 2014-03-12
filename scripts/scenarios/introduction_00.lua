local entities = require "entities"
local scenario = require "scenario"
local scen = scenario.new()

scen.name = "Default Scenario Name"
scen.description = "Default Description"

scen.cameras = scen.cameras or {}

local script = dofile "scripts/scenarios/introduction_00_script.lua"( scen )

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
    
    scen.asm:specific_content( core.contentmanager.load( core.loaders.NavigationMeshLoader, "scripts/scenarios/introduction_00.nav", function( value ) end, false ) )

    
    local function genF( e, f )
        if type( script[f] ) == "function"  then
            return function( delta ) script[ f ]( e, delta ) end
        else
            error( "Function " .. f .. " does not exist in script file" )
        end
    end
    
    directional( scen, -0.45121949911117554, -0.464176207780838, 0.7621951103210449, 1.0, 1.0, 1.0, 1.0 )
    ambient( scen, 1.0, 1.0, 1.0, 0.5 )

    ent = area( scen, {20.260589599609375, 0, 1.0933685302734375}, {-4.608107566833496, 4.608107566833496, -4.608107566833496, -4.608107566833496, 4.608107566833496, -4.608107566833496, 4.608107566833496, 4.608107566833496}, "" )
    scen:registerTickCallback( genF( ent, "checkAg2Enter" ) )

    ent = area( scen, {48.46102523803711, 0, 1.0933685302734375}, {-4.608107566833496, 4.608107566833496, -4.608107566833496, -4.608107566833496, 4.608107566833496, -4.608107566833496, 4.608107566833496, 4.608107566833496}, "" )
    scen:registerTickCallback( genF( ent, "checkAg1Enter" ) )

    ent = area( scen, {12.042716979980469, 0, 58.9816780090332}, {-2.658367156982422, 4.104645729064941, -2.658367156982422, -4.104645729064941, 2.658367156982422, -4.104645729064941, 2.658367156982422, 4.104645729064941}, "" )
    scen:registerInitCallback( genF( ent, "registerAg2Spawn" ) )

    ent = area( scen, {11.29880142211914, 0, -51.76402282714844}, {-2.93356990814209, 4.662440299987793, -2.93356990814209, -4.662440299987793, 2.93356990814209, -4.662440299987793, 2.93356990814209, 4.662440299987793}, "" )
    scen:registerInitCallback( genF( ent, "registerAg1Spawn" ) )

    ent = area( scen, {1.6570675373077393, 0, 9.781811714172363}, {-4.098126411437988, 4.098126411437988, -4.098126411437988, -4.098126411437988, 4.098126411437988, -4.098126411437988, 4.098126411437988, 4.098126411437988}, "" )
    scen:registerInitCallback( genF( ent, "registerSpawn" ) )

    ent = area( scen, {-60.25840377807617, 0, 10.129222869873047}, {-3.7555899620056152, 3.7555899620056152, -3.7555899620056152, -3.7555899620056152, 3.7555899620056152, -3.7555899620056152, 3.7555899620056152, 3.7555899620056152}, "waypoint2" )
    scen:registerInitCallback( genF( ent, "registerWaypoint" ) )
    scen:registerTickCallback( genF( ent, "winOnRioterInside" ) )

    ent = area( scen, {48.56011962890625, 0, -8.351276397705078}, {-2.908853769302368, 4.316685199737549, -2.908853769302368, -4.316685199737549, 2.908853769302368, -4.316685199737549, 2.908853769302368, 4.316685199737549}, "waypoint1" )
    scen:registerInitCallback( genF( ent, "registerWaypoint" ) )

    ent = area( scen, {1.6570675373077393, 0, -8.583291053771973}, {-4.097105503082275, 4.097105503082275, -4.097105503082275, -4.097105503082275, 4.097105503082275, -4.097105503082275, 4.097105503082275, 4.097105503082275}, "" )
    scen:registerInitCallback( genF( ent, "registerSpawn" ) )

    ent = area( scen, {48.48855972290039, 0, -9.944025039672852}, {-3.0074408054351807, 2.6553821563720703, -3.0074408054351807, -2.6553821563720703, 3.0074408054351807, -2.6553821563720703, 3.0074408054351807, 2.6553821563720703}, "" )
    scen:registerInitCallback( genF( ent, "spawnDesertersOnMain" ) )


    ent = staticModel( scen, -64.47183227539062, 0.0, 10.313325881958008, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, { 0.27721741795539856, 0.8422974348068237, 0.09035760164260864 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, -57.447811126708984, 0.0, 5.842321395874023, 0.0, 1.0, 0.0, 7.549789415861596e-08, { 0.5178174376487732, 0.8422974348068237, 0.09035760164260864 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, -57.447811126708984, 0.0, 14.64304256439209, 0.0, 1.0, 0.0, 7.549789415861596e-08, { 0.5178174376487732, 0.8422974348068237, 0.09035760164260864 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, -49.498409271240234, 2.049058198928833, 14.151671409606934, 0.7071068286895752, 0.0, 0.0, 0.7071068286895752, { 1.0, 1.0, 1.0 }, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -48.9012451171875, 1.7952842712402344, 14.631048202514648, 0.0, 0.0, -0.2308632731437683, 0.9729862809181213, { 1.0, 1.0, 1.0 }, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -49.1623420715332, 0.8615707159042358, 14.631048202514648, 0.0, 0.0, -0.06592507660388947, 0.9978246688842773, { 0.9999999403953552, 0.9999999403953552, 1.0 }, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -49.425228118896484, 0.0, 14.631048202514648, 0.0, 0.0, 0.0, 1.0, { 1.0, 1.0, 1.0 }, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -48.706851959228516, 0.0, 14.631048202514648, 0.0, 0.0, 0.0, 1.0, { 1.0, 1.0, 1.0 }, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -43.67610549926758, 0.0, 14.631048202514648, 0.0, 1.0, 0.0, 7.549790126404332e-08, { 1.0, 1.0, 1.0 }, "assets/model/static/props/fence-grid_00.bgnome", "assets/texture/static/props/grid-32_00.material", "" )
    ent = staticModel( scen, -43.67610549926758, 0.0, 14.631048202514648, 0.0, 1.0, 0.0, 7.549790126404332e-08, { 1.0, 1.0, 1.0 }, "assets/model/static/props/fence-bars_00.bgnome", "assets/texture/static/props/rust-32_00.material", "" )
    ent = staticModel( scen, -44.53049087524414, 0.0, 14.634249687194824, 0.0, 0.0, 0.0, 1.0, { 0.09431116282939911, 0.27630072832107544, 0.011764091439545155 }, "assets/model/static/street/whitebox/street_00.bgnome", "assets/texture/grey.material", "" )
    ent = staticModel( scen, 2.224308967590332, -2.384185791015625e-07, 39.42127227783203, 0.0, 0.0, 0.0, 1.0, { 0.23000000417232513, 1.0477070808410645, 0.6220260262489319 }, "assets/model/static/street/whitebox/street_00.bgnome", "assets/texture/grey.material", "" )
    ent = staticModel( scen, 1.455270528793335, 5.281300673232181e-06, -35.64756393432617, 0.0, 0.0, 0.0, 1.0, { 0.23000000417232513, 1.0477070808410645, 0.5420234799385071 }, "assets/model/static/street/whitebox/street_00.bgnome", "assets/texture/grey.material", "" )
    ent = staticModel( scen, 10.340189933776855, 0.02841770648956299, -57.4030647277832, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, { 0.04650302603840828, 0.8150208592414856, 0.8775044679641724 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, 15.431158065795898, 0.02841770648956299, -52.515724182128906, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, { 0.3972293734550476, 0.8150208592414856, 0.1406547725200653 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, 10.331435203552246, 0.01729041337966919, 63.83039855957031, 0.0, 1.0, 0.0, 7.549790126404332e-08, { 0.2864677608013153, 0.8289695978164673, 0.1606495976448059 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, 15.337873458862305, 0.01729041337966919, 59.14631271362305, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, { 0.3695581555366516, 0.900545060634613, 0.1606495976448059 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, 48.169376373291016, -0.2019776701927185, -13.239145278930664, 0.0, 1.0, 0.0, 7.549790126404332e-08, { 0.2203620821237564, 0.7573654651641846, 0.0806237980723381 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, 48.1129035949707, 3.5062155723571777, -8.618551254272461, 0.0, 1.0, 0.0, 7.549789415861596e-08, { 0.22036212682724, 0.26648014783859253, 1.0 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, 10.98147201538086, 3.7848594188690186, 59.14631271362305, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, { 0.3610134720802307, 0.2093174308538437, 1.0 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, 10.939444541931152, 3.8314387798309326, -52.49310302734375, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, { 0.38570454716682434, 0.17452426254749298, 1.0 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, -49.827552795410156, 0.0, 0.6261359453201294, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, { 1.0, 1.0, 1.0 }, "assets/model/static/props/fence-bars_00.bgnome", "assets/texture/static/props/rust-32_00.material", "" )
    ent = staticModel( scen, -49.827552795410156, 0.0, 0.6261359453201294, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, { 1.0, 1.0, 1.0 }, "assets/model/static/props/fence-grid_00.bgnome", "assets/texture/static/props/grid-32_00.material", "" )
    ent = staticModel( scen, 1.6596627235412598, 1.9073486328125e-06, 63.012516021728516, 0.0, -1.5099580252808664e-07, 0.0, 1.0, { 1.0, 1.0, 1.0 }, "assets/model/static/props/fence-grid_00.bgnome", "assets/texture/static/props/grid-32_00.material", "" )
    ent = staticModel( scen, 1.6596629619598389, 1.9073486328125e-06, 63.012516021728516, 0.0, -1.5099580252808664e-07, 0.0, 1.0, { 1.0, 1.0, 1.0 }, "assets/model/static/props/fence-bars_00.bgnome", "assets/texture/static/props/rust-32_00.material", "" )
    ent = staticModel( scen, 1.4974002838134766, 0.0, -56.819149017333984, 0.0, -1.5099580252808664e-07, 0.0, 1.0, { 1.0, 1.0, 1.0 }, "assets/model/static/props/fence-bars_00.bgnome", "assets/texture/static/props/rust-32_00.material", "" )
    ent = staticModel( scen, 1.4974000453948975, 0.0, -56.819149017333984, 0.0, -1.5099580252808664e-07, 0.0, 1.0, { 1.0, 1.0, 1.0 }, "assets/model/static/props/fence-grid_00.bgnome", "assets/texture/static/props/grid-32_00.material", "" )
    ent = staticModel( scen, 64.7956771850586, 0.0, 0.6261359453201294, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, { 1.0, 1.0, 1.0 }, "assets/model/static/props/fence-grid_00.bgnome", "assets/texture/static/props/grid-32_00.material", "" )
    ent = staticModel( scen, 64.7956771850586, 0.0, 0.6261359453201294, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, { 1.0, 1.0, 1.0 }, "assets/model/static/props/fence-bars_00.bgnome", "assets/texture/static/props/rust-32_00.material", "" )
    ent = staticModel( scen, 58.115333557128906, 2.3222344225359848e-06, -8.634541511535645, 0.0, 1.0, 0.0, 7.549790126404332e-08, { 0.47426360845565796, 1.0, 1.0 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, 49.68517303466797, 2.8874319468741305e-06, 10.2213134765625, 0.0, 1.0, 0.0, 7.549790126404332e-08, { 1.0377579927444458, 1.0, 1.0 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, -48.210906982421875, 2.365198952247738e-07, -8.594670295715332, 0.0, 1.0, 0.0, 7.549790126404332e-08, { 1.1307967901229858, 1.0, 1.0 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, -57.36458969116211, 4.039292335510254, 10.405611038208008, 0.0, 1.0, 0.0, 7.549789415861596e-08, { 0.5178174376487732, 0.1549662947654724, 1.0 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, 10.98147201538086, 6.90030617533921e-07, 34.252384185791016, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, { 1.3469974994659424, 1.0, 1.0 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, -7.950620651245117, 6.813509685343888e-07, 39.104286193847656, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, { 1.7481008768081665, 1.0, 1.0 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, -7.820948600769043, -2.8642779170695576e-07, -35.112266540527344, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, { 1.5227304697036743, 1.0, 1.0 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, -17.39337921142578, 4.3398244997661095e-09, -8.484456062316895, 0.0, 1.0, 0.0, 7.549790126404332e-08, { 1.0, 1.0, 1.0 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, -21.171916961669922, 5.468166932587337e-07, 10.275938987731934, 0.0, 1.0, 0.0, 7.549790126404332e-08, { 1.2421401739120483, 1.0, 1.0 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, 20.727741241455078, 5.489865770869073e-07, 10.350980758666992, 0.0, 1.0, 0.0, 7.549790126404332e-08, { 1.0, 1.0, 1.0 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, 30.40077018737793, 0.0, -8.634538650512695, 0.0, 1.0, 0.0, 7.549790126404332e-08, { 1.0, 1.0, 1.0 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, 10.976139068603516, 6.414849762492736e-10, -25.487030029296875, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, { 1.4772357940673828, 1.0, 1.0 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, 0.0, 0.0, 0.26212096214294434, 0.0, 0.0, 0.0, 1.0, { 1.1197912693023682, 1.0406486988067627, 0.34208089113235474 }, "assets/model/static/street/whitebox/street_00.bgnome", "assets/texture/grey.material", "" )

    ent = decal( scen, -43.918487548828125, -3.1758460998535156, 0.8475474119186401, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 8.05722427368164, 8.05722427368164, 8.05722427368164, "assets/texture/decal/left-arrow.material", "" )
    ent = decal( scen, -17.714937210083008, -3.1758460998535156, 0.8475474119186401, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 8.05722427368164, 8.05722427368164, 8.05722427368164, "assets/texture/decal/straight-arrow.material", "" )
    ent = decal( scen, 8.488615036010742, -3.1758460998535156, 0.8475474119186401, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 8.05722427368164, 8.05722427368164, 8.05722427368164, "assets/texture/decal/straight-arrow.material", "" )
    ent = decal( scen, 34.692161560058594, -3.1758460998535156, 0.8475474119186401, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 8.05722427368164, 8.05722427368164, 8.05722427368164, "assets/texture/decal/straight-arrow.material", "" )
    ent = decal( scen, -49.161842346191406, -0.4405651390552521, 14.199278831481934, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, "assets/texture/decal/radioactive-goo-decal.material", "" )
    ent = decal( scen, 1.6675349473953247, -3.9210731983184814, -51.81586456298828, -1.5099580252808664e-07, 0.0, 0.0, 1.0, 8.363571166992188, 8.363571166992188, 8.363571166992188, "assets/texture/decal/agitator-warning.material", "" )
    scen:registerInitCallback( genF( ent, "hideOnMain" ) )
    ent = decal( scen, 1.7658047676086426, 0.18924641609191895, 58.50574493408203, -1.5099580252808664e-07, 0.0, 0.0, 1.0, 8.363571166992188, 8.363571166992188, 8.363571166992188, "assets/texture/decal/agitator-warning.material", "" )
    scen:registerInitCallback( genF( ent, "hideOnMain" ) )
    ent = decal( scen, 48.18290328979492, -1.0383517742156982, 0.8035286664962769, -1.5099580252808664e-07, 0.0, 0.0, 1.0, 8.076769828796387, 8.076769828796387, 8.076769828796387, "assets/texture/decal/friendly-spawn.material", "" )
    scen:registerInitCallback( genF( ent, "hideOnMain" ) )
    ent = decal( scen, -44.43244171142578, -1.0383517742156982, 9.956398010253906, -1.5099578831723193e-07, 0.0, 0.0, 1.0, 8.838715553283691, 8.838715553283691, 8.838715553283691, "assets/texture/decal/goal.material", "" )
    scen:registerInitCallback( genF( ent, "hideOnMain" ) )
    ent = decal( scen, 1.6570675373077393, -1.0383479595184326, 9.78181266784668, -1.5099580252808664e-07, 0.0, 0.0, 1.0, 8.220477104187012, 8.220477104187012, 8.220477104187012, "assets/texture/decal/spawn-decal.material", "" )
    scen:registerInitCallback( genF( ent, "hideOnMain" ) )
    ent = decal( scen, 1.6570675373077393, -1.0383479595184326, -8.583290100097656, -1.5099580252808664e-07, 0.0, 0.0, 1.0, 8.220477104187012, 8.220477104187012, 8.220477104187012, "assets/texture/decal/spawn-decal.material", "" )
    scen:registerInitCallback( genF( ent, "hideOnMain" ) )
end
return scen
