local GUI = require "gui/GUI"
local Checkbox = require "gui/component/Checkbox"
local TextLabel = require "gui/component/TextLabel" 
local U = require "gui/placement/util"
local Image = require "gui/component/Image"

local xoffset = -20

local AbilityGUI = GUI:new
{
    width=155,height=175,anchor="East",
    onAbilitySelect = function( ability ) 
        print( ability )
    end
}

s_ability = core.system.squad.abilities

function AbilityGUI:new(o)
    o = GUI.new(self,o)

    local function onHoverAbility(self)
        for i,v in pairs(o.localComponents) do
            if v == self then
                meta = getmetatable( i )
                o.selectionLabel:setLabel( meta.__tostring(i) )
            end
        end
        -- Rencenter the label.
        o.labelWrapperGUI:constrict()
    end

    local function onExit(self)
        if o.currentActiveAbility then
            meta = getmetatable( o.currentActiveAbility )
            o.selectionLabel:setLabel( meta.__tostring(o.currentActiveAbility) )
        else
            o.selectionLabel:setLabel( "" )
        end
        -- Rencenter the label.
        o.labelWrapperGUI:constrict()
    end

    local function onClickAbility(self, value)
        for i,v in pairs(o.localComponents) do
            if v == self then
                meta = getmetatable( i )
                o.onAbilitySelect(i)
            end
        end
    end

    o.localComponents = {}

    o.localComponents[s_ability.Flee] = Checkbox:new {  
                            doStateSwitchOnPress = false,
                            checked = false,
                            matOpen             = "assets/texture/ui/grenade-unselected_00.material",
                            matSelected         = "assets/texture/ui/grenade-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/grenade-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/grenade-extra_00.material",
                            onHover             = onHoverAbility,
                            onClick             = onClickAbility,
                            onExit              = onExit,
                        }

    o.localComponents[s_ability.Rout] = Checkbox:new {  
                            doStateSwitchOnPress = false,
                            checked = false,
                            matOpen             = "assets/texture/ui/rout-unselected_00.material",
                            matSelected         = "assets/texture/ui/rout-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/rout-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/rout-extra_00.material",
                            onHover             = onHoverAbility,
                            onClick             = onClickAbility,
                            onExit              = onExit,
                        }
     

    o.localComponents[s_ability.Sprint] = Checkbox:new {  
                            doStateSwitchOnPress = false,
                            checked = false,
                            matOpen             = "assets/texture/ui/power-unselected_00.material",
                            matSelected         = "assets/texture/ui/power-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/power-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/power-extra_00.material",
                            onHover             = onHoverAbility,
                            onClick             = onClickAbility,
                            onExit              = onExit,
                        }

    o.localComponents[s_ability.Attack] = Checkbox:new {  
                            doStateSwitchOnPress = false,
                            checked = false,
                            matOpen             = "assets/texture/ui/attack-unselected_00.material",
                            matSelected         = "assets/texture/ui/attack-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/attack-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/attack-extra_00.material",
                            onHover             = onHoverAbility,
                            onClick             = onClickAbility,
                            onExit              = onExit,
                        }

    o.localComponents[s_ability.ArrestIndividual] = Checkbox:new {  
                            doStateSwitchOnPress = false,
                            checked = false,
                            matOpen             = "assets/texture/ui/arrest-individual-unselected_00.material",
                            matSelected         = "assets/texture/ui/arrest-individual-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/arrest-individual-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/arrest-individual-extra_00.material",
                            onHover             = onHoverAbility,
                            onClick             = onClickAbility,
                            onExit              = onExit,
                        }

    o.localComponents[s_ability.ArrestGroup] = Checkbox:new {  
                            doStateSwitchOnPress = false,
                            checked = false,
                            matOpen             = "assets/texture/ui/arrest-group-unselected_00.material",
                            matSelected         = "assets/texture/ui/arrest-group-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/arrest-group-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/arrest-group-extra_00.material",
                            onHover             = onHoverAbility,
                            onClick             = onClickAbility,
                            onExit              = onExit,
                        }

    o.localComponents[s_ability.TearGas] = Checkbox:new {  
                            doStateSwitchOnPress = false,
                            checked = false,
                            matOpen             = "assets/texture/ui/tear-gas-unselected_00.material",
                            matSelected         = "assets/texture/ui/tear-gas-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/tear-gas-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/tear-gas-extra_00.material",
                            onHover             = onHoverAbility,
                            onClick             = onClickAbility,
                            onExit              = onExit,
                        }

    o.localComponents[s_ability.Taze] = Checkbox:new {  
                            doStateSwitchOnPress = false,
                            checked = false,
                            matOpen             = "assets/texture/ui/taze-unselected_00.material",
                            matSelected         = "assets/texture/ui/taze-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/taze-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/taze-extra_00.material",
                            onHover             = onHoverAbility,
                            onClick             = onClickAbility,
                            onExit              = onExit,
                        }

    o.localComponents[s_ability.Blitz] = Checkbox:new {  
                            doStateSwitchOnPress = false,
                            checked = false,
                            matOpen             = "assets/texture/ui/blitz-unselected_00.material",
                            matSelected         = "assets/texture/ui/blitz-selected_00.material",
                            matHoverOpen        = "assets/texture/ui/blitz-hover_00.material",
                            matHoverSelected    = "assets/texture/ui/blitz-extra_00.material",
                            onHover             = onHoverAbility,
                            onClick             = onClickAbility,
                            onExit              = onExit,
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
        standardComponents[#standardComponents+1] = o.localComponents[v]
    end
    width,height = U.getTotalDimHeight( standardComponents, 10, 10 )
    o.standard = GUI:new({width=width,height=height})
    o.standard:addComponents( standardComponents )
    o.standard:addPlacementHandler( require "gui/placement/CenterPlacer" )

    local policeSpecificComponents = {}
    for _,v in ipairs( specialAbilities ) do
        policeSpecificComponents[#policeSpecificComponents+1] = o.localComponents[v]
    end
    width,height = U.getTotalDimHeight( standardComponents, 10, 10 )
    o.policeSpecific = GUI:new({width=width,height=height})
    o.policeSpecific:addComponents( policeSpecificComponents )
    o.policeSpecific:addPlacementHandler( require "gui/placement/CenterPlacer" )

    local policeSpecificComponents2 = {}
    for _,v in ipairs( specialAbilities2 ) do
        policeSpecificComponents2[#policeSpecificComponents2+1] = o.localComponents[v]
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
    o.buttonGUI = GUI:new({width=width,height=height,xoffset=xoffset})
    o.buttonGUI:addComponents( buttonGUIComponents )
    o.buttonGUI:addPlacementHandler( require "gui/placement/EastHorizontalPlacer" )

    o.labelWrapperGUI:addPlacementHandler( require "gui/placement/CenterPlacer" )
    o.selectionLabel = TextLabel:new({label=""})
    o.labelWrapperGUI:addComponent( o.selectionLabel )

    o:addComponent( o.labelWrapperGUI )
    o:addComponent( o.buttonGUI )
    o:addPlacementHandler( require "gui/placement/EastPlacer" )

    o.bgImage = Image:new
    { 
        ignoreConstrict=true,
        mat="assets/texture/ui/use-background_00.material"
    }

    o.height = o.bgImage.height
    o.width = o.bgImage.width

    o:addComponent( o.bgImage )

    return o
end

function AbilityGUI:setAbility( ability )
    self.currentActiveAbility = ability
    for i,v in pairs( self.localComponents ) do
        if i == ability then
            v:setChecked( true )
        else
            v:setChecked( false )
        end 
    end
end

return AbilityGUI
