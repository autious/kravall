local entities = require "entities"
local scenario = require "scenario"
local scen = scenario.new()

scen.name = "Default Scenario Name"
scen.description = "Default Description"

scen.cameras = scen.cameras or {}

local script = dofile "scripts/scenarios/presentation_script.lua"( scen )

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
    
    scen.asm:specific_content( core.contentmanager.load( core.loaders.NavigationMeshLoader, "scripts/scenarios/presentation.nav", function( value ) end, false ) )

    
    local function genF( e, f )
        if type( script[f] ) == "function"  then
            return function( delta ) script[ f ]( e, delta ) end
        else
            error( "Function " .. f .. " does not exist in script file" )
        end
    end
    
    directional( scen, -0.8414633870124817, -0.49371418356895447, 0.2195121943950653, 1.0, 1.0, 1.0, 1.0 )
    ambient( scen, 1.0, 1.0, 1.0, 0.5 )

    ent = pointLight( scen, 12.743307113647461, 0.3262444734573364, 17.125577926635742, 0.0, 0.7307690382003784, 0.0, 3.119999885559082, 5.0, "" )
    ent = spotLight( scen, 12.961088180541992, 5.418888092041016, 18.815767288208008, 0.0, -1.0, -0.0, 1.0, 1.0, 1.0, 0.8203047513961792, 0.4101523756980896, 20.0, 20.0, "" )
    ent = spotLight( scen, 6.9880757331848145, 5.418888092041016, 18.815767288208008, 0.0, -1.0, -0.0, 1.0, 1.0, 1.0, 0.8203047513961792, 0.4101523756980896, 20.0, 20.0, "" )
    ent = spotLight( scen, 0.06571441143751144, 5.418888092041016, 18.815767288208008, 0.0, -1.0, -0.0, 1.0, 1.0, 1.0, 0.8203047513961792, 0.4101523756980896, 20.0, 20.0, "" )

    ent = staticModel( scen, 12.890205383300781, 0.9168716669082642, 17.985736846923828, 0.0, 0.7872470617294312, 0.0, 0.6166377067565918, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, 13.231575965881348, 0.025325387716293335, 17.857702255249023, 0.0, 0.47347891330718994, 0.0, 0.8808051943778992, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, 12.515209197998047, 0.025325387716293335, 18.126384735107422, 0.0, 0.17845198512077332, 0.0, 0.9839486479759216, 1.0, "assets/model/static/props/barrel_00.bgnome", "assets/texture/static/props/barrel_00.material", "" )
    ent = staticModel( scen, 12.895374298095703, 4.934889316558838, 19.293107986450195, 0.0, 1.0, 0.0, -7.549790126404332e-08, 1.0, "assets/model/static/props/lamp-0_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, 6.922361373901367, 4.934889316558838, 19.293107986450195, 0.0, 1.0, 0.0, -7.549790126404332e-08, 1.0, "assets/model/static/props/lamp-0_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, 0.06571441143751144, 0.23964643478393555, 16.013505935668945, 0.0, 0.49497997760772705, 0.0, 0.8689044117927551, 1.0, "assets/model/static/props/car_00.bgnome", "assets/texture/static/props/car_00.material", "" )
    ent = staticModel( scen, 0.0, 4.934889316558838, 19.293107986450195, 0.0, 1.0, 0.0, -7.549790126404332e-08, 1.0, "assets/model/static/props/lamp-0_00.bgnome", "assets/texture/static/building/lamp_00.material", "" )
    ent = staticModel( scen, 267.7325134277344, 0.0, -31.54338836669922, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/street/scenario1-square_00.bgnome", "assets/texture/static/street/asphalt_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, 37.62161636352539, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-top-flat_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-top-flat_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, 37.62161636352539, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-top-edge_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-top-edge_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, 37.62161636352539, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-roof-angle_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-roof-angle_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, 37.62161636352539, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-main-3-windows_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-main-3-windows_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, 37.62161636352539, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-main-2-windows_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-main-2-windows_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, 37.62161636352539, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-foundation_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-foundation_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, 37.62161636352539, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-doors_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-doors_00.material", "" )
    ent = staticModel( scen, 0.0, 0.0, 37.62161636352539, 0.0, 0.0, 0.0, 1.0, 1.0, "assets/model/static/building/apartment/s01-h/0-corners_00.bgnome", "assets/texture/static/building/apartment/s01-h/0-corners_00.material", "" )

    ent = decal( scen, 12.776206970214844, 0.010926834307610989, 17.343700408935547, 0.0, 0.22520239651203156, 0.0, 0.9743120074272156, 1.0, 1.0, 1.0, "assets/texture/decal/radioactive-goo-decal.material", "" )
end
return scen
