local input = require "input" 
local MainMenu = require "gui/MainMenu"
local SetupMenu = require "gui/SetupMenu"
local ScenarioMenu = require "gui/ScenarioMenu"
local SettingsMenu = require "gui/SettingsMenu"
local CreditsMenu = require "gui/CreditsMenu"
local TutorialMenu = require "gui/TutorialMenu"

local entity = require "entities"
local group = entity.get "group"
local rioter = entity.get "rioter"

local MenuScrollSpeed = 500

return function( scen )
    local menuState = {}
    local scenarios = {}
    
    for _,v in pairs( require "scenario_list" ) do
        local new_scen = {}
        local s  = dofile( "scripts/scenarios/" .. v .. ".lua" )

        new_scen.filename = v
        new_scen.name = s.name or ""
        new_scen.description = s.description or ""

        scenarios[#scenarios+1] = new_scen
    end

    menuState.selectedScenario = nil 

    function menuState.goMain()
        scen.gamemode.camera:setGoal( scen.cameras.main.view, MenuScrollSpeed )

        if scen.gui ~= nil then
            scen.gui:destroy()
            scen.gui = nil
        end
        scen.gui = MainMenu:new({},menuState)
    end

    function menuState.goScenario()
        scen.gamemode.camera:setGoal( scen.cameras.scen.view, MenuScrollSpeed )

        if scen.gui ~= nil then
            scen.gui:destroy()
            scen.gui = nil
        end
        scen.gui = ScenarioMenu:new({scenarios = scenarios},menuState)
    end

    function menuState.goSetup()
        scen.gamemode.camera:setGoal( scen.cameras.setup.view, MenuScrollSpeed )

        if scen.gui ~= nil then
            scen.gui:destroy()
            scen.gui = nil
        end
        scen.gui = SetupMenu:new({},menuState)
    end

    function menuState.goCredits()
        scen.gamemode.camera:setGoal( scen.cameras.credits.view, MenuScrollSpeed )

        if scen.gui ~= nil then
            scen.gui:destroy()
            scen.gui = nil
        end
        scen.gui = CreditsMenu:new({},menuState)
    end

    function menuState.goSettings()
        scen.gamemode.camera:setGoal( scen.cameras.settings.view, MenuScrollSpeed )

        if scen.gui ~= nil then
            scen.gui:destroy()
            scen.gui = nil
        end
        scen.gui = SettingsMenu:new({},menuState)
    end

	 function menuState.goTutorial()
        scen.gamemode.camera:setGoal( scen.cameras.tutorial.view, MenuScrollSpeed )

        if scen.gui ~= nil then
            scen.gui:destroy()
            scen.gui = nil
        end
        scen.gui = TutorialMenu:new({scen = scen},menuState)
    end

    local function onKey( key, scancode, action )
        if key == core.input.keyboard.key.A and action == core.input.action.Release then
            menuState.goMain()
        end  

        if key == core.input.keyboard.key.B and action == core.input.action.Release then
            menuState.goScenario()
        end  

        if key == core.input.keyboard.key.C and action == core.input.action.Release then
            menuState.goSetup()
        end  

        if key == core.input.keyboard.key.D and action == core.input.action.Release then
            menuState.goCredits()
        end  

        if key == core.input.keyboard.key.E and action == core.input.action.Release then
            menuState.goSettings()
        end  
    end
	local fists
	
    local function init()
		fists = core.weaponData.pushWeapon( 1.0, 0.75, 10, 0.05, 0.01, 3.2, 2.9, 0.05, 0.5, "punch" )
        scen.gamemode.camera:setView( scen.cameras.main.view ) 
        input.registerOnKey( onKey )
        menuState.goMain()
    end

    local function destroy()
        input.deregisterOnKey( onKey )
        
        if scen.gui ~= nil then
            scen.gui:destroy()
            scen.gui = nil
        end
    end

    scen:registerInitCallback( init ) 
    scen:registerDestroyCallback( destroy )

    scen.gamemode = require "gamemodes/menu":new()
    scen:registerUpdateCallback( function(delta) scen.gamemode:update(delta) end )
    scen:registerDestroyCallback( function() scen.gamemode:destroy() end )

    local T = {}
	
	T.initPolice = function( entity )
		--print( "SNUTEN KOMMER!!!!!" )

		entity:addComponent( core.componentType.AnimationComponent)
		core.animations.loop(entity, "idle")
		--local theTime = math.random() * 10.0
		core.animations.setTime(entity, math.random(1, 200) / 200)
		
	end
	
	
	local hovercraftPos
	local hovercraftPosY
	--local hovercraftRot
	
	T.initHovercraft = function( entity )
		hovercraftPosY = hovercraftPosY or 0.0
		hovercraftPos = entity:get(core.componentType.WorldPositionComponent).position
		--hovercraftRot = entity:get(core.componentType.RotationComponent).rotation
	end
	
	local HCtval = 0.0
	T.updateHovercraft = function( entity, delta )
		
		HCtval = HCtval + delta * 0.5
		
		
		--print ( "I belive I can fly!!!!!!" )
		
		local pos = { hovercraftPos[1], (hovercraftPos[2] + math.sin(HCtval)), hovercraftPos[3]}
		--local rot = { hovercraftRot[1], (hovercraftRot[2] + math.sin(HCtval)), hovercraftRot[3], hovercraftRot[4]}
		local angle = 3.5 + 0.2*math.cos(HCtval)+ 0.1*math.cos(HCtval*1.2)
		
		local shenanigans = 0.5*(math.pi+0.1*math.sin(HCtval))
		local axis = {0, math.sin(shenanigans), math.cos(shenanigans)}
		
		local rot = { axis[1]*math.sin(angle/2), axis[2]*math.sin(angle/2), axis[3]*math.sin(angle/2), math.cos(angle/2)}
		--local rot 
		entity:set( core.componentType.WorldPositionComponent, {position=pos} )
		entity:set(core.componentType.RotationComponent, {rotation=rot})
		
	end
	
	
	function T.createDeserter( ent, xsize, ysize )
		local wpc = ent:get( core.componentType.WorldPositionComponent )
		local ac = ent:get( core.componentType.AreaComponent )
		verts = ac.vertices
    
		-- Make vertex positions from local space to world space
		for i = 1, 8, 2 do
			verts[i] = verts[i] + wpc.position[1]
			verts[i + 1] = verts[i + 1] + wpc.position[3]
		end
	    local grp = core.system.groups.createGroup(1)
		group( scen, ac.vertices, grp, {xsize, ysize}, fists, nil, 1, 1,core.RioterAlignment.Anarchists )
        return grp
	end
	
	local endPosition
	local startPosition
	local grp
    
   T.spawn = function(e)
		grp = T.createDeserter( e, 100,2 )
   end
    
	T.registerStart = function( entity )
		print( "REGGING START" )
		startPosition = entity:get( core.componentType.WorldPositionComponent ).position
		
	end
	
	T.registerEnd = function( entity )
		print( "REGGING END" )
		endPosition = entity:get( core.componentType.WorldPositionComponent ).position
	end
	
	T.checkEndEnter = function( entity )
		if grp and endPosition then 
			--print "SETTING GOAL" 
			core.system.groups.setGroupGoal( grp, unpack( endPosition ) )
		end
		
		local rioters = core.system.area.getAreaRioters( entity )
		
		for i,v in  pairs( rioters ) do
			v:set( core.componentType.WorldPositionComponent, {position=startPosition} )
			v:set( core.componentType.FlowfieldComponent, {node=-1} )
		end
	end
	
	
    return T
end
