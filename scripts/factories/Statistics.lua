local GUI = require "gui/GUI"

local Statistics = 
                {
					categories={},
					name="Statistics"
                }

local guiWidth = 500
local offsetX = 15
local indentX = 30
local newSectionY = 50
local newLineY = 20
local objIndent = 100

--function Statistics:new( o )
--    o = o or {}
--    setmetatable( o, self )
--    self.__index = self
--	
--	--o.text = text( 1, 1, o.text or "Empty ToolTip", { 0, 0, 0, 1}, "assets/font/toolTip.font" )
--	--local textDimX, textDimY = o.text:getDim()
--	--o.width = textDimX + textOff.x * 2
--	--o.height = 24
--	--o.show = false
--	--o.text:show( false )
--	
--	return o
--end

function Statistics.addObjectives( rows )
	Statistics.categories[ "objectives" ] = rows
	print( "Adding objectives..." )
	for i,v in pairs( Statistics.categories[ "objectives" ] ) do
		print ( i .. ": " .. v.title )
	end
end

function Statistics.addCategory( name, rows )
	Statistics.categories[ name ] = rows
end

function Statistics.clear()
	Statistics.categories = {}
	
	Statistics.text:destroy()
end

function Statistics.getCategoryAsSubGUI( name )
	if Statistics.categories[ name ] then
		local subGUI = GUI:new( {
									--width=
								} )
	
	else
		core.log.error( "Lua Statistics: No category exists named " .. name )
	end
end

function Statistics.getObjectivesAsSubGUI()
	objectivesGUI = GUI:new ( {
							} )
							
	local offsetY = 0
	objectivesGUI:addComponent( TextLabel:new( { label="objectives", xoffset=offsetX, yoffset=offsetY } ) )
	offsetY = offsetY + newLineY
	
	for i,v in pairs( self.categories[ "objectives" ] ) do
		local colour = {1,1,1,1}
		
		print(offsetY)
		if v.state == "fail" then
			colour = {1,0,0,1}
			objectivesGUI:addComponent( TextLabel:new( { label="[FAIL]", color=colour, xoffset=offsetX+indentX, yoffset=offsetY  } ) )
		else
			colour = {0,1,0,1}
			objectivesGUI:addComponent( TextLabel:new( { label="[SUCCESS]", color=colour, xoffset=offsetX+indentX, yoffset=offsetY } ) )
		end
		
		local currObjective = TextBox:new( 
										{ 
											body=v.title, color=colour, xoffset=offsetX+objIndent+indentX, yoffset=offsetY,
											width=guiWidth-(offsetX+objIndent+indentX), height=200
										} )
		local dimX, dimY = currObjective.text:getDim()
		currObjective:destroy()
		currObjective = TextBox:new( 
										{ 
											body=v.title, color=colour, xoffset=offsetX+objIndent+indentX, yoffset=offsetY-dimY,
											width=guiWidth-(offsetX+objIndent+indentX), height=200
										} )							
		objectivesGUI:addComponent( currObjective )
		offsetY = offsetY + dimY
	end
	
	return objectivesGUI
end

return Statistics