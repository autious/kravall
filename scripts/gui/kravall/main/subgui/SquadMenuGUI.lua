local GUI = require "gui/GUI"
local SimplePlacer = require "gui/placement/SimplePlacer"
local Checkbox = require "gui/component/Checkbox"
local Image = require "gui/component/Image"
local toolTipHandler = require "gui/tooltip/ToolTipHandler"
local createTT = require "gui/tooltip/ToolTip"

local xoffset=10

local SquadMenuGUI = GUI:new{
                            width=57,
                            height=553,
                            anchor="Center",
                            onStanceSelect=function( value ) end, 
							onAbilitySelect=function( ability ) print( ability ) end,
							onFormationSelect = function(formation) core.log.error("No handler set for formation callback") end
                        }

local s_aggressive = core.PoliceStance.Aggressive
local s_defensive = core.PoliceStance.Defensive
local s_passive = core.PoliceStance.Passive
local s_ability = core.system.squad.abilities

local f_circle      = core.system.squad.formations.CircleFormation
local f_line        = core.system.squad.formations.LineFormation
local f_halfcircle  = core.system.squad.formations.HalfCircleFormation
local f_no          = core.system.squad.formations.NoFormation

function SquadMenuGUI:new(o)
    o = GUI.new(self,o)    
	
    o.stances = {}
    o.stances[s_aggressive] = Checkbox:new( 
										{
											doStateSwitchOnPress=false,
											checked=false,
											xoffset=xoffset,
											yoffset=14,
											matOpen="assets/texture/ui/aggressive-unselected_00.material",
											matSelected="assets/texture/ui/aggressive-selected_00.material",
											matHoverOpen="assets/texture/ui/aggressive-hover_00.material",
											matHoverSelected="assets/texture/ui/aggressive-extra_00.material",
											onClick=function( self, value ) o.onStanceSelect( s_aggressive ) end,
											toolTip=createTT:new( { handler=toolTipHandler, text="Aggressive Stance - I" } )
										} )
	o.stances[s_defensive] = Checkbox:new( 
										{
											doStateSwitchOnPress=false,
											checked=false,
											xoffset=xoffset,
											yoffset=62,
											matOpen="assets/texture/ui/defensive-unselected_00.material",
											matSelected="assets/texture/ui/defensive-selected_00.material",
											matHoverOpen="assets/texture/ui/defensive-hover_00.material",
											matHoverSelected="assets/texture/ui/defensive-extra_00.material",
											onClick=function( self, value ) o.onStanceSelect( s_defensive ) end,
											toolTip=createTT:new( { handler=toolTipHandler, text="Defensive Stance - O" } )
										} )
	o.stances[s_passive] = Checkbox:new( 
										{
											doStateSwitchOnPress=false,
											checked=false,
											xoffset=xoffset,
											yoffset=109,
											matOpen="assets/texture/ui/passive-unselected_00.material",
											matSelected="assets/texture/ui/passive-selected_00.material",
											matHoverOpen="assets/texture/ui/passive-hover_00.material",
											matHoverSelected="assets/texture/ui/passive-extra_00.material",
											onClick=function( self, value ) o.onStanceSelect( s_passive ) end,
											toolTip=createTT:new( { handler=toolTipHandler, text="Passive Stance - P" } )
										} )
    o:addComponent( o.stances[s_aggressive] )									
    o:addComponent( o.stances[s_defensive] )
	o:addComponent( o.stances[s_passive] )
	
	o.abilities = {}

	o.abilities[s_ability.Attack] = Checkbox:new(
												{  
													doStateSwitchOnPress=false,
													checked=false,
													xoffset=xoffset-1,
													yoffset=188,
													matOpen="assets/texture/ui/attack-unselected_00.material",
													matSelected="assets/texture/ui/attack-selected_00.material",
													matHoverOpen="assets/texture/ui/attack-hover_00.material",
													matHoverSelected="assets/texture/ui/attack-extra_00.material",
													onHover=onHoverAbility,
													onClick= function(self) o.onAbilitySelect( s_ability.Attack ) end,
													onExit=onExit,
													toolTip=createTT:new( { handler=toolTipHandler, text="Attack - 1" } )
												} )
    o.abilities[s_ability.TearGas] = Checkbox:new( 
												{  
													doStateSwitchOnPress=false,
													checked=false,
													xoffset=xoffset-1,
													yoffset=236,
													matOpen="assets/texture/ui/tear-gas-unselected_00.material",
													matSelected="assets/texture/ui/tear-gas-selected_00.material",
													matHoverOpen="assets/texture/ui/tear-gas-hover_00.material",
													matHoverSelected="assets/texture/ui/tear-gas-extra_00.material",
													onHover=onHoverAbility,
													onClick= function(self) o.onAbilitySelect( s_ability.TearGas ) end,
													onExit=onExit,
													toolTip=createTT:new( { handler=toolTipHandler, text="Tear Gas - 2" } )
												} )
    o.abilities[s_ability.Sprint] = Checkbox:new( 
												{  
													doStateSwitchOnPress=false,
													checked=false,
													xoffset=xoffset-1,
													yoffset=281,
													matOpen="assets/texture/ui/power-unselected_00.material",
													matSelected="assets/texture/ui/power-selected_00.material",
													matHoverOpen="assets/texture/ui/power-hover_00.material",
													matHoverSelected="assets/texture/ui/power-extra_00.material",
													onHover=onHoverAbility,
													onClick= function(self) o.onAbilitySelect( s_ability.Sprint ) end,
													onExit=onExit,
													toolTip=createTT:new( { handler=toolTipHandler, text="Sprint - 3" } )
												} )
    o.abilities[s_ability.Halt] = Checkbox:new( 
												{  
													doStateSwitchOnPress=false,
													checked=false,
													xoffset=xoffset-1,
													yoffset=330,
													matOpen="assets/texture/ui/grenade-unselected_00.material",
													matSelected="assets/texture/ui/grenade-selected_00.material",
													matHoverOpen="assets/texture/ui/grenade-hover_00.material",
													matHoverSelected="assets/texture/ui/grenade-extra_00.material",
													onHover=onHoverAbility,
													onClick= function(self) o.onAbilitySelect( s_ability.Halt ) end,
													onExit=onExit,
													toolTip=createTT:new( { handler=toolTipHandler, text="Halt - 4" } )
												} )
						
	o:addComponent( o.abilities[s_ability.Attack] )									
    o:addComponent( o.abilities[s_ability.TearGas] )
	o:addComponent( o.abilities[s_ability.Sprint] )
	o:addComponent( o.abilities[s_ability.Halt] )
	
	o.formations = {}
    o.formations[f_line]= Checkbox:new( 
									{  
										doStateSwitchOnPress=false,
										checked=false,
										xoffset=xoffset,
										yoffset=404,
										matOpen="assets/texture/ui/form-line-unselected_00.material",
										matSelected="assets/texture/ui/form-line-selected_00.material",
										matHoverOpen="assets/texture/ui/form-line-hover_00.material",
										matHoverSelected="assets/texture/ui/form-line-extra_00.material",
										onClick=function(self,value) if value then o.onFormationSelect( f_line ) else o.onFormationSelect( f_no ) end end,
										toolTip=createTT:new( { handler=toolTipHandler, text="Line Formation - L" } )
									} )
    o.formations[f_circle] = Checkbox:new(
									{  
										doStateSwitchOnPress=false,
										checked=false,
										xoffset=xoffset,
										yoffset=453,
										matOpen="assets/texture/ui/form-circle-unselected_00.material",
										matSelected="assets/texture/ui/form-circle-selected_00.material",
										matHoverOpen="assets/texture/ui/form-circle-hover_00.material",
										matHoverSelected="assets/texture/ui/form-circle-extra_00.material",
										onClick=function(self,value) if value then o.onFormationSelect( f_circle ) else o.onFormationSelect( f_no ) end end,
										toolTip=createTT:new( { handler=toolTipHandler, text="Circle Formation - C" } )
									} )
    o.formations[f_halfcircle] = Checkbox:new(
									{  
										doStateSwitchOnPress=false,
										checked=false,
										xoffset=xoffset,
										yoffset=500,
										matOpen="assets/texture/ui/form-halfcircle-unselected_00.material",
										matSelected="assets/texture/ui/form-halfcircle-selected_00.material",
										matHoverOpen="assets/texture/ui/form-halfcircle-hover_00.material",
										matHoverSelected="assets/texture/ui/form-halfcircle-extra_00.material",
										onClick=function(self,value) if value then o.onFormationSelect( f_halfcircle ) else o.onFormationSelect( f_no ) end end,
										toolTip=createTT:new( { handler=toolTipHandler, text="Semi-Circle Formation - H" } )
									} )
						
	o:addComponent( o.formations[f_halfcircle] )
    o:addComponent( o.formations[f_circle] )
    o:addComponent( o.formations[f_line] )
						
	o:addComponent( Image:new
							{ 
								ignoreConstrict=true,
								mat="assets/texture/ui/contextual_abilities_00.material"
							} )
	
    o:addPlacementHandler( SimplePlacer )

    return o
end

function SquadMenuGUI:setStance( stance )
    for c_stance,v in pairs( self.stances ) do
        if c_stance == stance then
            v:setChecked( true )  
        else
            v:setChecked( false )  
        end
    end
end

function SquadMenuGUI:setAbility( ability )
    self.currentActiveAbility = ability
    for i,v in pairs( self.abilities ) do
        if i == ability then
            v:setChecked( true )
        else
            v:setChecked( false )
        end 
    end
end

function SquadMenuGUI:setUsableAbilities(abilities)
    for _,v in pairs(self.abilities) do
        v:setShow(false)
    end

    for _,a in pairs(abilities) do
        for k,v in pairs(self.abilities) do
            if a == k then
                v:setShow(true)
            end
        end
    end
end

function SquadMenuGUI:setFormation( formation )
    for _,v in pairs( self.formations ) do
        v:setChecked(false)
    end

    self.selectedFormation = formation

    if self.selectedFormation and self.selectedFormation ~= f_no then
        for i,v in pairs( self.formations ) do
            if i == formation then
                v:setChecked( true )
            end
        end
    end

    --update visuals
    self:constrict()
end

return SquadMenuGUI
