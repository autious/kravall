local GUI = require "gui/GUI"
local Checkbox = require "gui/component/Checkbox"

local AbilityGUI = GUI:new{width=100,height=150,anchor="Center"}

function AbilityGUI:new(o)
    o = GUI.new(self,o)

    local abilityGrenade = Checkbox:new {  
                            checked = false,
                            matOpen             = "assets/texture/ui/grenade-unselected_00.material",
                            matSelected         = "assets/texture/ui/grenade-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/grenade-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/grenade-extra_00.material"
                        }

    local abilityArrest = Checkbox:new {  
                            checked = false,
                            matOpen             = "assets/texture/ui/arrest-unselected_00.material",
                            matSelected         = "assets/texture/ui/arrest-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/arrest-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/arrest-extra_00.material"
                        }

    local abilityPower = Checkbox:new {  
                            checked = false,
                            matOpen             = "assets/texture/ui/power-unselected_00.material",
                            matSelected         = "assets/texture/ui/power-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/power-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/power-extra_00.material"
                        }

    o:addPlacementHandler( require "gui/placement/AnchorPlacer" )

    o:addComponent( abilityGrenade )
    o:addComponent( abilityArrest )
    o:addComponent( abilityPower )

    return o
end

return AbilityGUI
