local entities = require "entities"
local scenario = require "scenario"
local scen = scenario.new()

scen.name = "Default Scenario Name"
scen.description = "Default Description"

scen.cameras = scen.cameras or {}

local script = dofile "scripts/scenarios/max-police-place_script.lua"( scen )

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
    
    scen.asm:specific_content( core.contentmanager.load( core.loaders.NavigationMeshLoader, "scripts/scenarios/max-police-place.nav", function( value ) end, false ) )

    
    local function genF( e, f )
        if type( script[f] ) == "function"  then
            return function( delta ) script[ f ]( e, delta ) end
        else
            error( "Function " .. f .. " does not exist in script file" )
        end
    end
    
    directional( scen, -0.0, -1.0, 0.0, 1.0, 1.0, 1.0, 0.27737224102020264 )
    ambient( scen, 1.0, 1.0, 1.0, 0.2518247961997986 )

    ent = area( scen, {-27.610185623168945, 0, -1.596445918083191}, {-3.4176018238067627, 3.4176018238067627, -3.4176018238067627, -3.4176018238067627, 3.4176018238067627, -3.4176018238067627, 3.4176018238067627, 3.4176018238067627}, "" )
    scen:registerInitCallback( genF( ent, "spawnRiotersOnMain" ) )

    ent = area( scen, {-11.38118839263916, 0, 0.19888296723365784}, {-3.5933034420013428, 3.5933034420013428, -3.5933034420013428, -3.5933034420013428, 3.5933034420013428, -3.5933034420013428, 3.5933034420013428, 3.5933034420013428}, "" )
    scen:registerInitCallback( genF( ent, "registerSpawn" ) )

    ent = area( scen, {0.9353055953979492, 0, -0.1122860535979271}, {-3.5933034420013428, 3.5933034420013428, -3.5933034420013428, -3.5933034420013428, 3.5933034420013428, -3.5933034420013428, 3.5933034420013428, 3.5933034420013428}, "" )
    scen:registerInitCallback( genF( ent, "registerSpawn" ) )

    ent = spotLight( scen, -20.164817810058594, -4.273693084716797, 13.708491325378418, -0.11095533519983292, 0.993825376033783, -0.0, 1.0, 1.0, 1.0, 0.19587819278240204, 0.10459889255326615, 50.03199768066406, 10000.0, "" )
    ent = spotLight( scen, -20.164817810058594, -4.273693084716797, 25.372024536132812, -0.11095533519983292, 0.993825376033783, -0.0, 1.0, 1.0, 1.0, 0.19587819278240204, 0.10459889255326615, 50.03199768066406, 10000.0, "" )
    ent = spotLight( scen, -20.164817810058594, -4.273693084716797, 35.51907730102539, -0.11095533519983292, 0.993825376033783, -0.0, 1.0, 1.0, 1.0, 0.19587819278240204, 0.10459889255326615, 50.03199768066406, 10000.0, "" )

    ent = staticModel( scen, -44.901527404785156, -3.7172346115112305, 26.703746795654297, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-b/1-windows.bgnome", "assets/texture/static/building/window-sandframe-64_00.material", "" )
    ent = staticModel( scen, -44.901527404785156, -3.7172346115112305, 26.703746795654297, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-b/1-main.bgnome", "assets/texture/static/building/brickwall-32_01.material", "" )
    ent = staticModel( scen, -44.901527404785156, -3.7172346115112305, 26.703746795654297, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-b/1-frame.bgnome", "assets/texture/static/building/concrete-sand-32_00.material", "" )
    ent = staticModel( scen, -11.420299530029297, -3.31034517288208, 0.22364680469036102, 0.0, 0.0, 0.0, 1.0, 3.513718366622925, "assets/model/dev/cube.bgnome", "assets/texture/dev/default.material", "" )
    scen:registerInitCallback( genF( ent, "hideOnMain" ) )
    ent = staticModel( scen, 0.8873360753059387, -3.31034517288208, -0.17642156779766083, 0.0, 0.0, 0.0, 1.0, 3.513718366622925, "assets/model/dev/cube.bgnome", "assets/texture/dev/default.material", "" )
    scen:registerInitCallback( genF( ent, "hideOnMain" ) )

    ent = empty( scen, "", 23.707996368408203, 0.005897998809814453, -1.2461998462677002, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 )
    scen:registerInitCallback( genF( ent, "allGroupGoal" ) )

end
return scen
