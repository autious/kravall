local ent = require "entities"
local scenario = require "scenario"
local objective = require "objective" 
local scen = scenario.new()

-- SCRIPTS \/
scen.gamemode =  require "gamemodes/kravall":new()
scen:registerUpdateCallback( function(delta) scen.gamemode:update(delta) end )
scen:registerDestroyCallback( function() scen.gamemode:destroy() end )

local DONT_DIE_MSG = "Don't let the anarchists die" 
local ESCORT_MSG = "Escort atleast 5 anarchists to their home."
local objDontDie
local objLeadThrough

function printCount( ent )
--    print( core.system.area.getAreaRioterCount( ent ) ) 
    for _,ent in pairs( core.system.area.getAreaRioters( ent ) ) do
        scen.asm:destroyEntity( ent )
    end
end

function checkObjCount( ent )
    local count = core.system.area.getAreaRioterCount( ent )
    local alive_count = core.system.groups.getGroupMemberCount( 1 )

    if objDontDie ~= nil and objLeadThrough ~= nil then
        if alive_count < 5 then
            objDontDie.state = "fail"   
            objLeadThrough.state = "fail"
        end

        if count > 5 then
            objDontDie.state = "success"
            objLeadThrough.state = "success" 
        end

        objDontDie.title = DONT_DIE_MSG .. " " .. alive_count .. " still alive."
    end
end

scen:registerInitCallback( function()
    objDontDie = objective.new( DONT_DIE_MSG ) 
    objLeadThrough = objective.new( ESCORT_MSG )
    scen.gamemode.objectiveHandler:addObjective( objDontDie )
    scen.gamemode.objectiveHandler:addObjective( objLeadThrough )
end)

--DATA--
function scen:load()
    scen.asm:specific_content( core.contentmanager.load( 
            core.loaders.NavigationMeshLoader, "prototypeLevel.txt", function( value ) end, false ) )

    local ambient = ent.get "ambientLight"
    local directional = ent.get "directionalLight"
    local street_light = ent.get "streetLight"
    local street_light_intensity = 2.0


    -- DATA \/

    ambient(scen, 1.0, 1.0, 1.0, 0.1)
    directional(scen, -1, -1, 0.5)

    scen.gamemode.camera:lookAt( core.glm.vec3.new( -20, 35, 20 ), core.glm.vec3.new( 0, 0, 20 ) )
    print( "LOL" )

    -- Group 0 start to end, top row (left side)
    street_light(scen, -50, -0.5, street_light_intensity)
    street_light(scen, -40, -0.5, street_light_intensity)
    street_light(scen, -30, -0.5, street_light_intensity)
    street_light(scen, -20, -0.5, street_light_intensity)
    street_light(scen, -10.5, -0.5, street_light_intensity)
    street_light(scen, 1, -0.5, street_light_intensity)
    street_light(scen, 11, -0.5, street_light_intensity)
    street_light(scen, 21, -0.5, street_light_intensity)
    street_light(scen, 30.5, -2, street_light_intensity)
    street_light(scen, 40, -4, street_light_intensity)
    street_light(scen, 49.5, -4, street_light_intensity)
    street_light(scen, 59, -4, street_light_intensity)


    -- Group 0 start to end, bottom row (right side)
    street_light(scen, -50, 10, street_light_intensity)
    street_light(scen, -40, 10, street_light_intensity)
    street_light(scen, -30, 10, street_light_intensity)
    street_light(scen, -20, 10, street_light_intensity)
    street_light(scen, -10.5, 10, street_light_intensity)
    street_light(scen, 1, 10, street_light_intensity)
    street_light(scen, 11, 10, street_light_intensity)
    street_light(scen, 21, 10, street_light_intensity)
    street_light(scen, 30.5, 11.5, street_light_intensity)
    street_light(scen, 40, 13.5, street_light_intensity)
    street_light(scen, 49.5, 13.5, street_light_intensity)
    street_light(scen, 59, 13.5, street_light_intensity)

    -- Group 0 end light
    street_light(scen, 59, 4.75, street_light_intensity)

    -- Group 1 start to end, bottom part, left row
    street_light(scen, -25, 32, street_light_intensity)
    street_light(scen, -17, 32, street_light_intensity)
    street_light(scen, -10.5, 28, street_light_intensity)
    street_light(scen, -10.5, 19, street_light_intensity)

    -- Group 1 start to end, bottom part, right row
    street_light(scen, -25, 40.5, street_light_intensity)
    street_light(scen, -17, 40.5, street_light_intensity)
    street_light(scen, -8.5, 34.25, street_light_intensity)
    street_light(scen, 0, 28, street_light_intensity)
    street_light(scen, 1, 19, street_light_intensity)

    -- Group 1 start to end, top part, left row
    street_light(scen, -10.5, -12, street_light_intensity)
    street_light(scen, -10.5, -25, street_light_intensity)
    street_light(scen, -5.75, -34, street_light_intensity)
    street_light(scen, -1, -43, street_light_intensity)
    street_light(scen, 14, -46, street_light_intensity)
    street_light(scen, 28, -46, street_light_intensity)

    -- Group 1 start to end, top part, right row
    street_light(scen, 1, -12, street_light_intensity)
    street_light(scen, 1, -25, street_light_intensity)
    street_light(scen, 6, -34, street_light_intensity)
    street_light(scen, 14, -35, street_light_intensity)
    street_light(scen, 28, -35, street_light_intensity)

    local rioter = ent.get "rioter"
    local police = ent.get "police"
    local building = ent.get "building"
        
    -- Release	
    local groupOneGroupId = core.system.groups.createGroup()
    local centerPoint = { 49, 0, 5 }		
    for i = -7, 6 do
        for p = -6, 6 do
            rioter( scen, p * 1.5 + centerPoint[1], 0  + centerPoint[2], i * 1.5  + centerPoint[3], groupOneGroupId)
        end
    end
    core.system.groups.setGroupGoal(groupOneGroupId, -43, 0, 4)

    local groupTwoGroupId = core.system.groups.createGroup()
    local centerPoint = { 20.5, 0, -40 }		
    for i = -4, 4 do
        for p = -5, 5 do
            rioter( scen, p * 1.5 + centerPoint[1], 0  + centerPoint[2], i * 1.5  + centerPoint[3], groupTwoGroupId)
        end
    end
    core.system.groups.setGroupGoal(groupTwoGroupId, -21, 0, 36)

    -- Debug
    --local centerPoint = { 49, 0, 5 }		
    --for i = -2, 2 do
    --	for p = -2, 2 do
    --		rioter( scen, p * 1.5 + centerPoint[1], 0  + centerPoint[2], i * 1.5  + centerPoint[3], 0)
    --	end
    --end
    --core.nav_mesh.set_group_goal(0, -43, 0, 4)
    --
    --local centerPoint = { 20.5, 0, -40 }		
    --for i = -2, 2 do
    --	for p = -2, 2 do
    --		rioter( scen, p * 1.5 + centerPoint[1], 0  + centerPoint[2], i * 1.5  + centerPoint[3], 1)
    --	end
    --end
    --core.nav_mesh.set_group_goal(1, -21, 0, 36)

    local navmesh = ent.get "navMesh"
    navmesh(scen, 0, -0.1, 0)
    local plane = ent.get "plane"
    plane(scen, 0, -1, 0, 150)

    local navmesh = ent.get "navMesh"
    navmesh(scen, 0, -0.1, 0)
    local plane = ent.get "plane"
    plane(scen, 0, -1, 0)

    local lol_building = building(scen, 64, 12)
    building(scen, 64, 2)
    building(scen, 64, -8)
    building(scen, 55, -12)
    building(scen, 45, -11)
    building(scen, 30, -10)
    building(scen, 20, -8)
    building(scen, 10, -9)
    building(scen, 13, -23)
    building(scen, 23, -24)
    building(scen, 33, -23)
    building(scen, 36, -40)
    building(scen, 33, -53)
    building(scen, 21, -52)
    building(scen, 11, -52)
    building(scen, 1, -49)
    building(scen, -10, -50)
    building(scen, -15, -40)
    building(scen, -18, -27)
    building(scen, -17, -10)
    building(scen, -29, -8)
    building(scen, -39, -9)
    building(scen, -49, -9)
    building(scen, -57, -4)
    building(scen, -58, 6)
    building(scen, -56, 14)
    building(scen, -44, 16)
    building(scen, -33, 15)
    building(scen, -18, 15)
    building(scen, -24, 21)
    building(scen, -30, 27)
    building(scen, -33, 40)
    building(scen, -21, 45)
    building(scen, -12, 43)
    building(scen, -1, 38)
    building(scen, 6, 30)
    building(scen, 9, 15)
    building(scen, 18, 18)
    building(scen, 28, 17)
    building(scen, 36, 20)
    building(scen, 45, 19)
    building(scen, 56, 19)

    local area = ent.get "area"

    area( scen, {2,0,0}, { 5,-5, 5,5, -5,5, -5,-5 }, "test_area", nil, printCount )
    area( scen, {-21,0,36}, { 5,-5, 5,5, -5,5, -5,-5 }, "test_area", nil, checkObjCount )

    local GUI = require "gui/GUI"
    local Button = require "gui/component/Button"
    local Slider = require "gui/component/Slider"
    local Checkbox = require "gui/component/Checkbox"

    local CenterPlacer = require "gui/placement/CenterPlacer"
    local LeftPlacer = require "gui/placement/LeftPlacer"

    local gui = GUI:new()

    scen.lastCreated = rioter( scen, 25, 0, 0, groupOneGroupId)
    local button = Button:new({x=1000,y=100})
    function button.onClick()
            scen.lastCreated = rioter( scen, 25, 0, 0, groupOneGroupId)
    end

    local slider = Slider:new({x=1000,y=300}) 
    function slider.onChange( value )
        local s = 1+value*10
        scen.lastCreated:set( core.componentType.ScaleComponent, {scale = {s,s,s}}, true )
    end

    local checkbox = Checkbox:new({x=100,y=400,checked=true})
    function checkbox.onChange( value )
        --core.config.debugRenderAreas = value
    end

    gui:addComponent(button)
    gui:addComponent(slider)
    gui:addComponent(checkbox)

    gui:addPlacementHandler( LeftPlacer:new() )

    scen.gui = gui
    scen:registerDestroyCallback( function() scen.gui:destroy() end )
end

return scen;
