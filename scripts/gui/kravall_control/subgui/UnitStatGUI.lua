local GUI = require "gui/GUI"
local Checkbox = require "gui/component/Checkbox"

local UnitStatGUI = GUI:new{x=0,y=0, width=150, height=50, anchor="SouthEast"}

function UnitStatGUI:new(o)
    o = GUI.new(self,o)

    local buttonDefensive = Checkbox:new({
                        checked = false,
                        matOpen = "assets/texture/ui/defensive-unselected_00.material",
                        matSelected = "assets/texture/ui/defensive-selected_00.material",
                        matHoverOpen = "assets/texture/ui/defensive-hover_00.material",
                        matHoverSelected = "assets/texture/ui/defensive-extra_00.material",
                        anchor = "Center" 
                      })

    local buttonPassive = Checkbox:new( {
                        xoffset = 50,
                        checked = false,
                        matOpen = "assets/texture/ui/defensive-unselected_00.material",
                        matSelected = "assets/texture/ui/defensive-selected_00.material",
                        matHoverOpen = "assets/texture/ui/defensive-hover_00.material",
                        matHoverSelected = "assets/texture/ui/defensive-extra_00.material",
                        anchor = "West"
                      })

    local buttonAggressive = Checkbox:new( {
                        xoffset = 100,
                        checked = false,
                        matOpen = "assets/texture/ui/aggressive-unselected_00.material",
                        matSelected = "assets/texture/ui/aggressive-selected_00.material",
                        matHoverOpen = "assets/texture/ui/aggressive-hover_00.material",
                        matHoverSelected = "assets/texture/ui/aggressive-extra_00.material",
                        anchor = "East" 
                      })

    o:addPlacementHandler(require "gui/placement/SimplePlacer")
    o:addComponent( buttonDefensive )
    o:addComponent( buttonPassive )
    o:addComponent( buttonAggressive )
    return o
end

return UnitStatGUI
