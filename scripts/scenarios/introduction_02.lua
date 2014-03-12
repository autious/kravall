local entities = require "entities"
local scenario = require "scenario"
local scen = scenario.new()

scen.name = "Default Scenario Name"
scen.description = "Default Description"

scen.cameras = scen.cameras or {}
scen.cameras["startcam"] = {
    view = core.glm.mat4.new(
    1.0, -0.0, 0.0, -0.0,
    -0.0, 0.3142477869987488, 0.9493409991264343, 0.0,
    0.0, -0.9493409991264343, 0.3142477869987488, -0.0,
    30.450748443603516, -2.3430795669555664, -56.64949035644531, 1.0 ),
    fov = 1.0471980571746826,
    quaternion = core.glm.quat.new( 0.5855562090873718, 0.0, 0.0, 0.8106318116188049 ),
    translation = core.glm.vec3.new( -30.450748443603516, 54.5159912109375, 15.577595710754395 )
}

local script = dofile "scripts/scenarios/introduction_02_script.lua"( scen )

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
    
    scen.asm:specific_content( core.contentmanager.load( core.loaders.NavigationMeshLoader, "scripts/scenarios/introduction_02.nav", function( value ) end, false ) )

    
    local function genF( e, f )
        if type( script[f] ) == "function"  then
            return function( delta ) script[ f ]( e, delta ) end
        else
            error( "Function " .. f .. " does not exist in script file" )
        end
    end
    
    directional( scen, -0.45121949911117554, -0.464176207780838, 0.7621951103210449, 1.0, 1.0, 1.0, 1.0 )
    ambient( scen, 1.0, 1.0, 1.0, 0.5 )

    ent = area( scen, {-7.673438549041748, 0, 0.7990899085998535}, {-4.098126411437988, 4.098126411437988, -4.098126411437988, -4.098126411437988, 4.098126411437988, -4.098126411437988, 4.098126411437988, 4.098126411437988}, "" )
    scen:registerInitCallback( genF( ent, "registerSpawn" ) )

    ent = area( scen, {-44.53824234008789, 0, 0.8044350743293762}, {-4.299887657165527, 3.796531915664673, -4.299887657165527, -3.796531915664673, 4.299887657165527, -3.796531915664673, 4.299887657165527, 3.796531915664673}, "" )
    scen:registerInitCallback( genF( ent, "spawnAgitatorsOnMain" ) )


    ent = staticModel( scen, -53.73119354248047, 2.1281705642195448e-07, 8.5245361328125, 0.0, -0.4966583847999573, 0.0, 0.8679461479187012, { 1.0, 1.0, 1.0 }, "assets/model/static/props/car_00.bgnome", "assets/texture/static/props/car_00.material", "" )
    ent = staticModel( scen, -51.00545883178711, 2.1281705642195448e-07, 11.50390625, 0.0, 0.0, 0.0, 1.0, { 1.0, 1.0, 1.0 }, "assets/model/static/props/car_00.bgnome", "assets/texture/static/props/car_00.material", "" )
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
    ent = staticModel( scen, -49.827552795410156, 0.0, 0.6261359453201294, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, { 1.0, 1.0, 1.0 }, "assets/model/static/props/fence-bars_00.bgnome", "assets/texture/static/props/rust-32_00.material", "" )
    ent = staticModel( scen, -49.827552795410156, 0.0, 0.6261359453201294, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, { 1.0, 1.0, 1.0 }, "assets/model/static/props/fence-grid_00.bgnome", "assets/texture/static/props/grid-32_00.material", "" )
    ent = staticModel( scen, -2.9054794311523438, 0.0, 0.6261359453201294, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, { 1.0, 1.0, 1.0 }, "assets/model/static/props/fence-grid_00.bgnome", "assets/texture/static/props/grid-32_00.material", "" )
    ent = staticModel( scen, -2.9054794311523438, 0.0, 0.6261359453201294, 0.0, 0.7071068286895752, 0.0, 0.7071068286895752, { 1.0, 1.0, 1.0 }, "assets/model/static/props/fence-bars_00.bgnome", "assets/texture/static/props/rust-32_00.material", "" )
    ent = staticModel( scen, -48.210906982421875, 2.365198952247738e-07, -8.594670295715332, 0.0, 1.0, 0.0, 7.549790126404332e-08, { 1.1307967901229858, 1.0, 1.0 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, -57.36458969116211, 4.039292335510254, 10.405611038208008, 0.0, 1.0, 0.0, 7.549789415861596e-08, { 0.5178174376487732, 0.1549662947654724, 1.0 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, -17.39337921142578, 4.3398244997661095e-09, -8.484456062316895, 0.0, 1.0, 0.0, 7.549790126404332e-08, { 1.0, 1.0, 1.0 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, -21.171916961669922, 5.468166932587337e-07, 10.275938987731934, 0.0, 1.0, 0.0, 7.549790126404332e-08, { 1.2421401739120483, 1.0, 1.0 }, "assets/model/static/building/whitebox/building_00.bgnome", "assets/texture/orange.material", "" )
    ent = staticModel( scen, -33.75909423828125, -0.29442429542541504, 0.8790919780731201, 0.0, 0.0, 0.0, 1.0, { 0.5359215140342712, 1.0406486988067627, 0.34208089113235474 }, "assets/model/static/street/whitebox/street_00.bgnome", "assets/texture/grey.material", "" )

    ent = decal( scen, -44.50523376464844, -1.0383479595184326, 0.7990908622741699, -1.5099580252808664e-07, 0.0, 0.0, 1.0, 8.220477104187012, 8.220477104187012, 8.220477104187012, "assets/texture/decal/agitator-warning.material", "" )
    scen:registerInitCallback( genF( ent, "hideOnMain" ) )
    ent = decal( scen, -49.161842346191406, -0.4405651390552521, 14.199278831481934, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, "assets/texture/decal/radioactive-goo-decal.material", "" )
    ent = decal( scen, -7.673438549041748, -1.0383479595184326, 0.7990908622741699, -1.5099580252808664e-07, 0.0, 0.0, 1.0, 8.220477104187012, 8.220477104187012, 8.220477104187012, "assets/texture/decal/spawn-decal.material", "" )
    scen:registerInitCallback( genF( ent, "hideOnMain" ) )
end
return scen
