local GUI = require "gui/GUI"
local Checkbox = require "gui/component/Checkbox"

local FormationGUI  = GUI:new{width=100,height=150,anchor="South", onFormationSet = function() core.log.error("No handler set for formation callback") end }

local f_circle      = core.system.squad.formations.CircleFormation
local f_line        = core.system.squad.formations.HalfCircleFormation
local f_halfcircle  = core.system.squad.formations.LineFormation
local f_no          = core.system.squad.formations.NoFormation

function FormationGUI:new(o)
    o = GUI.new(self,o)

    o.formations = {}
    o.formations[f_line]= Checkbox:new {  
                            checked = false,
                            matOpen             = "assets/texture/ui/form-line-unselected_00.material",
                            matSelected         = "assets/texture/ui/form-line-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/form-line-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/form-line-extra_00.material",
                            onChange            = function(value) o:onFormationSet( f_line ) end 
                        }

    o.formations[f_halfcircle] = Checkbox:new {  
                            checked = false,
                            matOpen             = "assets/texture/ui/form-circle-unselected_00.material",
                            matSelected         = "assets/texture/ui/form-circle-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/form-circle-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/form-circle-extra_00.material",
                            onChange            = function(value) o:onFormationSet( f_halfcircle ) end 
                        }

    o.formations[f_circle] = Checkbox:new {  
                            checked = false,
                            matOpen             = "assets/texture/ui/form-square-unselected_00.material",
                            matSelected         = "assets/texture/ui/form-square-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/form-square-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/form-square-extra_00.material",
                            onChange            = function(value) o:onFormationSet( f_circle ) end 
                        }

    o:addPlacementHandler( require "gui/placement/AnchorPlacer" )

    for _,v in pairs( o.formations ) do
        o:addComponent( v )
    end


    return o
end

function FormationGUI:setFormation( formation )
    print( formation )
    for _,v in pairs( self.formations ) do
        v.checked = false
    end

    if self.selectedFormation ~= formation and self.selectedFormation ~= f_no then
        self.selectedFormation = formation
        self.formations[formation].checked = true
    else
        self.selectedFormation = f_no
    end

    --update visuals
    self:constrict()
end

return FormationGUI
