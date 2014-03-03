local GUI = require "gui/GUI"
local Checkbox = require "gui/component/Checkbox"
local TextLabel = require "gui/component/TextLabel"

local UnitStatGUI = GUI:new{x=0,y=0, width=250, height=130, anchor="SouthEast"}

function UnitStatGUI:new(o)
    o = GUI.new(self,o)
    
    --o.lifeGUI = GUI:new({ anchor="South", x=0,y=0, width=40, height=70})
    --o.lifeGUI:addPlacementHandler( require "gui/placement/SimplePlacer" )

    local wrapperGUI = GUI:new{width=2,height=2, yoffset=-10, anchor="SouthWest"}

    o.labelUnitName =  TextLabel:new( { label="Police Unit", anchor="SouthWest" })
    o.labelUnitHealth = TextLabel:new( { label="Health: NaN", anchor="SouthWest" } )
    o.labelUnitMorale = TextLabel:new( { label="Morale: NaN", anchor="SouthWest" } )
    o.labelUnitStamina = TextLabel:new( { label="Stamina: NaN", anchor="SouthWest" } )
    o.labelUnitStance = TextLabel:new( { label="Stance: NaN", anchor="SouthWest" } )
    o.labelUnitFormation = TextLabel:new( { label="Formation: NaN", anchor="SouthWest" } )

    o:addPlacementHandler(require "gui/placement/AnchorPlacer")
    wrapperGUI:addPlacementHandler(require "gui/placement/AnchorPlacer")

    wrapperGUI:addComponent( o.labelUnitName )
    wrapperGUI:addComponent( o.labelUnitHealth )
    wrapperGUI:addComponent( o.labelUnitMorale )
    wrapperGUI:addComponent( o.labelUnitStamina )
    wrapperGUI:addComponent( o.labelUnitStance )
    wrapperGUI:addComponent( o.labelUnitFormation )
    
    o:addComponent( wrapperGUI )

    return o
end

function UnitStatGUI:setFormation( formation )
    local formations = core.system.squad.formations
    if formation == formations.LineFormation then
        self.labelUnitFormation:setLabel( "Formation: Line" )
    elseif formation == formations.CircleFormation then
        self.labelUnitFormation:setLabel( "Formation: Circle" )
    elseif formation == formations.HalfCircleFormation then
        self.labelUnitFormation:setLabel( "Formation: Half Circle" )
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
    self.labelUnitName:setLabel( name )
end

function UnitStatGUI:update(delta)
    GUI.update(self,delta)
end

return UnitStatGUI
