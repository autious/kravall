local GUI = require "gui/GUI"
local Checkbox = require "gui/component/Checkbox"
local TextLabel = require "gui/component/TextLabel" 
local U = require "gui/placement/util"

local AbilityGUI = GUI:new{width=100,height=175,anchor="Center"}

s_ability = core.system.squad.abilities
function AbilityGUI:new(o)
    o = GUI.new(self,o)


    local components = {}

    local function onHoverAbility(self)
        print( "Woah" )
        for i,v in pairs(components) do
            if v == self then
                meta = getmetatable( i )
                o.selectionLabel:setLabel( meta.__tostring(i) )
            end
        end
        -- Rencenter the label.
        o.labelWrapperGUI:constrict()
    end


    components[s_ability.Flee] = Checkbox:new {  
                            checked = false,
                            matOpen             = "assets/texture/ui/grenade-unselected_00.material",
                            matSelected         = "assets/texture/ui/grenade-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/grenade-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/grenade-extra_00.material",
                            onHover             = onHoverAbility
                        }

    components[s_ability.Rout] = Checkbox:new {  
                            checked = false,
                            matOpen             = "assets/texture/ui/rout-unselected_00.material",
                            matSelected         = "assets/texture/ui/rout-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/rout-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/rout-extra_00.material",
                            onHover             = onHoverAbility
                        }
     

    components[s_ability.Sprint] = Checkbox:new {  
                            checked = false,
                            matOpen             = "assets/texture/ui/power-unselected_00.material",
                            matSelected         = "assets/texture/ui/power-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/power-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/power-extra_00.material",
                            onHover             = onHoverAbility
                        }

    components[s_ability.Attack] = Checkbox:new {  
                            checked = false,
                            matOpen             = "assets/texture/ui/attack-unselected_00.material",
                            matSelected         = "assets/texture/ui/attack-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/attack-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/attack-extra_00.material",
                            onHover             = onHoverAbility
                        }

    components[s_ability.ArrestIndividual] = Checkbox:new {  
                            checked = false,
                            matOpen             = "assets/texture/ui/arrest-individual-unselected_00.material",
                            matSelected         = "assets/texture/ui/arrest-individual-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/arrest-individual-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/arrest-individual-extra_00.material",
                            onHover             = onHoverAbility
                        }

    components[s_ability.ArrestGroup] = Checkbox:new {  
                            checked = false,
                            matOpen             = "assets/texture/ui/arrest-group-unselected_00.material",
                            matSelected         = "assets/texture/ui/arrest-group-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/arrest-group-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/arrest-group-extra_00.material",
                            onHover             = onHoverAbility
                        }

    components[s_ability.TearGas] = Checkbox:new {  
                            checked = false,
                            matOpen             = "assets/texture/ui/tear-gas-unselected_00.material",
                            matSelected         = "assets/texture/ui/tear-gas-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/tear-gas-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/tear-gas-extra_00.material",
                            onHover             = onHoverAbility
                        }

    components[s_ability.Taze] = Checkbox:new {  
                            checked = false,
                            matOpen             = "assets/texture/ui/taze-unselected_00.material",
                            matSelected         = "assets/texture/ui/taze-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/taze-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/taze-extra_00.material",
                            onHover             = onHoverAbility
                        }

    components[s_ability.Blitz] = Checkbox:new {  
                            checked = false,
                            matOpen             = "assets/texture/ui/blitz-unselected_00.material",
                            matSelected         = "assets/texture/ui/blitz-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/blitz-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/blitz-extra_00.material",
                            onHover             = onHoverAbility
                        }

    local standardAbilities = 
    {
        s_ability.Flee,
        s_ability.Rout,
        s_ability.Sprint,
    }

    local specialAbilities =
    {
        s_ability.Attack,
        s_ability.ArrestIndividual,
        s_ability.ArrestGroup,
    }
    
    local specialAbilities2 =
    {
        s_ability.TearGas,
        s_ability.Taze,
        s_ability.Blitz
    }

    local width
    local height

    local standardComponents = {}
    for _,v in ipairs( standardAbilities ) do
        standardComponents[#standardComponents+1] = components[v]
    end
    width,height = U.getTotalDimHeight( standardComponents, 10, 10 )
    o.standard = GUI:new({width=width,height=height})
    o.standard:addComponents( standardComponents )
    o.standard:addPlacementHandler( require "gui/placement/CenterPlacer" )

    local policeSpecificComponents = {}
    for _,v in ipairs( specialAbilities ) do
        policeSpecificComponents[#policeSpecificComponents+1] = components[v]
    end
    width,height = U.getTotalDimHeight( standardComponents, 10, 10 )
    o.policeSpecific = GUI:new({width=width,height=height})
    o.policeSpecific:addComponents( policeSpecificComponents )
    o.policeSpecific:addPlacementHandler( require "gui/placement/CenterPlacer" )

    local policeSpecificComponents2 = {}
    for _,v in ipairs( specialAbilities2 ) do
        policeSpecificComponents2[#policeSpecificComponents2+1] = components[v]
    end
    width,height = U.getTotalDimHeight( standardComponents, 10, 10 )
    o.policeSpecific2 = GUI:new({width=width,height=height})
    o.policeSpecific2:addComponents( policeSpecificComponents2 )
    o.policeSpecific2:addPlacementHandler( require "gui/placement/CenterPlacer" )

    o.labelWrapperGUI = GUI:new{width=o.width,height=14}

    buttonGUIComponents = {}
    buttonGUIComponents[#buttonGUIComponents+1] = o.policeSpecific
    buttonGUIComponents[#buttonGUIComponents+1] = o.policeSpecific2
    buttonGUIComponents[#buttonGUIComponents+1] = o.standard 
    width,height = U.getTotalDimWidth( buttonGUIComponents, 10, 10 )
    o.buttonGUI = GUI:new({width=width,height=height})
    o.buttonGUI:addComponents( buttonGUIComponents )
    o.buttonGUI:addPlacementHandler( require "gui/placement/EastHorizontalPlacer" )


    o.labelWrapperGUI:addPlacementHandler( require "gui/placement/CenterPlacer" )
    o.selectionLabel = TextLabel:new({label=""})
    o.labelWrapperGUI:addComponent( o.selectionLabel )

    o:addComponent( o.labelWrapperGUI )
    o:addComponent( o.buttonGUI )
    o:addPlacementHandler( require "gui/placement/EastPlacer" )

    return o
end

return AbilityGUI
