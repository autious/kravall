local GUI = require "gui/GUI"
local Checkbox = require "gui/component/Checkbox"

local FormationGUI = GUI:new{width=100,height=150,anchor="South"}

function FormationGUI:new(o)
    o = GUI.new(self,o)

    o.formations = {}
    o.formations["line"]= Checkbox:new {  
                            checked = false,
                            matOpen             = "assets/texture/ui/form-line-unselected_00.material",
                            matSelected         = "assets/texture/ui/form-line-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/form-line-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/form-line-extra_00.material",
                            onChange            = function(value) o:selectFormation( "line" ) end 
                        }

    o.formations["halfcircle"] = Checkbox:new {  
                            checked = false,
                            matOpen             = "assets/texture/ui/form-circle-unselected_00.material",
                            matSelected         = "assets/texture/ui/form-circle-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/form-circle-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/form-circle-extra_00.material",
                            onChange            = function(value) o:selectFormation( "halfcircle" ) end 
                        }

    o.formations["circle"] = Checkbox:new {  
                            checked = false,
                            matOpen             = "assets/texture/ui/form-square-unselected_00.material",
                            matSelected         = "assets/texture/ui/form-square-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/form-square-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/form-square-extra_00.material",
                            onChange            = function(value) o:selectFormation( "circle" ) end 
                        }

    o:addPlacementHandler( require "gui/placement/AnchorPlacer" )

    for _,v in pairs( o.formations ) do
        o:addComponent( v )
    end


    return o
end

function FormationGUI:selectFormation( formation, value )
    print( formation )
    for _,v in pairs( self.formations ) do
        v.checked = false
    end

    if self.selectedFormation ~= formation then
        self.selectedFormation = formation
        self.formations[formation].checked = true
    else
        self.selectedFormation = nil
    end

    --update visuals
    self:constrict()
end


return FormationGUI
