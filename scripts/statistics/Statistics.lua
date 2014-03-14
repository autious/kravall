local GUI = require "gui/GUI"

local TextLabel = require "gui/component/TextLabel"
local TextBox = require "gui/component/TextBox"

local SimplePlacer = require "gui/placement/SimplePlacer"

local Statistics = 
                {
					categories={},
					rank="Play tester",
					playTime=0,
					totalScore=0,
					achievedScore=0,
					guiWidth = 500,
					offsetX = 0,
					indentX = 30,
					newSectionY = 50,
					newLineY = 20,
					objIndent = 100,
					listOrder={}
                }
				
local objectiveScore = 100

function Statistics.prepare()
	Statistics.achievedScore = 0
	Statistics.totalScore = 0
	
	for i,v in pairs( Statistics.categories[ "objectives" ] ) do
		if v.state == "success" then
			Statistics.achievedScore = Statistics.achievedScore + objectiveScore
			Statistics.totalScore = Statistics.totalScore + objectiveScore
		elseif v.state == "fail" then
			Statistics.totalScore = Statistics.totalScore + objectiveScore
		end
	end
	
	for i,v in pairs( Statistics.categories ) do
		if i ~= "objectives" then
			for j,w in pairs( Statistics.categories[ i ] ) do
				if w.achievedResult == w.achievedResult then -- protect against NaN
					Statistics.achievedScore = Statistics.achievedScore + w.achievedResult
				end
				if w.maxResult == w.maxResult then -- protect against NaN
					Statistics.totalScore = Statistics.totalScore + w.maxResult
				end
			end
		end
	end
	
	local percentage = Statistics.achievedScore / Statistics.totalScore
	
	if percentage <= 0.1 then
		Statistics.rank = "1. (" .. Statistics.achievedScore .. "/" .. Statistics.totalScore .. ")" -- First-Time Player
	elseif percentage <= 0.4 then
		Statistics.rank = "2. (" .. Statistics.achievedScore .. "/" .. Statistics.totalScore .. ")" -- Beginner Player
	elseif percentage <= 0.6 then
		Statistics.rank = "3. (" .. Statistics.achievedScore .. "/" .. Statistics.totalScore .. ")" -- Play Tester
	elseif percentage <= 0.8 then
		Statistics.rank = "4. (" .. Statistics.achievedScore .. "/" .. Statistics.totalScore .. ")" -- Developer
	else
		Statistics.rank = "5. (" .. Statistics.achievedScore .. "/" .. Statistics.totalScore .. ")"	-- Pro developer
	end
end
				
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
	Statistics.categories = {}
end

function Statistics.getAllAsSubGUI( xoffset, yoffset )
	subGUI = GUI:new( {
							width=Statistics.guiWidth, xoffset=xOffset, yoffset=yOffset
					} )
	local currGUI = Statistics.getObjectivesAsSubGUI( xoffset, yoffset )
	
	print( "Objectives gui height: " .. currGUI.height )
	subGUI:addComponent( currGUI )
	yoffset = yoffset + currGUI.height
	
	if #Statistics.listOrder == 0 then	
		for i,v in pairs( Statistics.categories ) do
			if i ~= "objectives" then
				yoffset = yoffset + Statistics.newLineY
				
				currGUI = Statistics.getCategoryAsSubGUI( i, xoffset, yoffset )
				subGUI:addComponent( currGUI )
				print( i .. " gui height: " .. currGUI.height )
				yoffset = yoffset + currGUI.height
			end
		end
	else
		for i,v in pairs( Statistics.listOrder ) do
			if v ~= "objectives" then
				yoffset = yoffset + Statistics.newLineY
				
				currGUI = Statistics.getCategoryAsSubGUI( v, xoffset, yoffset )
				subGUI:addComponent( currGUI )
				yoffset = yoffset + currGUI.height
			end
		end
	end
	subGUI.height = yoffset
	subGUI:addPlacementHandler( SimplePlacer )
	
	return subGUI
end

function Statistics.getCategoryAsSubGUI( name, xOffset, yOffset )
	if Statistics.categories[ name ] then
		local subGUI = GUI:new( {
									width=Statistics.guiWidth, xoffset=xOffset, yoffset=yOffset
								} )
	
		local offsetY = 0
		subGUI:addComponent( TextLabel:new( { label=name, xoffset=offsetX, yoffset=offsetY } ) )
		offsetY = offsetY + Statistics.newLineY
		
		for i,v in pairs( Statistics.categories[ name ] ) do
			local currStat = TextLabel:new( { label=v.resultTitle, yoffset=offsetY  } )
			local dimX, dimY = currStat.text:getDim()
			currStat.xoffset = subGUI.width - dimX - xOffset
			subGUI:addComponent( currStat )
			
			local xOff = Statistics.offsetX+Statistics.indentX
			currStat = TextBox:new( { 
										body=v.title, xoffset=xOff, yoffset=offsetY,
										width=Statistics.guiWidth - xOff - dimX, height=200
									} )
			
			dimX, dimY = currStat.text:getDim()
			currStat.yoffset = currStat.yoffset - dimY
			subGUI:addComponent( currStat )
			
			offsetY = offsetY + dimY
		end
		
		subGUI.height = offsetY
		subGUI:addPlacementHandler( SimplePlacer )
		
		return subGUI
	else
		core.log.error( "Lua Statistics: No category exists named " .. name )
	end
	
	return nil
end

function Statistics.getObjectivesAsSubGUI( xOffset, yOffset )
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

		if v.state == "fail" then
			colour = {1,0,0,1}
			objectivesGUI:addComponent( TextLabel:new( { label="[FAIL]", color=colour, xoffset=Statistics.offsetX+Statistics.indentX, yoffset=offsetY  } ) )
		elseif v.state == "success" then
			colour = {0,1,0,1}
			objectivesGUI:addComponent( TextLabel:new( { label="[SUCCESS]", color=colour, xoffset=Statistics.offsetX+Statistics.indentX, yoffset=offsetY } ) )
		else
			objectivesGUI:addComponent( TextLabel:new( { label="[UNKNOWN]", color=colour, xoffset=Statistics.offsetX+Statistics.indentX, yoffset=offsetY } ) )
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
	
	objectivesGUI.height = offsetY
	objectivesGUI:addPlacementHandler( SimplePlacer )
	
	return objectivesGUI
end

function Statistics.setListOrder( newListOrder )
	Statistics.listOrder = newListOrder
end

function Statistics.getGUIWidth()
	return Statistics.guiWidth
end

function Statistics.setGUIWidth( newWidth )
	Statistics.guiWidth = newWidth
end

return Statistics