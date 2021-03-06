local input = require "input" 
local MainMenu = require "gui/MainMenu"
local SetupMenu = require "gui/SetupMenu"
local ScenarioMenu = require "gui/ScenarioMenu"
local SettingsMenu = require "gui/SettingsMenu"
local CreditsMenu = require "gui/CreditsMenu"
local TutorialMenu = require "gui/TutorialMenu"
local PDC = require "particle_definition"

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
		--NO FUCKING KEYBINDS HURR DURR RAGE
        --if key == core.input.keyboard.key.A and action == core.input.action.Release then
        --    menuState.goMain()
        --end  
		--
        --if key == core.input.keyboard.key.B and action == core.input.action.Release then
        --    menuState.goScenario()
        --end  
		--
        --if key == core.input.keyboard.key.C and action == core.input.action.Release then
        --    menuState.goSetup()
        --end  
		--
        --if key == core.input.keyboard.key.D and action == core.input.action.Release then
        --    menuState.goCredits()
        --end  
		--
        --if key == core.input.keyboard.key.E and action == core.input.action.Release then
        --    menuState.goSettings()
        --end  
    end
	local fists
	
	
	
    local function init()
		--print ("Hej")
		
	--	core.contentmanager.load(core.loaders.MaterialLoader, "assets/texture/particle/smoke.material", 
    --    function(value)        
    --        scen.smoke = core.system.particle.createParticleDefinition(200, value)
    --    end, false)		
	--	
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
		entity:addComponent( core.componentType.AnimationComponent)
		core.animations.loop(entity, "idle-defense")
		core.animations.setTime(entity, math.random(1, 200) / 200)
	end
	
	
	local hovercraftPos

	
	T.initHovercraft = function( entity )
		hovercraftPos = entity:get(core.componentType.WorldPositionComponent).position
	--	
	--	--print ("bye")
	--	--
	--	local particleEntity = core.entity.create(core.componentType.EmitterComponent
	--								, core.componentType.WorldPositionComponent)
	--								--, core.componentType.MovementComponent
	--								--, core.componentType.MovementDataComponent
	--								--, core.componentType.RotationComponent
	--								--, core.componentType.UnitTypeComponent
	--								--, core.componentType.AttributeComponent
	--								--, core.componentType.FlowfieldComponent
	--								
	--	particleEntity:set(core.componentType.WorldPositionComponent, { position = {0, 0, 0}})--hovercraftPos })
	--	particleEntity:set(core.componentType.EmitterComponent, {
	--		rate = 100,
	--		offset = {0, -2, 0},
	--		life = 3,
	--		lifeVariance = 0.5,
	--		lifeReduction = 1.5,
	--		lifeReductionVariance = 0,
	--		velocity = {0, 0, 3},
	--		velocityVariance = {0, 0, 4},
	--		acceleration = {0, 2, 0},
	--		coneDirection = {0, 1, 0},
	--		coneAngle = 60,
	--		coneAngleVariance = 30,
	--		type = core.system.particle.emitters.Cone,
	--		handle = scen.smoke
	--		}, true)
	end
	
	
	
	local blueLightRotation1 = math.pi
	local blueLightRotation2 = math.pi
	local redLightRotation1 = 0
	local redLightRotation2 = 0
	
	local rotSpeed = 10.0
	
	T.updateRedLightFront = function ( entity, delta )
		redLightRotation1 = redLightRotation1 + rotSpeed * delta
		if redLightRotation1 > math.pi * 2 then
			redLightRotation1 = 0.0
		end
		local rot = { math.cos(redLightRotation1), 0, math.sin(redLightRotation1)}
		entity:set( core.componentType.RotationComponent, {rotation=rot} )
	end
	
	T.updateRedLightBack = function ( entity, delta )
		redLightRotation2 = redLightRotation2 + rotSpeed * delta
		if redLightRotation2 > math.pi * 2 then
			redLightRotation2 = 0.0
		end
		local rot = { math.cos(redLightRotation2), 0, math.sin(redLightRotation2)}
		entity:set( core.componentType.RotationComponent, {rotation=rot} )
	end
	
	T.updateBlueLightFront = function ( entity, delta )
		blueLightRotation1 = blueLightRotation1 + rotSpeed * delta
		if blueLightRotation1 > math.pi * 2 then
			blueLightRotation1 = 0.0
		end
		local rot = { math.cos(blueLightRotation1), 0, math.sin(blueLightRotation1)}
		entity:set( core.componentType.RotationComponent, {rotation=rot} )
	end

	T.updateBlueLightBack = function ( entity, delta )
		blueLightRotation2 = blueLightRotation2 + rotSpeed * delta
		
		if blueLightRotation2 > math.pi * 2 then
			blueLightRotation2 = 0.0
		end
		local rot = { math.cos(blueLightRotation2), 0, math.sin(blueLightRotation2)}
		entity:set( core.componentType.RotationComponent, {rotation=rot} )
	end
	
	
	local videoBaseIntensity = 1
	T.initVideoBlinkLight = function ( entity, delta )
		videoBaseIntensity = entity:get(core.componentType.LightComponent).intensity
	end
	
	local arrowBaseIntensity
	T.initArrowBlinkLight = function ( entity, delta )
		arrowBaseIntensity = entity:get(core.componentType.LightComponent).intensity
	end
	
	local signOn
    local signOff

    local signOnMaterial
    local signOffMaterial

	
    scen:registerInitCallback( function()
        signOn = core.contentmanager.load(core.loaders.MaterialLoader, "assets/texture/static/props/neon_signs.material", 
        function(handle)
            signOnMaterial = handle
        end,
        false)

        signOff = core.contentmanager.load(core.loaders.MaterialLoader, "assets/texture/static/props/neon_signs_broken.material", 
        function(handle)
            signOffMaterial = handle
        end,
        false)
    end)

    scen:registerDestroyCallback( function()
        signOn:free()
        signOff:free()
    end)

	local videoTick = 0
	T.videoBlinkSign = function ( entity, delta )
		videoTick = videoTick + delta
		math.randomseed(videoTick)
		local flicker = math.random()*(0.5+0.5*math.sin(2*math.sin(videoTick)))
		if  flicker > 0.95 then
			flicker = 1
		elseif  flicker > 0.9 then
			flicker = 0.1
		else
			flicker = 0
		end
		local gc;
		gc = entity:get(core.componentType.GraphicsComponent)
		
		if flicker == 1 then
            gc.material = signOnMaterial
		else
            gc.material = signOffMaterial
		end
		
		entity:set(core.componentType.GraphicsComponent, gc)
	end
	
	T.videoBlinkLight = function ( entity, delta )
		math.randomseed(videoTick)
		local flicker = math.random()*(0.5+0.5*math.sin(2*math.sin(videoTick)))
		if  flicker > 0.95 then
			flicker = 1
		elseif  flicker > 0.9 then
			flicker = 0.1
		else
			flicker = 0
		end
		local lc;
		lc = entity:get(core.componentType.LightComponent)
		lc.intensity = videoBaseIntensity * flicker;
		entity:set(core.componentType.LightComponent, lc)
	end
	
	local arrowTick = 0
	T.arrowBlinkSign = function ( entity, delta )
		arrowTick = arrowTick + delta
		math.randomseed(arrowTick)
		local flicker = (0.5+0.5*math.sin(arrowTick*2))
		if  flicker > 0.5 then
			flicker = 0
		else
			flicker = 1
		end
		local gc;
		gc = entity:get(core.componentType.GraphicsComponent)
		
		if flicker == 1 then
            gc.material = signOnMaterial
		else
            gc.material = signOffMaterial
		end
		
		entity:set(core.componentType.GraphicsComponent, gc)
	end
	
	T.arrowBlinkLight = function ( entity, delta )
		math.randomseed(arrowTick)
		local flicker = (0.5+0.5*math.sin(arrowTick*2))
		if  flicker > 0.5 then
			flicker = 0
		else
			flicker = 1
		end
		
		local lc;
		lc = entity:get(core.componentType.LightComponent)
		lc.intensity = arrowBaseIntensity * flicker;
		entity:set(core.componentType.LightComponent, lc)
	end
	
	
	
	
	
	local HCtval = 0.0
	T.updateHovercraft = function( entity, delta )
		
		HCtval = HCtval + delta * 0.5
		
		
		local pos = { hovercraftPos[1], (hovercraftPos[2] + math.sin(HCtval)), hovercraftPos[3]}
		local angle = 3.5 + 0.2*math.cos(HCtval)+ 0.1*math.cos(HCtval*1.2)
		
		local shenanigans = 0.5*(math.pi+0.1*math.sin(HCtval))
		local axis = {0, math.sin(shenanigans), math.cos(shenanigans)}
		
		local rot = { axis[1]*math.sin(angle/2), axis[2]*math.sin(angle/2), axis[3]*math.sin(angle/2), math.cos(angle/2)}

		entity:set( core.componentType.WorldPositionComponent, {position=pos} )
		entity:set( core.componentType.RotationComponent, {rotation=rot} )
		
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
		grp = T.createDeserter( e, 300,2 )
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
		
        local x,y,z = unpack(startPosition)
        local newPosition = {x,y,(math.random(1,100)/100.0-0.5)*10+z}
		local rioters = core.system.area.getAreaRioters( entity )
		
		for i,v in  pairs( rioters ) do
			v:set( core.componentType.WorldPositionComponent, {position=newPosition} )
			v:set( core.componentType.FlowfieldComponent, {node=-1} )
		end
	end
	
	
    return T
end
