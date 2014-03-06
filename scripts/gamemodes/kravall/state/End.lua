local GUI = require "gui/GUI"
local EndScreen = require "gui/EndScreen"

local Objective = require "gui/component/objective_handler/Objective"
--local Button = require "gui/component/Button"
--local Slider = require "gui/component/Slider"
--local Checkbox = require "gui/component/Checkbox"
--local TextSelectList = require "gui/component/TextSelectList"
--local TextLabel = require "gui/component/TextLabel"
--local TextBox = require "gui/component/TextBox"
--local Image = require "gui/component/Image"
--
--local AnchorPlacer = require "gui/placement/AnchorPlacer"
--local EastPlacer = require "gui/placement/EastPlacer"
--local NorthPlacer = require "gui/placement/NorthPlacer"
--local WestPlacer = require "gui/placement/WestPlacer"
--local CenterPlacer = require "gui/placement/CenterPlacer"

--local Util = require "gui/placement/util"

local End = 
{ 
    name = "End",
    won = false
}

function End:new(o)
    o = o or {}
    setmetatable( o, self )
    self.__index = self
	
	o.objectives = {}
	
	for i,v in pairs( o.objectiveList ) do
		print( v.label )
		table.insert(o.objectives, Objective:new( { title=v.label, active=i.active, state=i.state, show=true } ) )
    end
	
	print( "Objectives length: " .. #o.objectives )
	
    o.gui = EndScreen:new( { width=500, height=600, won=o.won, objectives=o.objectives } )
	
    --o.gui:addPlacementHandler( CenterPlacer )
    --
    --if o.won then
    --    o.gui:addComponent( Image:new{ mat = "assets/texture/ui/win.material" } )
    --else
    --    o.gui:addComponent( Image:new{ mat = "assets/texture/ui/loss.material" } )
    --end
    --
    --o.gui:addComponent( Button:new
    --{
    --    onClick = function() openscenario( "main_menu" ) end 
    --} )

    return o
end

function End:update( delta )

end

function End:destroy()
    self.gui:destroy()
end

return End
