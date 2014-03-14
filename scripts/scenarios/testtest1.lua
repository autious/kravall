local entities = require "entities"
local scenario = require "scenario"
local scen = scenario.new()

scen.name = "Default Scenario Name"
scen.description = "Default Description"

scen.cameras = scen.cameras or {}
scen.cameras["devcam"] = {
    view = core.glm.mat4.new(
    -0.7041349411010742, -0.48494094610214233, 0.5186774730682373, -0.0,
    -2.980232238769531e-07, 0.7304637432098389, 0.6829515099525452, 0.0,
    -0.7100661993026733, 0.4808898866176605, -0.5143451690673828, 0.0,
    -0.19291740655899048, 0.9632153511047363, -6.277410507202148, 1.0 ),
    fov = 0.8575560450553894,
    quaternion = core.glm.quat.new( 0.1411971151828766, -0.8586244583129883, -0.33886799216270447, 0.35776516795158386 ),
    translation = core.glm.vec3.new( 3.5872139930725098, 3.58357310295105, -3.8289406299591064 )
}

local script = dofile "scripts/scenarios/testtest1_script.lua"( scen )

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
    
    scen.asm:specific_content( core.contentmanager.load( core.loaders.NavigationMeshLoader, "scripts/scenarios/testtest1.nav", function( value ) end, false ) )

    
    local function genF( e, f )
        if type( script[f] ) == "function"  then
            return function( delta ) script[ f ]( e, delta ) end
        else
            error( "Function " .. f .. " does not exist in script file" )
        end
    end
    
    directional( scen, -0.5950919985771179, -0.7691010236740112, 0.23312883079051971, 1.0, 1.0, 1.0, 0.25 )
    ambient( scen, 0.2824733853340149, 0.4258257746696472, 1.0, 0.25 )

    ent = area( scen, {0.2036857008934021, 0, -4.238103866577148}, {-2.146284341812134, 2.146284341812134, -2.146284341812134, -2.146284341812134, 2.146284341812134, -2.146284341812134, 2.146284341812134, 2.146284341812134}, "" )
    scen:registerInitCallback( genF( ent, "CreateGroupB" ) )

    ent = area( scen, {0.2036857008934021, 0, 4.2552666664123535}, {-2.146284341812134, 2.146284341812134, -2.146284341812134, -2.146284341812134, 2.146284341812134, -2.146284341812134, 2.146284341812134, 2.146284341812134}, "" )
    scen:registerInitCallback( genF( ent, "CreateGroupA" ) )



end
return scen
