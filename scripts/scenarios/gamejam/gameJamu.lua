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

scen.asm:specific_content( core.contentmanager.load( core.loaders.NavigationMeshLoader, "scripts/scenarios/gamejam/gameJamu.nav", function( value ) end, false ) )


local script = dofile "scripts/scenarios/gameJamu_script.lua"( scen )

local function genF( e, f )
    if type( script[f] ) == "function"  then
        return function() script[ f ]( e ) end
    else
        error( "Function " .. f .. " does not exist in script file" )
    end
end

directional( scen, -0.545976996421814, -0.7968841791152954, -0.2586206793785095, 1.0, 0.8689313530921936, 0.69304358959198, 0.0 )
ambient( scen, 0.5842457413673401, 0.857562243938446, 1.0, 0.009999999776482582 )

scen.cameras = scen.cameras or {}



return scen
