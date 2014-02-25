local entities = require "entities"
local scenario = require "scenario"
local scen = scenario.new()

scen.name = "Default Scenario Name"
scen.description = "Default Description"

scen.cameras = scen.cameras or {}

local script = dofile "scripts/scenarios/testtest.lua"( scen )

function scen:load()
    local ambient = entities.get "ambientLight"
    local directional = entities.get "directionalLight"
    local pointLight = entities.get "pointLight"
    local spotLight = entities.get "spotLight"
    local area = entities.get "area"
    local rioter = entities.get "rioter"
    local staticModel = entities.get "staticModel"
	local squad = entities.get "policeSquad"
    
    scen.asm:specific_content( core.contentmanager.load( core.loaders.NavigationMeshLoader, "extremeScenario.txt", function( value ) end, false ) )

    
	
    directional( scen, 0.6643835306167603, -0.6150288581848145, 0.42465752363204956, 1.0, 0.9453219771385193, 0.7452670335769653, 0.7205882668495178 )
    ambient( scen, 0.6720433235168457, 0.8547968864440918, 1.0, 0.017941176295280457 )
	
	local fists = core.weaponData.pushWeapon( 1.0, 0.75, 20, 0.2, 0.05, 3.2, 2.9, 0.05, 0.5, "punch" )
	core.gameMetaData.registerEscapePoint( 50, 0, 0 )

	local rGroup = core.system.groups.createGroup();
	local centerPoint = { -50, 0, 0 }
	local side = math.sqrt( 32 )
	for i = -side/2, side/2 do
		for p = -side/2, side/2 do
			--rioter( scen, p * 1.5 + centerPoint[1], 0  + centerPoint[2], i * 1.5  + centerPoint[3], rGroup, nil, nil, fists)
		end
	end
	core.system.groups.setGroupGoal(rGroup, 0, 0, -300)

	local sqads = {}	
	for i = 0, 10 do
		sqads[#sqads + 1] = squad(scen, 20, 0, 0, math.pi/2, fists);
	end
	

end
return scen
