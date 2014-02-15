local GUI = require "gui/GUI"
local Checkbox = require "gui/component/Checkbox"

local StanceGUI = GUI:new{width=100,height=150,anchor="North"}

function StanceGUI:new(o)
    o = GUI.new(self,o)    

    local stanceDefensiveDATA = {
                        checked = false,
                        matOpen = "assets/texture/ui/defensive-unselected_00.material",
                        matSelected = "assets/texture/ui/defensive-selected_00.material",
                        matHoverOpen = "assets/texture/ui/defensive-hover_00.material",
                        matHoverSelected = "assets/texture/ui/defensive-extra_00.material"
                      }

    local stancePassiveDATA = {
                        checked = false,
                        matOpen = "assets/texture/ui/defensive-unselected_00.material",
                        matSelected = "assets/texture/ui/defensive-selected_00.material",
                        matHoverOpen = "assets/texture/ui/defensive-hover_00.material",
                        matHoverSelected = "assets/texture/ui/defensive-extra_00.material"
                      }

    local stanceAggressiveDATA = {
                        checked = false,
                        matOpen = "assets/texture/ui/aggressive-unselected_00.material",
                        matSelected = "assets/texture/ui/aggressive-selected_00.material",
                        matHoverOpen = "assets/texture/ui/aggressive-hover_00.material",
                        matHoverSelected = "assets/texture/ui/aggressive-extra_00.material"
                      }
   
    local buttonDefensive = Checkbox:new( stanceDefensiveDATA )
    local buttonPassive = Checkbox:new( stancePassiveDATA )
    local buttonAggressive = Checkbox:new( stanceAggressiveDATA )

    o:addPlacementHandler(require "gui/placement/AnchorPlacer")
    o:addComponent( buttonDefensive )
    o:addComponent( buttonPassive )
    o:addComponent( buttonAggressive )

    return o
end


return StanceGUI
