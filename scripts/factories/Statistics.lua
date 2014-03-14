local GUI = require "gui/GUI"

local TextLabel = require "gui/component/TextLabel"
local TextBox = require "gui/component/TextBox"

local SimplePlacer = require "gui/placement/SimplePlacer"

local Statistics = 
                {
					categories={},
					rank="Play tester",
					playTime=0,
					guiWidth = 500,
					offsetX = 0,
					indentX = 30,
					newSectionY = 50,
					newLineY = 20,
					objIndent = 100
                }

function Statistics.addObjective( objective )
	table.insert( Statistics.categories[ "objectives" ], objective )
end				
				
function Statistics.addObjectives( allObjectives )
	Statistics.categories[ "objectives" ] = allObjectives
end

function Statistics.addToCategory( name, row )
	if not Statistics.categories[ name ] then
		Statistics.categories[ name ] = {}
	end
	
	table.insert( Statistics.categories[ name ], row )
end

function Statistics.clear()
	for i,v in pairs( Statistics.categories ) do
		for j,w in pairs( v ) do
			w:destroy()
		end
	end

	Statistics.categories = {}
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

function Statistics.getObjectivesAsSubGUI(xOffset, yOffset)
	xOffset = xOffset or 0
	yOffset = yOffset or 0
	
	objectivesGUI = GUI:new ( {
									width=Statistics.guiWidth, xoffset=xOffset, yoffset=yOffset
							} )
							
	local offsetY = 0
	objectivesGUI:addComponent( TextLabel:new( { label="Objectives", xoffset=offsetX, yoffset=offsetY } ) )
	offsetY = offsetY + Statistics.newLineY
	
	for i,v in pairs( Statistics.categories[ "objectives" ] ) do
		local colour = {1,1,1,1}
		
		print(offsetY)
		if v.state == "fail" then
			colour = {1,0,0,1}
			objectivesGUI:addComponent( TextLabel:new( { label="[FAIL]", color=colour, xoffset=Statistics.offsetX+Statistics.indentX, yoffset=offsetY  } ) )
		else
			colour = {0,1,0,1}
			objectivesGUI:addComponent( TextLabel:new( { label="[SUCCESS]", color=colour, xoffset=Statistics.offsetX+Statistics.indentX, yoffset=offsetY } ) )
		end
		
		xOff = Statistics.offsetX + Statistics.objIndent + Statistics.indentX
		
		local currObjective = TextBox:new( 
										{ 
											body=v.title, color=colour, xoffset=xOff, yoffset=offsetY,
											width=Statistics.guiWidth-xOff, height=200
										} )
		local dimX, dimY = currObjective.text:getDim()
		currObjective.yoffset = currObjective.yoffset - dimY
		objectivesGUI:addComponent( currObjective )
		offsetY = offsetY + dimY
	end
	
	objectivesGUI:addPlacementHandler( SimplePlacer )
	
	return objectivesGUI
end

function Statistics.getGUIWidth()
	return Statistics.guiWidth
end

function Statistics.setGUIWidth( newWidth )
	Statistics.guiWidth = newWidth
end

return Statistics