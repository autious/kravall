local GUI = require "gui/GUI"
local Checkbox = require "gui/component/Checkbox"
local Image = require "gui/component/Image"

local FormationGUI  = GUI:new{height=150,width=100, anchor="SouthEast", onFormationSelect = function(formation) core.log.error("No handler set for formation callback") end }

local f_circle      = core.system.squad.formations.CircleFormation
local f_line        = core.system.squad.formations.LineFormation
local f_halfcircle  = core.system.squad.formations.HalfCircleFormation
local f_no          = core.system.squad.formations.NoFormation

local xoffset = -20

function FormationGUI:new(o)
    o = GUI.new(self,o)

    o.selectedFormation = f_no
    o.formations = {}
    o.formations[f_line]= Checkbox:new {  
                            doStateSwitchOnPress = false,
                            checked = false,
                            matOpen             = "assets/texture/ui/form-line-unselected_00.material",
                            matSelected         = "assets/texture/ui/form-line-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/form-line-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/form-line-extra_00.material",
                            onClick            = function(self,value) if value then o.onFormationSelect( f_line ) else o.onFormationSelect( f_no ) end end ,
                            xoffset             = xoffset
                        }

    o.formations[f_halfcircle] = Checkbox:new {  
                            doStateSwitchOnPress = false,
                            checked = false,
                            matOpen             = "assets/texture/ui/form-circle-unselected_00.material",
                            matSelected         = "assets/texture/ui/form-circle-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/form-circle-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/form-circle-extra_00.material",
                            onClick            = function(self,value) if value then o.onFormationSelect( f_halfcircle ) else o.onFormationSelect( f_no ) end end ,
                            xoffset             = xoffset
                        }

    o.formations[f_circle] = Checkbox:new {  
                            doStateSwitchOnPress = false,
                            checked = false,
                            matOpen             = "assets/texture/ui/form-square-unselected_00.material",
                            matSelected         = "assets/texture/ui/form-square-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/form-square-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/form-square-extra_00.material",
                            onClick            = function(self,value) if value then o.onFormationSelect( f_circle ) else o.onFormationSelect( f_no ) end end ,
                            xoffset             = xoffset
                        }

    o:addPlacementHandler( require "gui/placement/EastPlacer" )

    for _,v in pairs( o.formations ) do
        o:addComponent( v )
    end

    o.bgImage = Image:new
    { 
        ignoreConstrict=true,
        mat="assets/texture/ui/form-background_00.material"
    }
    o.height = o.bgImage.height
    o.width = o.bgImage.width

    o:addComponent( o.bgImage )


    return o
end

function FormationGUI:setFormation( formation )
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

return FormationGUI
