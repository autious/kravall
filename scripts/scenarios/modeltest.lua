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


    local ent = staticModel( scen, -1.9268417358398438, 1.2505719661712646, -6.480384349822998, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s00-a/1-roof_00.bgnome", "assets/texture/static/building/concrete-128_02.material", "" )
    local ent = staticModel( scen, -1.9268417358398438, 1.2505719661712646, -6.480384349822998, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s00-a/1-main_00.bgnome", "assets/texture/static/building/window-128_03.material", "" )
    local ent = staticModel( scen, -1.9268417358398438, 1.2505719661712646, -6.480384349822998, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s00-a/1-corners_00.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    local ent = staticModel( scen, -50.08200454711914, 1.2505719661712646, -5.246585369110107, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s00-a/0-winFrames_00.bgnome", "assets/texture/static/building/concrete-orange-32_00.material", "" )
    local ent = staticModel( scen, -50.08200454711914, 1.2505719661712646, -5.246585369110107, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s00-a/0-windows_00.bgnome", "assets/texture/static/building/wincolor-64_00.material", "" )
    local ent = staticModel( scen, -50.08200454711914, 1.2505719661712646, -5.246585369110107, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s00-a/0-main_00.bgnome", "assets/texture/static/building/concrete-orange-32_00.material", "" )
    local ent = staticModel( scen, -50.08200454711914, 1.2505719661712646, -5.246585369110107, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s00-a/0-corners_00.bgnome", "assets/texture/static/building/concrete-64_01.material", "" )
    local ent = staticModel( scen, -22.795700073242188, 0.0, -1.960296630859375, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/4-windows.bgnome", "assets/texture/static/building/window-plain-grey_00.material", "" )
    local ent = staticModel( scen, -22.795700073242188, 0.0, -1.960296630859375, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/4-roof.bgnome", "assets/texture/static/building/concrete-64_01.material", "" )
    local ent = staticModel( scen, -22.795700073242188, 0.0, -1.960296630859375, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/4-pillars.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    local ent = staticModel( scen, -22.795700073242188, 0.0, -1.960296630859375, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/4-main.bgnome", "assets/texture/static/building/brickwall-64_03.material", "" )
    local ent = staticModel( scen, -22.795700073242188, 0.0, -1.960296630859375, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/4-floordivide.bgnome", "assets/texture/static/building/concrete-32_00.material", "" )
    local ent = staticModel( scen, -22.795700073242188, 0.0, -1.960296630859375, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-d/4-door.bgnome", "assets/texture/static/building/garage-port-64_01.material", "" )
    local ent = staticModel( scen, -20.24264907836914, 29.71091079711914, -2.6201400756835938, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/props/ventilation/1-vent-top.bgnome", "assets/texture/static/building/metal-grey_00.material", "" )
    local ent = staticModel( scen, -20.24264907836914, 29.71091079711914, -2.6201400756835938, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/props/ventilation/1-vent-main.bgnome", "assets/texture/static/building/metal-black_00.material", "" )
    local ent = staticModel( scen, -21.477937698364258, 25.115493774414062, 11.083052635192871, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/props/ventilation/2-vent-main.bgnome", "assets/texture/static/props/ventbg-32_00.material", "" )
    local ent = staticModel( scen, -21.477937698364258, 25.115493774414062, 11.083052635192871, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/props/ventilation/2-vent-front.bgnome", "assets/texture/static/props/airconditioner-64_00.material", "" )
    local ent = staticModel( scen, -21.477937698364258, 25.115493774414062, 11.083052635192871, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/props/ventilation/2-vent-bottom.bgnome", "assets/texture/static/building/metal-grey_00.material", "" )
    local ent = staticModel( scen, -23.2830867767334, 29.690292358398438, 6.0535454750061035, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/props/ventilation/0-vent-top.bgnome", "assets/texture/static/props/vent-64_00.material", "" )
    local ent = staticModel( scen, -23.2830867767334, 29.690292358398438, 6.0535454750061035, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/props/ventilation/0-vent-main.bgnome", "assets/texture/static/props/ventside-64_00.material", "" )
    local ent = staticModel( scen, -23.2830867767334, 29.690292358398438, 6.0535454750061035, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/props/ventilation/0-vent-bottom.bgnome", "assets/texture/static/building/metal-grey_00.material", "" )

end
return scen
