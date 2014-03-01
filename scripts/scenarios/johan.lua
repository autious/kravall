local entities = require "entities"
local scenario = require "scenario"
local scen = scenario.new()

scen.name = "Default Scenario Name"
scen.description = "Default Description"

scen.cameras = scen.cameras or {}

local script = dofile "scripts/scenarios/johan_script.lua"( scen )

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
    
    directional( scen, 0.6196318864822388, -0.4971596598625183, 0.6073619723320007, 1.0, 1.0, 1.0, 0.05000000074505806 )
    ambient( scen, 1.0, 1.0, 1.0, 0.009999999776482582 )

    ent = pointLight( scen, 19.190876007080078, 1.6946126222610474, -8.006370544433594, 1.0, 0.791355311870575, 0.27348625659942627, 3.0, 5.0, "" )
    ent = pointLight( scen, 14.65572452545166, 1.6946126222610474, -8.006370544433594, 1.0, 0.791355311870575, 0.27348625659942627, 3.0, 5.0, "" )
    ent = pointLight( scen, 13.523261070251465, 1.6946126222610474, -8.006370544433594, 1.0, 0.791355311870575, 0.27348625659942627, 3.0, 5.0, "" )
    ent = pointLight( scen, 9.107473373413086, 1.6946126222610474, -8.006370544433594, 1.0, 0.791355311870575, 0.27348625659942627, 3.0, 5.0, "" )
    ent = pointLight( scen, -14.546480178833008, 1.6946126222610474, -8.006370544433594, 1.0, 0.791355311870575, 0.27348625659942627, 3.0, 5.0, "" )
    ent = pointLight( scen, -19.040250778198242, 1.6946126222610474, -8.006370544433594, 1.0, 0.791355311870575, 0.27348625659942627, 3.0, 5.0, "" )
    ent = pointLight( scen, -20.122385025024414, 1.6946126222610474, -8.006370544433594, 1.0, 0.791355311870575, 0.27348625659942627, 3.0, 5.0, "" )
    ent = spotLight( scen, 10.935330390930176, 51.80292510986328, -31.54894256591797, 0.6830129027366638, 0.25881892442703247, 0.6830125451087952, 1.0, 1.0, 1.0, 0.6195918917655945, 0.30979594588279724, 10.0, 20.0, "" )
    ent = spotLight( scen, 23.9088134765625, 51.80292510986328, -31.548946380615234, -0.6830129623413086, 0.25881892442703247, 0.6830124855041504, 1.0, 1.0, 1.0, 0.6195918917655945, 0.30979594588279724, 10.0, 20.0, "" )
    ent = spotLight( scen, 10.935328483581543, 51.80292510986328, -18.10958480834961, 0.6830127239227295, 0.25881892442703247, -0.6830127239227295, 1.0, 1.0, 1.0, 0.6195918917655945, 0.30979594588279724, 10.0, 20.0, "" )
    ent = spotLight( scen, 23.9088134765625, 51.80292510986328, -18.109588623046875, -0.683012843132019, 0.25881892442703247, -0.6830126047134399, 1.0, 1.0, 1.0, 0.6195918917655945, 0.30979594588279724, 10.0, 20.0, "" )
    ent = pointLight( scen, 16.75263214111328, 53.453125, -29.650991439819336, 0.0, 1.0, 0.0, 3.0, 0.5, "" )
    ent = pointLight( scen, 20.74766731262207, 53.3634033203125, -26.905601501464844, 0.0, 1.0, 0.0, 3.0, 0.5, "" )
    ent = pointLight( scen, 22.612295150756836, 54.54296875, -24.907339096069336, 1.0, 1.0, 1.0, 3.0, 0.5, "" )
    ent = pointLight( scen, 22.057424545288086, 52.58857727050781, -24.856706619262695, 1.0, 1.0, 1.0, 3.0, 0.5, "" )
    ent = pointLight( scen, 20.74766731262207, 53.3634033203125, -22.85397720336914, 1.0, 0.0, 0.0, 3.0, 0.5, "" )
    ent = pointLight( scen, 16.752635955810547, 53.453125, -20.266517639160156, 1.0, 0.0, 0.0, 3.0, 0.5, "" )
    ent = pointLight( scen, -24.573177337646484, 1.6946126222610474, -8.006370544433594, 1.0, 0.791355311870575, 0.27348625659942627, 3.0, 5.0, "" )
    ent = pointLight( scen, -2.776893138885498, 1.0432937145233154, 10.187715530395508, 0.0, 0.3769230246543884, 0.0, 2.4000000953674316, 2.5, "" )
    ent = pointLight( scen, -2.401970863342285, 0.1954396665096283, 8.34895133972168, 0.0, 0.3769230246543884, 0.0, 2.4000000953674316, 2.5, "" )
    ent = pointLight( scen, -3.403597354888916, 0.14211565256118774, 9.30454158782959, 0.0, 0.3769230246543884, 0.0, 2.4000000953674316, 2.5, "" )
    ent = pointLight( scen, -0.14110231399536133, 0.2989373505115509, 8.651192665100098, 0.0, 0.3769230246543884, 0.0, 2.4000000953674316, 2.5, "" )
    ent = pointLight( scen, -1.7547281980514526, 0.9900795221328735, 9.533310890197754, 0.0, 0.3769230246543884, 0.0, 2.4000000953674316, 2.5, "" )
    ent = spotLight( scen, 27.072330474853516, 6.091360092163086, -0.0, 0.0, -1.0, -0.0, 0.9748446941375732, 1.0, 0.6600653529167175, 1.1780972480773926, 0.5890486240386963, 16.719999313354492, 30.0, "" )
    ent = spotLight( scen, 20.477859497070312, 6.091360092163086, -0.0, 0.0, -1.0, -0.0, 0.9748446941375732, 1.0, 0.6600653529167175, 1.1780972480773926, 0.5890486240386963, 16.719999313354492, 30.0, "" )
    ent = spotLight( scen, 10.131933212280273, 6.091360092163086, -0.0, 0.0, -1.0, -0.0, 0.9748446941375732, 1.0, 0.6600653529167175, 1.1780972480773926, 0.5890486240386963, 16.719999313354492, 30.0, "" )
    ent = spotLight( scen, -10.345926284790039, 6.091360092163086, -0.0, 0.0, -1.0, -0.0, 0.9748446941375732, 1.0, 0.6600653529167175, 1.1780972480773926, 0.5890486240386963, 16.719999313354492, 30.0, "" )
    ent = spotLight( scen, 0.0, 6.091360092163086, -0.0, 0.0, -1.0, -0.0, 0.9748446941375732, 1.0, 0.6600653529167175, 1.1780972480773926, 0.5890486240386963, 16.719999313354492, 30.0, "" )

    ent = staticModel( scen, 2.8804421424865723, 0.5662765502929688, 7.30252742767334, 0.0, 0.443843811750412, 0.0, 0.8961042165756226, 1.0, "assets/model/static/props/container-filled_00.bgnome", "assets/texture/static/props/container_00.material", "" )
    ent = staticModel( scen, 8.195691108703613, 0.5662765502929688, 7.964821815490723, 0.0, -0.6779846549034119, 0.0, 0.7350760698318481, 1.0, "assets/model/static/props/container-empty_00.bgnome", "assets/texture/static/props/container_00.material", "" )
    ent = staticModel( scen, -8.020992279052734, 0.0012874603271484375, -9.168323516845703, 0.0, -0.3123601973056793, 0.0, 0.9499637484550476, 1.0, "assets/model/static/props/crate_01.bgnome", "assets/texture/static/props/crate_01.material", "" )
    ent = staticModel( scen, -8.561803817749023, 0.0012874603271484375, -8.472677230834961, 0.0, 0.1623247116804123, 0.0, 0.9867374897003174, 1.0, "assets/model/static/props/crate_01.bgnome", "assets/texture/static/props/crate_01.material", "" )
    ent = staticModel( scen, 16.93962287902832, 51.90251922607422, -24.902660369873047, 0.0, 1.5099580252808664e-07, 0.0, 1.0, 1.0, "assets/model/static/props/hovercraft_00.bgnome", "assets/texture/static/props/hovercraft_00.material", "" )
    ent = staticModel( scen, 16.839420318603516, 0.1324501484632492, -8.758119583129883, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/props/market_00.bgnome", "assets/texture/static/props/market_00.material", "" )
    ent = staticModel( scen, 11.221826553344727, 0.1324501484632492, -8.758119583129883, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/props/market_00.bgnome", "assets/texture/static/props/market_00.material", "" )
    ent = staticModel( scen, -22.42475700378418, 0.1324501484632492, -8.758119583129883, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/props/market_00.bgnome", "assets/texture/static/props/market_00.material", "" )
    ent = staticModel( scen, -16.80716323852539, 0.1324501484632492, -8.758119583129883, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/props/market_00.bgnome", "assets/texture/static/props/market_00.material", "" )
    ent = staticModel( scen, 17.331846237182617, 49.810707092285156, -24.853858947753906, 0.0, 1.0, 0.0, -7.549790126404332e-08, 1.0, "assets/model/static/props/helipad_00.bgnome", "assets/texture/static/props/helipad_00.material", "" )
    ent = staticModel( scen, 332.7080078125, 0.0, -27.42495346069336, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/street/scenario1-square_00.bgnome", "assets/texture/static/street/asphalt_00.material", "" )
    ent = staticModel( scen, 203.1403350830078, 0.0, -27.42495346069336, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/street/scenario1-square_00.bgnome", "assets/texture/static/street/asphalt_00.material", "" )
    ent = staticModel( scen, 27.072330474853516, 6.613555431365967, -0.0, 0.0, -0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/props/hanging-street-light-short_00.bgnome", "assets/texture/static/props/hanging-street-light_00.material", "" )
    ent = staticModel( scen, -21.756851196289062, 6.613555431365967, -0.0, 0.0, -0.7832713723182678, 0.0, 0.6216801404953003, 1.0, "assets/model/static/props/hanging-street-light-short_00.bgnome", "assets/texture/static/props/hanging-street-light_00.material", "" )
    ent = staticModel( scen, 20.477859497070312, 6.613555431365967, -0.0, 0.0, -0.7575740218162537, 0.0, 0.652749240398407, 1.0, "assets/model/static/props/hanging-street-light-short_00.bgnome", "assets/texture/static/props/hanging-street-light_00.material", "" )
    ent = staticModel( scen, 10.131933212280273, 6.613555431365967, -0.0, 0.0, -0.630814790725708, 0.0, 0.7759334444999695, 1.0, "assets/model/static/props/hanging-street-light-short_00.bgnome", "assets/texture/static/props/hanging-street-light_00.material", "" )
    ent = staticModel( scen, -10.345926284790039, 6.613555431365967, -0.0, 0.0, -0.6382718086242676, 0.0, 0.7698111534118652, 1.0, "assets/model/static/props/hanging-street-light-short_00.bgnome", "assets/texture/static/props/hanging-street-light_00.material", "" )
    ent = staticModel( scen, 0.0, 6.613555431365967, -0.0, 0.0, -0.7719178795814514, 0.0, 0.635722279548645, 1.0, "assets/model/static/props/hanging-street-light-short_00.bgnome", "assets/texture/static/props/hanging-street-light_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, 29.004405975341797, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-top-flat_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-top-flat_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, 29.004405975341797, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-top-edge_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-top-edge_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, 29.004405975341797, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-angle_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-angle_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, 29.004405975341797, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-main-3-windows_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-main-3-windows_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, 29.004405975341797, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-main-2-windows_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-main-2-windows_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, 29.004405975341797, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-foundation_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-foundation_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, 29.004405975341797, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-doors_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-doors_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, 29.004405975341797, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-corners_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-corners_00.material", "" )
    ent = staticModel( scen, -2.8349359035491943, 0.0, 9.88503646850586, 0.0, 0.7561582326889038, 0.0, 0.6543888449668884, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -1.4411301612854004, 0.0, 8.667076110839844, 0.0, 0.105945885181427, 0.0, 0.9943720102310181, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -2.4132697582244873, 0.0, 9.341194152832031, 0.0, 0.19625653326511383, 0.0, 0.9805526733398438, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, -1.74428391456604, 0.0, 9.511468887329102, 0.0, -0.034767575562000275, 0.0, 0.9993954300880432, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, -29.29818344116211, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-corners_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-corners_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, -29.29818344116211, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-doors_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-doors_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, -29.29818344116211, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-foundation_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-foundation_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, -29.29818344116211, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-main-2-windows_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-main-2-windows_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, -29.29818344116211, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-main-3-windows_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-main-3-windows_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, -29.29818344116211, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-angle_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-angle_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, -29.29818344116211, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-top-edge_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-top-edge_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, -29.29818344116211, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-top-flat_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-top-flat_00.material", "" )
    ent = staticModel( scen, 268.3460693359375, 0.0, -27.42495346069336, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/street/scenario1-square_00.bgnome", "assets/texture/static/street/asphalt_00.material", "" )
    ent = staticModel( scen, 1.1547068357467651, 0.0, -8.981343269348145, 0.0, -0.3295283615589142, 0.0, 0.944145679473877, 1.0, "assets/model/static/props/tyre_00.bgnome", "assets/texture/static/props/tyre_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, -9.005179405212402, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/props/tyre-pile_00.bgnome", "assets/texture/static/props/tyre_00.material", "" )
    ent = staticModel( scen, 2.876291036605835, 0.0, -9.122841835021973, 0.0, -0.0774390771985054, 0.0, 0.9969971179962158, 1.0, "assets/model/static/props/garbage-bin_02.bgnome", "assets/texture/static/props/garbage-bin_02.material", "" )
    ent = staticModel( scen, -14.655938148498535, 0.0, 8.94991683959961, 0.0, 1.0, 0.0, 7.549790126404332e-08, 1.0, "assets/model/static/props/garbage-bin_01.bgnome", "assets/texture/static/props/garbage-bin_01.material", "" )
    ent = staticModel( scen, -19.502944946289062, 0.0, 8.787432670593262, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/props/garbage-bin_00.bgnome", "assets/texture/static/props/garbage-bin_00.material", "" )
    ent = staticModel( scen, -6.63585090637207, 0.0, -8.842134475708008, 0.0, -0.13332974910736084, 0.0, 0.9910718202590942, 1.0, "assets/model/static/props/crate_00.bgnome", "assets/texture/static/props/crate_00.material", "" )
    ent = staticModel( scen, -0.5612831115722656, 0.0, 9.511468887329102, 0.0, 0.4813521206378937, 0.0, 0.8765274286270142, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )

    ent = decal( scen, -9.209197998046875, 0.24288368225097656, 0.29724985361099243, 0.0, 0.3152351379394531, 0.0, 0.9490135908126831, 1.0, 1.0, 1.0, "assets/texture/decal/blood-decal.material", "" )
    ent = decal( scen, -10.466140747070312, 0.24288368225097656, 0.9277031421661377, 0.0, 0.9243794679641724, 0.0, 0.3814745247364044, 1.0, 1.0, 1.0, "assets/texture/decal/blood-decal.material", "" )
    ent = decal( scen, -10.466140747070312, 0.24288368225097656, -0.3087177276611328, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, "assets/texture/decal/blood-decal.material", "" )
    ent = decal( scen, -2.809680461883545, 0.9870482087135315, 10.105459213256836, 0.0, -0.09264829754829407, 0.0, 0.9956989288330078, 0.711990475654602, 2.033860921859741, 0.6453677415847778, "assets/texture/decal/radioactive-goo-decal.material", "" )
    ent = decal( scen, -1.7946031093597412, 0.8968799710273743, 9.385228157043457, 0.0, 0.9804826974868774, 0.0, 0.19660554826259613, 1.0, 2.033860921859741, 1.0, "assets/texture/decal/radioactive-goo-decal.material", "" )
    ent = decal( scen, -3.4782917499542236, 0.0, 9.45339584350586, 0.0, 0.5068062543869019, 0.0, 0.8620601296424866, 1.0, 1.0, 1.0, "assets/texture/decal/radioactive-goo-decal.material", "" )
    ent = decal( scen, -2.3612680435180664, 0.0, 8.373587608337402, 0.0, -0.021065587177872658, 0.0, 0.999778151512146, 1.0, 1.0, 1.0, "assets/texture/decal/radioactive-goo-decal.material", "" )
    ent = decal( scen, 0.0, 0.0, 8.593814849853516, 0.0, 0.42362329363822937, 0.0, 0.9058384299278259, 1.0, 1.0, 1.0, "assets/texture/decal/radioactive-goo-decal.material", "" )
end
return scen
