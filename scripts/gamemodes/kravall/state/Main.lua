local MoveMarker = require "visual_objects/MoveMarker"
local PoliceSquadHandler = require "gamemodes/kravall/PoliceSquadHandler"
local OverviewHandler = require "gamemodes/kravall/OverviewHandler"
local KravallControl = require "gui/kravall/main/KravallControl"
local PDC = require "particle_definition"
local ASM = require "assembly_loader"

local ent = require "entities"
local squadInstance = ent.get "squadInstance"

local Main = { name = "Main" }
function Main:new(o)
    o = o or {}
    setmetatable( o, self )
    self.__index = self

    assert( o.asm, "you need to give Main an asm instance" )


    o.particleDefinitions =
    {
        TearGas = PDC:createParticleDefinition(o.asm, 5000, "assets/texture/particle/smoke.material")
    }


    --Init the things that are required.
    o.moveMarker = MoveMarker:new()

    o.gui = KravallControl:new( 
    {
        -- Called when the user is changing the formation from the gui.
        onFormationSelect = function( formation )
            o.policeHandler:setFormation( formation )
        end,
        -- Called when the user is changing stance from the gui.
        onStanceSelect = function( stance )
            o.policeHandler:setStance( stance )
        end,
        -- Called when the user is changing the current active ability in the gui.
        onAbilitySelect = function( ability )
            o.policeHandler:setAbility( ability )          
        end
    })

    o.policeHandler = PoliceSquadHandler:new( 
    {
        -- Called when the currently active formation is changed logically.
        onFormationChange = function( formation )
            o.gui:setFormation( formation ) 
        end,
        -- Called when the currently active stance is changed logically.
        onStanceChange = function( stance )
            o.gui:setStance( stance )
        end,

        onAbilityChange = function( ability )
            o.gui:setAbility( ability )
        end,

        -- Called whenever the selection changes
        -- might contain previously sent units.
        -- Could be empty
        onSelectedSquadsChange = function( squads )
            if squads == nil or #squads == 0 then
                o.gui.squadMenuGUI:setShow( false ) 
            else
                o.gui.squadMenuGUI:setShow( true ) 
            end
            o.gui:setSelectedSquads( squads )
        end,

        -- Called when the currently active unit changes
        -- or when the state of the unit might have changes (like health)
        onSelectedUnitInformationChange = function( data )
            o.gui:setUnitInformation( data ) 
        end,
        onMoveToPosition = function( squads, position, accept )
            if accept then
                o.moveMarker:playAccept(  position )
            else
                o.moveMarker:playDeny( position )
            end
        end,

        onUsableAbilitiesChange = function( abilities )
            o.gui:setUsableAbilities( abilities )
        end,
        onEventMessage = function( component )
            o.gui:addEvent(component)
        end,

        particleDefinitions = o.particleDefinitions,
    })	

    o.overviewHandler = OverviewHandler:new(
    {
        onEnterOverview = function()
            o:enterOverview()
        end,
        onExitOverview = function(pos)
            o:exitOverview(pos)
        end,
    })
	
	local policeTeam = 1
    if o.unitInstances then
        for _,v in  pairs( o.unitInstances ) do
            o.policeHandler:addSquad( squadInstance( o.asm, v, o.activeWeaponList, policeTeam ) )
        end
    end

    return o
end

function Main:update(delta)

    self.gui:update(delta)

    if self.overviewHandler.inOverview == false then
        self.policeHandler:update(delta)
    end
   
    self.overviewHandler:update(delta)
    self.moveMarker:update( delta )

end

function Main:enterOverview()
    local pos = {self.camera.position:get()}
    self.camera:addInterpolationPoint(core.glm.vec3.new(pos[1], 250, pos[3]), core.glm.quat.new(math.sin(math.pi/4), 0, 0, math.cos(math.pi/4)))
end

function Main:exitOverview(target)
    local pos = {target:get()}
    self.camera:addInterpolationPoint(core.glm.vec3.new(pos[1], pos[2] + 50, pos[3]), core.glm.quat.new(math.sin(math.pi/4), 0, 0, math.cos(math.pi/4)))
end

function Main:destroy()
    self.moveMarker:destroy()
    self.policeHandler:destroy()
    self.gui:destroy()
end

return Main
