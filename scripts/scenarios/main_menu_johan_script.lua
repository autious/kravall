local input = require "input" 
local MainMenu = require "gui/MainMenu"
local SetupMenu = require "gui/SetupMenu"
local ScenarioMenu = require "gui/ScenarioMenu"
local SettingsMenu = require "gui/SettingsMenu"
local CreditsMenu = require "gui/CreditsMenu"
local TutorialMenu = require "gui/TutorialMenu"

local entity = require "entities"
local group = entity.get "group"

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
        scen.gamemode.camera:setGoal( scen.cameras.settings.view, MenuScrollSpeed )

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
		group( scen, ac.vertices, grp, {xsize, ysize}, fists, {0.9,0.3,0,1}, 1, 1,core.RioterAlignment.Pacifist )
        return grp
	end
	
	local endPosition
	local startPosition
	local grp
	T.registerStart = function( entity )
		--print( "REGGING START" )
		startPosition = entity:get( core.componentType.WorldPositionComponent ).position
		
		grp = T.createDeserter( entity, 20,20 )
	end
	
	T.registerEnd = function( entity )
		--print( "REGGING END" )
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
		end
	end
	
	
    return T
end
