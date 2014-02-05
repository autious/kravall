local entities = require "entities"
local scenario = require "scenario"
local scen = scenario.new()

local ambient = entities.get "ambientLight"
local directional = entities.get "directionalLight"
local street_light = entities.get "streetLight"
local area = entities.get "area"
local rioter = entities.get "rioter"

local script = dofile "scripts/scenarios/level1-whitebox_script.lua"( scen )

local function genF( e, f )
    if type( script[f] ) == "function"  then
        return function() script[ f ]( e ) end
    else
        error( "Function " .. f .. " does not exist in script file" )
    end
end

scen.asm:specific_content( core.contentmanager.load( core.loaders.NavigationMeshLoader, "scripts/scenarios/level1-whitebox.nav", function( value ) end, false ) )

local ent = area( scen, {-18.679946899414062, 0, -190.99557495117188}, {-8.275400161743164, 8.275400161743164, -8.275400161743164, -8.275400161743164, 8.275400161743164, -8.275400161743164, 8.275400161743164, 8.275400161743164}, "area4" )

local ent = area( scen, {116.83180236816406, 0, -190.68045043945312}, {-8.275400161743164, 8.275400161743164, -8.275400161743164, -8.275400161743164, 8.275400161743164, -8.275400161743164, 8.275400161743164, 8.275400161743164}, "area3" )

local ent = area( scen, {118.19918823242188, 0, -47.10471725463867}, {-8.275400161743164, 8.275400161743164, -8.275400161743164, -8.275400161743164, 8.275400161743164, -8.275400161743164, 8.275400161743164, 8.275400161743164}, "area2" )

local ent = area( scen, {-45.70919418334961, 0, -44.9755973815918}, {-7.435268402099609, 7.435268402099609, -7.435268402099609, -7.435268402099609, 7.435268402099609, -7.435268402099609, 7.435268402099609, 7.435268402099609}, "area1" )

local ent = area( scen, {-45.965911865234375, 0, 167.45249938964844}, {-5.631386756896973, 5.963907241821289, -5.631386756896973, -5.298865795135498, 5.631386756896973, -5.298865795135498, 5.631386756896973, 5.963907241821289}, "" )
scen:registerInitCallback( genF( ent, "createRioter_0" ) )
scen:registerTickCallback( genF( ent, "setDestination" ) )


return scen
