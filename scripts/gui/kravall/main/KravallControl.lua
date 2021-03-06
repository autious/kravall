local GUI = require "gui/GUI"
local Button = require "gui/component/Button"
local Checkbox = require "gui/component/Checkbox"
--local TextLabel = require "gui/component/TextLabel"
local Image = require "gui/component/Image"

local AnchorPlacer = require "gui/placement/AnchorPlacer"
local CenterPlacer = require "gui/placement/CenterPlacer"
local SimplePlacer = require "gui/placement/SimplePlacer"

local EventListerGUI = require "gui/kravall/main/subgui/EventListerGUI"
local UnitStatGUI = require "gui/kravall/main/subgui/UnitStatGUI"
local SquadMenuGUI = require "gui/kravall/main/subgui/SquadMenuGUI"

local toolTipHandler = require "gui/tooltip/ToolTipHandler"
local createTT = require "gui/tooltip/ToolTip"

local KravallControl = 
{ 
    onFormationSelect = function(formation) core.log.error("No handler set for onFormationChange in KravallControl.") end,
    onStanceSelect = function(stance) core.log.error("No handler set for onStanceChange in KravallControl.") end,
    onAbilitySelect = function(ability) end,
    onUseOverview = function() core.log.error("No function set for onUseOverview in KravallControl.") end,
    onCycleSquads = function() core.log.error("No function set for onCycleSquads in KravallControl.") end,
    onRequestPause = function() core.log.error("No function set for onRequestPause in KravallControl.") end,
}
                            
function KravallControl:new(o)
    o = o or {}

    setmetatable( o, self )
    self.__index = self

    o.gui = GUI:new()
    
	o.squadMenuGUI = SquadMenuGUI:new( 
									{ 
										width=57, height=553, anchor="NorthEast",
										onStanceSelect = function(stance) o.onStanceSelect( stance ) end,
										onAbilitySelect = function(ability) print( "select ability" ); o.onAbilitySelect( ability ) end,
										onFormationSelect = function(form) o.onFormationSelect(form) end
									} )
	o.gui:addComponent( o.squadMenuGUI )
	o.squadMenuGUI:setShow( false )
	
    ------------------
    o.statusGUI = UnitStatGUI:new( { show = false, width=200, height=110 } )
	o.statusGUI:setShow( false )
	
	o.gui:addComponent( o.statusGUI )
	------------------
	local eventWidth = 237
	local globalWidth = 60
	local leftCornerHeight = 159
	o.leftCornerGUI = GUI:new( { width=eventWidth+globalWidth, height=leftCornerHeight, anchor="SouthWest" } )
	
	o.leftCornerGUI:addPlacementHandler( SimplePlacer )
	o.gui:addComponent( o.leftCornerGUI )
	------------------
	o.globalGUI = GUI:new( 
						{
							width=globalWidth, height=leftCornerHeight
						} )

    o.overviewButton = Checkbox:new(
                                {
                                    checked = false,
                                    xoffset=10,
                                    yoffset=19,
                                    matOpen="assets/texture/ui/overveiw-unselected_00.material",
                                    matSelected="assets/texture/ui/overveiw-selected_00.material",
                                    matHoverOpen="assets/texture/ui/overveiw-hover_00.material",
                                    matHoverSelected="assets/texture/ui/overveiw-hover_00.material",
                                    onClick=o.onUseOverview,
                                    toolTip=createTT:new( { handler=toolTipHandler, text="Go to overview mode - G" } )
                                })
	o.globalGUI:addComponent( o.overviewButton ) 
										
	o.globalGUI:addComponent( Button:new (
										{ 
											xoffset=10,
											yoffset=66,
											matReleased="assets/texture/ui/circle-squads-unselected_00.material",
											matPressed="assets/texture/ui/circle-squads-selected_00.material",
											matHover="assets/texture/ui/circle-squads-hover_00.material",
											onClick=o.onCycleSquads,
											toolTip=createTT:new( { handler=toolTipHandler, text="Cycle through the police squads - X" } )
										} ) )

	o.globalGUI:addComponent( Button:new (
										{ 
											xoffset=10,
											yoffset=111,
											matReleased="assets/texture/ui/leave-unselected_00.material",
											matPressed="assets/texture/ui/leave-selected_00.material",
											matHover="assets/texture/ui/leave-hover_00.material",
											onClick=o.onRequestPause,
											toolTip=createTT:new( { handler=toolTipHandler, text="Quit menu - ESC" } )
										} ) )
	
	o.globalGUI:addComponent( Image:new( { mat="assets/texture/ui/non-contextual-abilities_00.material" } ) )
	
	o.globalGUI:addPlacementHandler( SimplePlacer )
	o.leftCornerGUI:addComponent( o.globalGUI )
	------------------
    o.eventGUIPadder = GUI:new( {width=eventWidth, height=leftCornerHeight, xoffset=globalWidth} )
	local xOff = 5
    o.eventGUI = EventListerGUI:new( {x=0,y=0, xoffset=xOff, yoffset=12, width=globalWidth-eventWidth-xOff, height=leftCornerHeight - 18 } )
    o.eventGUIPadder:addComponent( o.eventGUI )
	o.eventGUIPadder:addComponent( Image:new( { mat="assets/texture/ui/event-bar.material" } ) )
	o.eventGUIPadder:addComponent( Image:new( { mat="assets/texture/ui/event-box-background.material" } ) )
	
	o.eventGUIPadder:addPlacementHandler( SimplePlacer )
	o.leftCornerGUI:addComponent( o.eventGUIPadder )
    ------------------
	--o.overviewGUI = OverviewGUI:new( {        
    --    onUseOverview = function() o.onUseOverview() end
    --})
	
    o.gui:addPlacementHandler( AnchorPlacer )

    return o
end

function KravallControl:setFormation( formation )
    --self.formationGUI:setFormation( formation )
	self.squadMenuGUI:setFormation( formation )
end

function KravallControl:setStance( stance )
    --self.stanceGUI:setStance( stance )
	self.squadMenuGUI:setStance( stance )
end

function KravallControl:setAbility( ability )
    --self.abilitiesGUI:setAbility( ability ) 
	self.squadMenuGUI:setAbility( ability ) 
end

function KravallControl:setOverview( state )
    self.overviewButton:setChecked(state) 
end

function KravallControl:setUnitInformation( data )
    self.statusGUI:setFormation( data.formation )
    self.statusGUI:setStance( data.stance )
    self.statusGUI:setName( data.name )
    self.statusGUI:setHealth( data.health )
    self.statusGUI:setMorale( data.morale )
    self.statusGUI:setStamina( data.stamina )
end

function KravallControl:setSelectedSquads( squads )
    if squads and #squads > 0 then
        self.statusGUI:setShow( true )
    else
        self.statusGUI:setShow( false )
    end
    --TODO: show/hide the status field depending on selection.
end

function KravallControl:setUsableAbilities(abilities)
    --self.abilitiesGUI:setUsableAbilities(abilities)
	self.squadMenuGUI:setUsableAbilities(abilities)
end



function KravallControl:addEvent( component )
    self.eventGUI:addComponent( component )
end

function KravallControl:init()
end

function KravallControl:update( delta )
    -- For debug writing
    self.gui:renderDebug( delta )

    --self.count = self.count or 0
    --self.count = self.count + delta
    --
    --if self.count > 2 then
    --    self.ind = self.ind or 0
    --    self.ind = self.ind + 1
    --    
    --    --if self.ind % 2 == 1 then
    --        self:addEvent(TextLabel:new( { label="Status: " .. self.ind, font="assets/font/toolTip.font" } ))
    --    --else
    --    --    self:addEvent(Button:new( {onClick=function() print( "JOEL" ) end }))
    --    --end
    --    self.count = 0
    --end

    -- constrict each frame to give animation like results.
    self.eventGUI:constrict( delta )
end

function KravallControl:setShow( flag )
    self.gui:setShow( flag ) 
end

function KravallControl:destroy()
    self.gui:destroy() 
end

return KravallControl
