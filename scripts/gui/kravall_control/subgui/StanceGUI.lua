local GUI = require "gui/GUI"
local Checkbox = require "gui/component/Checkbox"

local StanceGUI = GUI:new{width=100,height=150,anchor="North"}

local s_aggressive = core.PoliceStance.Aggressive
local s_defensive = core.PoliceStance.Defensive
local s_passive = core.PoliceStance.Passive

function StanceGUI:new(o)
    o = GUI.new(self,o)    

    o.stances = {}

    o.stances[s_aggressive] = Checkbox:new({
                        doStateSwitchOnPress = false,
                        checked = false,
                        matOpen = "assets/texture/ui/defensive-unselected_00.material",
                        matSelected = "assets/texture/ui/defensive-selected_00.material",
                        matHoverOpen = "assets/texture/ui/defensive-hover_00.material",
                        matHoverSelected = "assets/texture/ui/defensive-extra_00.material"
                      })

    o.stances[s_defensive] = Checkbox:new( {
                        doStateSwitchOnPress = false,
                        checked = false,
                        matOpen = "assets/texture/ui/defensive-unselected_00.material",
                        matSelected = "assets/texture/ui/defensive-selected_00.material",
                        matHoverOpen = "assets/texture/ui/defensive-hover_00.material",
                        matHoverSelected = "assets/texture/ui/defensive-extra_00.material"
                      })

    o.stances[s_passive] = Checkbox:new({
                        doStateSwitchOnPress = false,
                        checked = false,
                        matOpen = "assets/texture/ui/aggressive-unselected_00.material",
                        matSelected = "assets/texture/ui/aggressive-selected_00.material",
                        matHoverOpen = "assets/texture/ui/aggressive-hover_00.material",
                        matHoverSelected = "assets/texture/ui/aggressive-extra_00.material"
                      })

    o:addPlacementHandler(require "gui/placement/CenterPlacer")
    o:addComponents( o.stances )

    return o
end

function StanceGUI:setStance( stance )
    if stance then
        if o.stances[stance] then
           o.stances[stance]:setChecked( true ) 
        end
    end  
end


return StanceGUI
