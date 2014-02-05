local entities = require "entities"
local scenario = require "scenario"
local scen = scenario.new()

scen.name = "Default Scenario Name"
scen.description = "Default Description"

scen.cameras = scen.cameras or {}

local script = dofile "scripts/scenarios/scenario1-level_script.lua"( scen )

function scen:load()
    local ambient = entities.get "ambientLight"
    local directional = entities.get "directionalLight"
    local pointLight = entities.get "pointLight"
    local spotLight = entities.get "spotLight"
    local area = entities.get "area"
    local rioter = entities.get "rioter"
    local staticModel = entities.get "staticModel"
    
    scen.asm:specific_content( core.contentmanager.load( core.loaders.NavigationMeshLoader, "scripts/scenarios/scenario1-level.nav", function( value ) end, false ) )

    
    local function genF( e, f )
        if type( script[f] ) == "function"  then
            return function() script[ f ]( e ) end
        else
            error( "Function " .. f .. " does not exist in script file" )
        end
    end
    
    directional( scen, 0.6643835306167603, -0.6150288581848145, 0.42465752363204956, 1.0, 0.9453219771385193, 0.7452670335769653, 0.7205882668495178 )
    ambient( scen, 0.6720433235168457, 0.8547968864440918, 1.0, 0.27941176295280457 )

    local ent = area( scen,     {-44.24905776977539, 0, 49.49397277832031},     {-4.055175304412842, 4.055175304412842,     -4.055175304412842, -4.055175304412842,     4.055175304412842, -4.055175304412842,     4.055175304412842, 4.055175304412842},     "policeArea1" )
    scen:registerInitCallback( genF( ent, "createPoliceSquad" ) )

    local ent = area( scen,     {-61.939453125, 0, 34.66481018066406},     {-3.9302239418029785, 3.9302239418029785,     -3.9302239418029785, -3.9302239418029785,     3.9302239418029785, -3.9302239418029785,     3.9302239418029785, 3.9302239418029785},     "policeArea1" )
    scen:registerInitCallback( genF( ent, "createPoliceSquad" ) )

    local ent = area( scen,     {38.40287780761719, 0, -124.29673767089844},     {-8.91488265991211, 8.91488265991211,     -8.91488265991211, -8.91488265991211,     8.91488265991211, -8.91488265991211,     8.91488265991211, 8.91488265991211},     "angry1" )
    scen:registerInitCallback( genF( ent, "createRioter_3" ) )
    scen:registerTickCallback( genF( ent, "track" ) )

    local ent = area( scen,     {-122.22075653076172, 0, 33.37147903442383},     {-8.557184219360352, 8.557184219360352,     -8.557184219360352, -8.557184219360352,     8.557184219360352, -8.557184219360352,     8.557184219360352, 8.557184219360352},     "angry1" )
    scen:registerInitCallback( genF( ent, "createRioter_2" ) )
    scen:registerTickCallback( genF( ent, "track" ) )

    local ent = area( scen,     {38.5906867980957, 0, 121.1289291381836},     {-8.871253967285156, 10.870834350585938,     -8.871253967285156, -6.871673583984375,     8.871253967285156, -6.871673583984375,     8.871253967285156, 10.870834350585938},     "angry1" )
    scen:registerInitCallback( genF( ent, "createRioter_1" ) )
    scen:registerTickCallback( genF( ent, "track" ) )

    local ent = area( scen,     {-60.2327880859375, 0, -218.23377990722656},     {-12.307221412658691, 12.815917015075684,     -12.307221412658691, -9.872344017028809,     10.3810396194458, -9.872344017028809,     10.3810396194458, 12.815917015075684},     "goal4" )

    local ent = area( scen,     {117.35212707519531, 0, -217.65927124023438},     {-11.631661415100098, 12.136002540588379,     -11.631661415100098, -12.136002540588379,     12.64034366607666, -12.136002540588379,     12.64034366607666, 12.136002540588379},     "goal3" )

    local ent = area( scen,     {122.8984603881836, 0, -45.691444396972656},     {-9.311534881591797, 9.483535766601562,     -9.311534881591797, -11.256767272949219,     11.428768157958984, -11.256767272949219,     11.428768157958984, 9.483535766601562},     "goal2" )

    local ent = area( scen,     {-45.50669479370117, 0, -46.472023010253906},     {-10.193031311035156, 10.193031311035156,     -10.193031311035156, -10.193031311035156,     10.193031311035156, -10.193031311035156,     10.193031311035156, 10.193031311035156},     "goal1" )

    local ent = area( scen,     {-44.694461822509766, 0, 169.2928466796875},     {-9.7553071975708, 10.207505226135254,     -9.7553071975708, -7.405930519104004,     7.858128547668457, -7.405930519104004,     7.858128547668457, 10.207505226135254},     "start" )
    scen:registerInitCallback( genF( ent, "createRioter_0" ) )
    scen:registerTickCallback( genF( ent, "setDestination" ) )

    local ent = pointLight( scen, -4.364441871643066, 0.6289991736412048, -3.9723050594329834, 1.0, 1.0, 1.0, 29.999980926513672, 1.0, "" )
    local ent = pointLight( scen, 4.076244831085205, 5.903861999511719, -1.0054539442062378, 1.0, 1.0, 1.0, 29.999980926513672, 1.0, "" )

    local ent = staticModel( scen, -44.40789031982422, 0.0, 249.193603515625, 0.0, 0.0, 0.0, 1.0, 1.3679145574569702, "assets/model/static/building/tmp/whitebox.bgnome", "assets/material/static/building/orange.material", "" )
    local ent = staticModel( scen, 8.080698013305664, -1.0639421816449612e-05, 155.2982177734375, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-b/1-windows.bgnome", "assets/material/static/building/window-sandframe-64_00.material", "b1" )
    local ent = staticModel( scen, 8.080698013305664, -1.0639421816449612e-05, 155.2982177734375, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-b/1-frame.bgnome", "assets/material/static/building/concrete-sand-32_00.material", "b1" )
    local ent = staticModel( scen, 8.080698013305664, -1.0639421816449612e-05, 155.2982177734375, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-b/1-main.bgnome", "assets/material/static/building/brickwall-32_01.material", "b1" )
    local ent = staticModel( scen, -25.246007919311523, -1.0609626770019531e-05, 154.67922973632812, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-b/0-frame.bgnome", "assets/material/static/building/concrete-32_01.material", "b1" )
    local ent = staticModel( scen, -25.246007919311523, -1.0609626770019531e-05, 154.67922973632812, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-b/0-windows.bgnome", "assets/material/static/building/window-concrete-128_01.material", "b1" )
    local ent = staticModel( scen, -25.246007919311523, -1.0609626770019531e-05, 154.67922973632812, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, 1.0, "assets/model/static/building/apartment/s01-b/0-main.bgnome", "assets/material/static/building/brickwall-16_00.material", "b1" )

end
return scen
