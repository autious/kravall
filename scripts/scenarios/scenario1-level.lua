local entities = require "entities"
local scenario = require "scenario"
local scen = scenario.new()

local ambient = entities.get "ambientLight"
local directional = entities.get "directionalLight"
local pointLight = entities.get "pointLight"
local spotLight = entities.get "spotLight"
local area = entities.get "area"
local rioter = entities.get "rioter"
local staticModel = entities.get "staticModel"

scen.asm:specific_content( core.contentmanager.load( core.loaders.NavigationMeshLoader, "scripts/scenarios/scenario1-level.nav", function( value ) end, false ) )


local script = dofile "scripts/scenarios/scenario1-level_script.lua"( scen )

local function genF( e, f )
    if type( script[f] ) == "function"  then
        return function() script[ f ]( e ) end
    else
        error( "Function " .. f .. " does not exist in script file" )
    end
end

directional( scen, 1.5707963705062866, -0.0, 0.0, 1.0, 1.0, 1.0, 1.0 )
ambient( scen, 1.0, 1.0, 1.0, 0.5 )

scen.cameras = scen.cameras or {}
local ent = area( scen, {-44.24905776977539, 0, 49.49397277832031}, {-4.055175304412842, 4.055175304412842, -4.055175304412842, -4.055175304412842, 4.055175304412842, -4.055175304412842, 4.055175304412842, 4.055175304412842}, "policeArea1" )
scen:registerInitCallback( genF( ent, "createPoliceSquad" ) )

local ent = area( scen, {-61.939453125, 0, 34.66481018066406}, {-3.9302239418029785, 3.9302239418029785, -3.9302239418029785, -3.9302239418029785, 3.9302239418029785, -3.9302239418029785, 3.9302239418029785, 3.9302239418029785}, "policeArea1" )
scen:registerInitCallback( genF( ent, "createPoliceSquad" ) )

local ent = area( scen, {-45.64253234863281, 0, 170.38099670410156}, {-8.91488265991211, 8.91488265991211, -8.91488265991211, -8.91488265991211, 8.91488265991211, -8.91488265991211, 8.91488265991211, 8.91488265991211}, "angry1" )
scen:registerInitCallback( genF( ent, "createRioter_3" ) )
scen:registerTickCallback( genF( ent, "track" ) )

local ent = area( scen, {-122.22075653076172, 0, 33.37147903442383}, {-8.557184219360352, 8.557184219360352, -8.557184219360352, -8.557184219360352, 8.557184219360352, -8.557184219360352, 8.557184219360352, 8.557184219360352}, "angry1" )
scen:registerInitCallback( genF( ent, "createRioter_2" ) )
scen:registerTickCallback( genF( ent, "track" ) )

local ent = area( scen, {38.5906867980957, 0, 121.1289291381836}, {-8.871253967285156, 10.870834350585938, -8.871253967285156, -6.871673583984375, 8.871253967285156, -6.871673583984375, 8.871253967285156, 10.870834350585938}, "angry1" )
scen:registerInitCallback( genF( ent, "createRioter_1" ) )
scen:registerTickCallback( genF( ent, "track" ) )

local ent = area( scen, {-40.44048309326172, 0, -193.0405731201172}, {-12.307221412658691, 12.815917015075684, -12.307221412658691, -9.872344017028809, 10.3810396194458, -9.872344017028809, 10.3810396194458, 12.815917015075684}, "goal4" )

local ent = area( scen, {117.35212707519531, 0, -191.85108947753906}, {-11.631661415100098, 12.136002540588379, -11.631661415100098, -12.136002540588379, 12.64034366607666, -12.136002540588379, 12.64034366607666, 12.136002540588379}, "goal3" )

local ent = area( scen, {116.70249938964844, 0, -45.691444396972656}, {-9.311534881591797, 9.483535766601562, -9.311534881591797, -11.256767272949219, 11.428768157958984, -11.256767272949219, 11.428768157958984, 9.483535766601562}, "goal2" )

local ent = area( scen, {-45.50669479370117, 0, -46.472023010253906}, {-10.193031311035156, 10.193031311035156, -10.193031311035156, -10.193031311035156, 10.193031311035156, -10.193031311035156, 10.193031311035156, 10.193031311035156}, "goal1" )

local ent = area( scen, {40.907344818115234, 0, -127.9789047241211}, {-9.7553071975708, 10.207505226135254, -9.7553071975708, -7.405930519104004, 7.858128547668457, -7.405930519104004, 7.858128547668457, 10.207505226135254}, "start" )
scen:registerInitCallback( genF( ent, "createRioter_0" ) )
scen:registerTickCallback( genF( ent, "setDestination" ) )




return scen
