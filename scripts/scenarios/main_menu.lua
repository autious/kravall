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

--scen.asm:specific_content( core.contentmanager.load( core.loaders.NavigationMeshLoader, "scripts/scenarios/main_menu.nav", function( value ) end, false ) )

local script = dofile "scripts/scenarios/main_menu_script.lua"( scen )

local function genF( e, f )
    if type( script[f] ) == "function"  then
        return function() script[ f ]( e ) end
    else
        error( "Function " .. f .. " does not exist in script file" )
    end
end

directional( scen, 1.5707963705062866, -0.0, 0.0, 1.0, 1.0, 1.0, 1.0 )
ambient( scen, 1.0, 1.0, 1.0, 0.5 )

local ent = staticModel( scen, 0.0, 0.0, 22.39171600341797, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/ui/testMenue.bgnome", "assets/material/test.material", "" )

return scen
