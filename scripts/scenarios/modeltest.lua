local entities = require "entities"
local scenario = require "scenario"
local scen = scenario.new()

scen.name = "Default Scenario Name"
scen.description = "Default Description"

scen.cameras = scen.cameras or {}

local script = dofile "scripts/scenarios/modeltest_script.lua"( scen )

function scen:load()
    local ambient = entities.get "ambientLight"
    local directional = entities.get "directionalLight"
    local pointLight = entities.get "pointLight"
    local spotLight = entities.get "spotLight"
    local area = entities.get "area"
    local rioter = entities.get "rioter"
    local staticModel = entities.get "staticModel"
    local empty = entities.get "empty"
    
    local function genF( e, f )
        if type( script[f] ) == "function"  then
            return function( delta ) script[ f ]( e, delta ) end
        else
            error( "Function " .. f .. " does not exist in script file" )
        end
    end
    
    directional( scen, -0.0, -1.0, 0.0, 1.0, 1.0, 1.0, 1.0 )
    ambient( scen, 1.0, 1.0, 1.0, 0.5 )


    local ent = staticModel( scen, -22.918071746826172, 0.0, -0.0, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/1-winpane.bgnome", "assets/texture/static/building/concrete-64_01.material", "" )
    local ent = staticModel( scen, -22.918071746826172, 0.0, -0.0, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/1-windows.bgnome", "assets/texture/static/building/window-plain-grey_00.material", "" )
    local ent = staticModel( scen, -22.918071746826172, 0.0, -0.0, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/1-roofedge.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    local ent = staticModel( scen, -22.918071746826172, 0.0, -0.0, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/1-roof.bgnome", "assets/texture/static/building/concrete-64_01.material", "" )
    local ent = staticModel( scen, -22.918071746826172, 0.0, -0.0, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/1-main.bgnome", "assets/texture/static/building/brickwall-darkred_00.material", "" )
    local ent = staticModel( scen, -22.918071746826172, 0.0, -0.0, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/1-door.bgnome", "assets/texture/static/building/garage-port-64_01.material", "" )
    local ent = staticModel( scen, 17.44530487060547, 0.0, -0.0, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/4-windows.bgnome", "assets/texture/static/building/window-plain-grey_00.material", "" )
    local ent = staticModel( scen, 17.44530487060547, 0.0, -0.0, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/4-roof.bgnome", "assets/texture/static/building/concrete-64_01.material", "" )
    local ent = staticModel( scen, 17.44530487060547, 0.0, -0.0, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/4-pillars.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    local ent = staticModel( scen, 17.44530487060547, 0.0, -0.0, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/4-main.bgnome", "assets/texture/static/building/brickwall-64_03.material", "" )
    local ent = staticModel( scen, 17.44530487060547, 0.0, -0.0, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/4-floordivide.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    local ent = staticModel( scen, 17.44530487060547, 0.0, -0.0, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/4-door.bgnome", "assets/texture/static/building/garage-port-64_01.material", "" )
    local ent = staticModel( scen, 1.3913826942443848, 0.0, -0.0, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/3-windows.bgnome", "assets/texture/static/building/window-plain-brown_00.material", "" )
    local ent = staticModel( scen, 1.3913826942443848, 0.0, -0.0, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/3-roof.bgnome", "assets/texture/static/building/concrete-64_01.material", "" )
    local ent = staticModel( scen, 1.3913826942443848, 0.0, -0.0, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/3-pillars.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    local ent = staticModel( scen, 1.3913826942443848, 0.0, -0.0, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/3-main.bgnome", "assets/texture/static/building/brickwall-32_00.material", "" )
    local ent = staticModel( scen, 1.3913826942443848, 0.0, -0.0, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/3-door.bgnome", "assets/texture/static/building/garage-port-64_01.material", "" )
    local ent = staticModel( scen, -10.953140258789062, 0.0, -0.0, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/2-windows.bgnome", "assets/texture/static/building/window-plain-grey_00.material", "" )
    local ent = staticModel( scen, -10.953140258789062, 0.0, -0.0, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/2-roofedge.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    local ent = staticModel( scen, -10.953140258789062, 0.0, -0.0, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/2-main.bgnome", "assets/texture/static/building/concrete-sand-32_00.material", "" )
    local ent = staticModel( scen, -10.953140258789062, 0.0, -0.0, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/2-doorhandle.bgnome", "assets/texture/static/building/metal-grey_00.material", "" )
    local ent = staticModel( scen, -10.953140258789062, 0.0, -0.0, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/2-dooredge.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    local ent = staticModel( scen, -10.953140258789062, 0.0, -0.0, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/2-door.bgnome", "assets/texture/static/building/garage-port-64_01.material", "" )
    local ent = staticModel( scen, 0.0, 0.0, 46.26560592651367, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/0-windows.bgnome", "assets/texture/static/building/wincolor-64_00.material", "" )
    local ent = staticModel( scen, 0.0, 0.0, 46.26560592651367, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/0-roof.bgnome", "assets/texture/static/building/concrete-64_01.material", "" )
    local ent = staticModel( scen, 0.0, 0.0, 46.26560592651367, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/0-main.bgnome", "assets/texture/static/building/brickwall-64_01.material", "" )

end
return scen
