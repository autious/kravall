local GUI = require "gui/GUI"
local Checkbox = require "gui/component/Checkbox"
local Image = require "gui/component/Image"
local toolTipHandler = require "gui/tooltip/ToolTipHandler"
local createTT = require "gui/tooltip/ToolTip"

local xoffset = -20

local StanceGUI = GUI:new{
                            width=100,
                            height=150,
                            anchor="NorthEast",
                            onStanceSelect = function( value ) end  
                        }

local s_aggressive = core.PoliceStance.Aggressive
local s_defensive = core.PoliceStance.Defensive
local s_passive = core.PoliceStance.Passive

function StanceGUI:new(o)
    o = GUI.new(self,o)    

    o.stances = {}

    o.stances[s_aggressive] = Checkbox:new({
                        doStateSwitchOnPress = false,
                        checked = false,
                        matOpen = "assets/texture/ui/aggressive-unselected_00.material",
                        matSelected = "assets/texture/ui/aggressive-selected_00.material",
                        matHoverOpen = "assets/texture/ui/aggressive-hover_00.material",
                        matHoverSelected = "assets/texture/ui/aggressive-extra_00.material",
                        onClick = function( self, value ) o.onStanceSelect( s_aggressive ) end,
                        xoffset = xoffset,
						toolTip	= createTT:new( { handler=toolTipHandler, text="Aggressive Stance - I" } )
                      })


    o.stances[s_defensive] = Checkbox:new( {
                        doStateSwitchOnPress = false,
                        checked = false,
                        matOpen = "assets/texture/ui/defensive-unselected_00.material",
                        matSelected = "assets/texture/ui/defensive-selected_00.material",
                        matHoverOpen = "assets/texture/ui/defensive-hover_00.material",
                        matHoverSelected = "assets/texture/ui/defensive-extra_00.material",
                        onClick = function( self, value ) o.onStanceSelect( s_defensive ) end,
                        xoffset = xoffset,
						toolTip	= createTT:new( { handler=toolTipHandler, text="Defensive Stance - O" } )
                      })

    o.stances[s_passive] = Checkbox:new({
                        doStateSwitchOnPress = false,
                        checked = false,
                        matOpen = "assets/texture/ui/passive-unselected_00.material",
                        matSelected = "assets/texture/ui/passive-selected_00.material",
                        matHoverOpen = "assets/texture/ui/passive-hover_00.material",
                        matHoverSelected = "assets/texture/ui/passive-extra_00.material",
                        onClick = function( self, value ) o.onStanceSelect( s_passive ) end,
                        xoffset = xoffset,
						toolTip	= createTT:new( { handler=toolTipHandler, text="Passive Stance - P" } )
                      })

    o.bgImage = Image:new
    { 
        ignoreConstrict=true,
        mat="assets/texture/ui/stance-background_00.material"
    }

    o.height = o.bgImage.height
    o.width = o.bgImage.width

    o:addPlacementHandler(require "gui/placement/EastPlacer")

    o:addComponent( o.stances[s_aggressive] )
    o:addComponent( o.stances[s_defensive] )
    o:addComponent( o.stances[s_passive] )

    o:addComponent( o.bgImage )

    return o
end

function StanceGUI:setStance( stance )
    for c_stance,v in pairs( self.stances ) do
        if c_stance == stance then
            v:setChecked( true )  
        else
            v:setChecked( false )  
        end
    end
end

return StanceGUI
