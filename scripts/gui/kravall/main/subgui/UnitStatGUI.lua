local GUI = require "gui/GUI"
local Checkbox = require "gui/component/Checkbox"
local TextLabel = require "gui/component/TextLabel"
local Image = require "gui/component/Image"

local SimplePlacer = require "gui/placement/SimplePlacer"

local UnitStatGUI = GUI:new{x=0,y=0, width=300, height=120, anchor="SouthEast"}

function UnitStatGUI:new(o)
    o = GUI.new(self,o)
    
    --o.lifeGUI = GUI:new({ anchor="South", x=0,y=0, width=40, height=70})
    --o.lifeGUI:addPlacementHandler( require "gui/placement/SimplePlacer" )
	
	local xOff = 12
    local wrapperGUI = GUI:new{ width=2, height=2, yoffset=-10, anchor="SouthWest" }

    --o.labelUnitName =  TextLabel:new( { label="Police Unit", yoffset=0, font="assets/font/toolTip.font" })
    --o.labelUnitHealth = TextLabel:new( { label="Health: NaN", yoffset=18, font="assets/font/toolTip.font"  } )
    --o.labelUnitMorale = TextLabel:new( { label="Morale: NaN", yoffset=36, font="assets/font/toolTip.font"  } )
    --o.labelUnitStamina = TextLabel:new( { label="Stamina: NaN", yoffset=54, font="assets/font/toolTip.font"  } )
    --o.labelUnitStance = TextLabel:new( { label="Stance: NaN", yoffset=72, font="assets/font/toolTip.font" } )
    --o.labelUnitFormation = TextLabel:new( { label="Formation: NaN", yoffset=90, font="assets/font/toolTip.font"  } )
	
	o.labelUnitHealth = TextLabel:new( { label="Health: NaN", xoffset=xOff, yoffset=12, font="assets/font/toolTip.font"  } )
    o.labelUnitMorale = TextLabel:new( { label="Morale: NaN", xoffset=xOff, yoffset=30, font="assets/font/toolTip.font"  } )
    o.labelUnitStamina = TextLabel:new( { label="Stamina: NaN", xoffset=xOff, yoffset=30, font="assets/font/toolTip.font"  } )
    o.labelUnitStance = TextLabel:new( { label="Stance: NaN", xoffset=xOff, yoffset=48, font="assets/font/toolTip.font" } )
    o.labelUnitFormation = TextLabel:new( { label="Formation: NaN", xoffset=xOff, yoffset=66, font="assets/font/toolTip.font"  } )
	o.labelUnitMorale:setShow( false )

    --wrapperGUI:addComponent( o.labelUnitName )
    wrapperGUI:addComponent( o.labelUnitHealth )
    --wrapperGUI:addComponent( o.labelUnitMorale )
    wrapperGUI:addComponent( o.labelUnitStamina )
    wrapperGUI:addComponent( o.labelUnitStance )
    wrapperGUI:addComponent( o.labelUnitFormation )
	
	wrapperGUI:addComponent( Image:new( { mat="assets/texture/ui/status-bar.material" } ) )
	wrapperGUI:addComponent( Image:new( { mat="assets/texture/ui/status-box-background.material" } ) )
	
	wrapperGUI:addPlacementHandler( SimplePlacer )
	wrapperGUI:setShow( false )
	
    o:addComponent( wrapperGUI )
	
	o:addPlacementHandler( SimplePlacer )
	
    return o
end

function UnitStatGUI:setFormation( formation )
    local formations = core.system.squad.formations
    if formation == formations.LineFormation then
        self.labelUnitFormation:setLabel( "Formation: Line" )
    elseif formation == formations.CircleFormation then
        self.labelUnitFormation:setLabel( "Formation: Circle" )
    elseif formation == formations.HalfCircleFormation then
        self.labelUnitFormation:setLabel( "Formation: Semi-Circle" )
    elseif formation == formations.NoFormation then
        self.labelUnitFormation:setLabel( "Formation: None" )
    else
        self.labelUnitFormation:setLabel( "Formation: Unknown" )
    end
end

function UnitStatGUI:setStance( stance )
    local s = core.PoliceStance

    if stance == s.Defensive then
        self.labelUnitStance:setLabel( "Stance: Defensive")
    elseif stance == s.Aggressive then
        self.labelUnitStance:setLabel( "Stance: Aggressive")
    elseif stance == s.Passive then
        self.labelUnitStance:setLabel( "Stance: Passive")
    else
        self.labelUnitStance:setLabel( "Stance: None")
    end
end

function UnitStatGUI:setHealth( health )
    self.labelUnitHealth:setLabel( "Health: " .. string.format( "%.0f",health ) )
end

function UnitStatGUI:setMorale( morale )
    self.labelUnitMorale:setLabel( "Morale: " .. string.format( "%.2f", morale ) )
end

function UnitStatGUI:setStamina( stamina )
    self.labelUnitStamina:setLabel( "Stamina: " .. string.format( "%.0f", stamina ) )
end

function UnitStatGUI:setName( name )
	--print( name )
    --self.labelUnitName:setLabel( name )
end

function UnitStatGUI:update(delta)
    GUI.update(self,delta)
end

return UnitStatGUI
