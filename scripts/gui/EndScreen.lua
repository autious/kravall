local GUI = require "gui/GUI"
local Statistics = require "factories/Statistics"

local TextLabel = require "gui/component/TextLabel"
local TextBox = require "gui/component/TextBox"
local Image = require "gui/component/Image"
local Button = require "gui/component/Button"
local Objective = require "gui/component/objective_handler/Objective"

local SimplePlacer = require "gui/placement/SimplePlacer"

local Util = require "gui/placement/util"

local EndScreen = 
{
	anchor="Center",
	won=false,
	objectives={}
    --policeTypes = {},
    --onSelectCurrentSquad = function(squadDef) end,
    --onFinished = function() end,
    --onSelectCreatedSquad = function (squadInstance) end,
    --onRemoveSelected = function() end
}

function EndScreen:new(o)
    o = o or {}
    setmetatable(o,self)
    self.__index = self

	local guiWidth = 500
	local offsetX = 15
	local indentX = 30
	local offsetY = 0
	local newSectionY = 50
	local newLineY = 20
	
    o.gui = GUI:new( { width=guiWidth, height=600, anchor="Center" } )
	
	if o.won then
		--o.gui:addComponent( TextLabel:new( { label="Game is over, you won", xoffset=offsetX, yoffset=offsetY } ) )
		o.gui:addComponent( Image:new{ mat = "assets/texture/ui/win.material" } )
	else
		--o.gui:addComponent( TextLabel:new( { label="Game is over, you lost", xoffset=offsetX, yoffset=offsetY  } ) )
		o.gui:addComponent( Image:new{ mat = "assets/texture/ui/loss.material" } )
	end
	
	o.gui:addComponent( Button:new
    {
        onClick = function() openscenario( "main_menu" ) end 
    } )
	
	--offsetY = offsetY + newSectionY
	--o.gui:addComponent( TextLabel:new( { label="Objectives", xoffset=offsetX, yoffset=offsetY } ) )
	--offsetY = offsetY + newLineY
	--
	--table.insert( o.objectives, Objective:new( { title="This is a very very very long objective title. At least that's what it is supposed to be. Use your imagination!! ;) ", state="fail", show=true } ) )
	--table.insert( o.objectives, Objective:new( { title="Another objective...", state="fail", show=true } ) )
	--
	--local tempY = offsetY
	--if o.objectives then
	--	for i,v in pairs( o.objectives ) do
	--		local colour = {1,1,1,1}
	--		local objIndent=100
	--		
	--		print(offsetY)
	--		if v.state == "fail" then
	--			colour = {1,0,0,1}
	--			o.gui:addComponent( TextLabel:new( { label="[FAIL]", color=colour, xoffset=offsetX+indentX, yoffset=offsetY  } ) )
	--		else
	--			colour = {0,1,0,1}
	--			o.gui:addComponent( TextLabel:new( { label="[SUCCESS]", color=colour, xoffset=offsetX+indentX, yoffset=offsetY } ) )
	--		end
	--		
	--		local currObjective = TextBox:new( 
	--										{ 
	--											body=v.title, color=colour, xoffset=offsetX+objIndent+indentX, yoffset=offsetY,
	--											width=guiWidth-(offsetX+objIndent+indentX), height=200
	--										} )
	--		local dimX, dimY = currObjective.text:getDim()
	--		currObjective:destroy()
	--		currObjective = TextBox:new( 
	--										{ 
	--											body=v.title, color=colour, xoffset=offsetX+objIndent+indentX, yoffset=offsetY-dimY,
	--											width=guiWidth-(offsetX+objIndent+indentX), height=200
	--										} )							
	--		o.gui:addComponent( currObjective )
	--		offsetY = offsetY + dimY
	--	end
	--end
	--
	--offsetY = offsetY + newSectionY
	--o.gui:addComponent( TextLabel:new( { label="Units", xoffset=offsetX, yoffset=offsetY } ) )
	--offsetY = offsetY + newLineY
	--o.gui:addComponent( TextLabel:new( { label="Rioters killed: 322", xoffset=offsetX+indentX, yoffset=offsetY } ) )
	--offsetY = offsetY + newLineY
	--o.gui:addComponent( TextLabel:new( { label="Police killed: 25", xoffset=offsetX+indentX, yoffset=offsetY } ) )
	
	--o.gui:addPlacementHandler( SimplePlacer )
	o.gui:addPlacementHandler( require "gui/placement/CenterPlacer" )
    return o
end

function EndScreen:destroy()
    self.gui:destroy()  
    self.gui = nil
end

return EndScreen
