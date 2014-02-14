local GUI = require "gui/GUI"
local Checkbox = require "gui/component/Checkbox"
local TextLabel = require "gui/component/TextLabel"

local UnitStatGUI = GUI:new{x=0,y=0, width=150, height=70, anchor="SouthEast"}

function UnitStatGUI:new(o)
    o = GUI.new(self,o)

    
    o.lifeGUI = GUI:new({ anchor="South", x=0,y=0, width=40, height=30})
    o.lifeGUI:addPlacementHandler( require "gui/placement/SimplePlacer" )

    local buttonDefensive = Checkbox:new({
                        checked = false,
                        matOpen = "assets/texture/ui/defensive-unselected_00.material",
                        matSelected = "assets/texture/ui/defensive-selected_00.material",
                        matHoverOpen = "assets/texture/ui/defensive-hover_00.material",
                        matHoverSelected = "assets/texture/ui/defensive-extra_00.material",
                      })

    local buttonPassive = Checkbox:new( {
                        xoffset = 50,
                        checked = false,
                        matOpen = "assets/texture/ui/defensive-unselected_00.material",
                        matSelected = "assets/texture/ui/defensive-selected_00.material",
                        matHoverOpen = "assets/texture/ui/defensive-hover_00.material",
                        matHoverSelected = "assets/texture/ui/defensive-extra_00.material",
                      })

    local buttonAggressive = Checkbox:new( {
                        xoffset = 100,
                        checked = false,
                        matOpen = "assets/texture/ui/aggressive-unselected_00.material",
                        matSelected = "assets/texture/ui/aggressive-selected_00.material",
                        matHoverOpen = "assets/texture/ui/aggressive-hover_00.material",
                        matHoverSelected = "assets/texture/ui/aggressive-extra_00.material",
                      })
    
    local labelUnitName =  TextLabel:new( { label="Police Unit", anchor="North" })

    o:addPlacementHandler(require "gui/placement/AnchorPlacer")
    --o.lifeGUI:addComponent( buttonDefensive )
    --o.lifeGUI:addComponent( buttonPassive )
    --o.lifeGUI:addComponent( buttonAggressive )
    o:addComponent( labelUnitName )
    o:addComponent( o.lifeGUI )
    return o
end

function UnitStatGUI:update(delta)
    self.lifeGUI:update(delta)
    GUI.update(self,delta)
end

return UnitStatGUI
