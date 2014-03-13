local GUI = require "gui/GUI"
local Button = require "gui/component/Button"
local Slider = require "gui/component/Slider"
local Checkbox = require "gui/component/Checkbox"
local AnchorPlacer = require "gui/placement/AnchorPlacer"
local TextLabel = require "gui/component/TextLabel"
local TextBox = require "gui/component/TextBox"
local Image = require "gui/component/Image"

TutorialMenu = {}
local tutImage = nil
local mainGameTutButtons = {}
local tBox = nil
local headline = nil
local activeImage = 1
local prevActiveImage = -1
function TutorialMenu:new(o, menuState)
    o = o or {}
    o.gui = GUI:new()
	tutGUI = GUI:new()

	--back to main menu
    o.gui:addComponent(Button:new({
                                    matReleased = "assets/texture/ui/back-button-release.material",
                                    matPressed = "assets/texture/ui/back-button-press.material",
                                    matHover = "assets/texture/ui/back-button-hover.material",
                                    anchor="SouthWest",xoffset=0,yoffset=0, onClick = function() if tutImage then tutImage:destroy() end TutorialMenu:DestroyButtons(o) menuState.goMain() end }))
									
	--basic gameplay tutorials
	o.gui:addComponent(Button:new({
                                  	matReleased = "assets/texture/ui/tutorial/basic-gameplay-release.material",
									matPressed = "assets/texture/ui/tutorial/basic-gameplay-press.material",
									matHover = "assets/texture/ui/tutorial/basic-gameplay-hover.material",
                                    anchor="NorthWest",xoffset=0,yoffset=0, onClick = function() if tutImage then tutImage:destroy() end TutorialMenu:AddButtons(o, 2) end }))

	--prep screen tutorials
	o.gui:addComponent(Button:new({
                                    matReleased = "assets/texture/ui/tutorial/preparation-release.material",
                                    matPressed = "assets/texture/ui/tutorial/preparation-press.material",
                                    matHover = "assets/texture/ui/tutorial/preparation-hover.material",
                                    anchor="NorthWest",xoffset=0,yoffset=0, onClick = function() if tutImage then tutImage:destroy() end TutorialMenu:AddButtons(o, 0) end }))
	
	--gameplay tutorials
	o.gui:addComponent(Button:new({
                                    matReleased = "assets/texture/ui/tutorial/advanced-gameplay-release.material",
                                    matPressed = "assets/texture/ui/tutorial/advanced-gameplay-press.material",
                                    matHover = "assets/texture/ui/tutorial/advanced-gameplay-hover.material",
                                    anchor="NorthWest",xoffset=0,yoffset=0, onClick = function() if tutImage then tutImage:destroy() end TutorialMenu:AddButtons(o, 1) end }))

	--keybind image
	o.gui:addComponent(Button:new({
                                    matReleased = "assets/texture/ui/tutorial/keybinding-release.material",
                                    matPressed = "assets/texture/ui/tutorial/keybinding-press.material",
                                    matHover = "assets/texture/ui/tutorial/keybinding-hover.material",
                                    anchor="NorthWest",xoffset=0,yoffset=0, onClick = function()  if tutImage then tutImage:destroy() end TutorialMenu:AddButtons(o, -1) TutorialMenu:SetImage(0, o) end }))

	
	tBox = TextBox:new({ignoreConstrict = true, body=[[]], width=700, height=900, xoffset = 500, yoffset= 350, anchor="CenterHorizontal"})
	headline = TextBox:new({ body=[[]], width=200, height=100, xoffset = 500, yoffset= -250, anchor="NorthWest"})
	o.gui:addComponent(tBox)
	o.gui:addComponent(headline)



    o.gui:addPlacementHandler( AnchorPlacer )

    setmetatable( o, self )
    self.__index = self
    return o
end

function TutorialMenu:DestroyButtons(o)
	if mainGameTutButtons.policeSpawnButton then
		o.gui:removeComponent(mainGameTutButtons.policeSpawnButton)
		o.gui:removeComponent(mainGameTutButtons.buyInstruct)
		o.gui:removeComponent(mainGameTutButtons.hostileSpawn)
		o.gui:removeComponent(mainGameTutButtons.friendlySpawn)
		o.gui:removeComponent(mainGameTutButtons.escortPath)
		
		mainGameTutButtons.policeSpawnButton:destroy()
		mainGameTutButtons.buyInstruct:destroy()
		mainGameTutButtons.hostileSpawn:destroy()
		mainGameTutButtons.friendlySpawn:destroy()
		mainGameTutButtons.escortPath:destroy()
		
		mainGameTutButtons.policeSpawnButton = nil
		mainGameTutButtons.buyInstruct = nil
		mainGameTutButtons.hostileSpawn = nil
		mainGameTutButtons.friendlySpawn = nil
		mainGameTutButtons.escortPath = nil

		o.gui:removeComponent(mainGameTutButtons.forward)
		mainGameTutButtons.forward:destroy()
		mainGameTutButtons.forward = nil

		o.gui:removeComponent(mainGameTutButtons.back)
		mainGameTutButtons.back:destroy()
		mainGameTutButtons.back = nil
		
	end

	if mainGameTutButtons.moodsButton then
		
		o.gui:removeComponent(mainGameTutButtons.objectivesButton)
		o.gui:removeComponent(mainGameTutButtons.selectingPoliceButton)
		o.gui:removeComponent(mainGameTutButtons.formationsButton)
		o.gui:removeComponent(mainGameTutButtons.stancesButton)
		o.gui:removeComponent(mainGameTutButtons.specialAbilitiesButton)
		o.gui:removeComponent(mainGameTutButtons.moodsButton)

		mainGameTutButtons.objectivesButton:destroy()
		mainGameTutButtons.selectingPoliceButton:destroy()
		mainGameTutButtons.formationsButton:destroy()
		mainGameTutButtons.stancesButton:destroy()
		mainGameTutButtons.specialAbilitiesButton:destroy()
		mainGameTutButtons.moodsButton:destroy()

		mainGameTutButtons.objectivesButton = nil
		mainGameTutButtons.selectingPoliceButton = nil
		mainGameTutButtons.specialAbilitiesButton = nil
		mainGameTutButtons.stancesButton = nil
		mainGameTutButtons.escortPath = nil
		mainGameTutButtons.moodsButton = nil

		o.gui:removeComponent(mainGameTutButtons.forward)
		mainGameTutButtons.forward:destroy()
		mainGameTutButtons.forward = nil

		o.gui:removeComponent(mainGameTutButtons.back)
		mainGameTutButtons.back:destroy()
		mainGameTutButtons.back = nil
	end
end

function TutorialMenu:SetImage(id, o)
	if tutImage then
		tutImage:destroy()
	end

	if id == 0 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/keybindTut.material" }
		tBox:setText([[]])
		headline:setText([[Keybindings]])
	elseif id == 1 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/policeSpawnTut.material" }
		tBox:setText([[The light blue squares with "SPAWN" written in them shows where it is possible to place your newly bought police.]])
		headline:setText([[Police spawn]])
	elseif id == 2 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/buyingUnitsTut.material" }
		tBox:setText([[The purchasing interface for buying police units consists of three different windows. The top window shows how much cash you have left to spend on units. 
		
		The second window shows what units can be purchased. Selecting a unit in this window will show its cost and stats. While a unit is selected in this window, clicking anywhere within a defined police spawn area will purchase that unit.
		
		The third window shows what units have been purchased. Clicking a unit in this window will highlight it in the world and clicking a unit in the world will highlight it in the window. Pressing "Remove" while a unit is highlighted in this window will remove the unit and refund the cost of the unit.]])
		headline:setText([[Buying units]])
	elseif id == 3 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/hostileSpawnTut.material" }
		tBox:setText([[The red square with a "!" marks a potential spawn point for hostile rioters.]])
		headline:setText([[Hostile spawn]])
	elseif id == 4 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/friendlySpawnTut.material" }
		tBox:setText([[The green square with "START" marks the spawn point for friendly rioters that you must protect.]])
		headline:setText([[Friendly spawn]])
	elseif id == 5 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/friendlyPathTut.material" }
		tBox:setText([[The green arrows indicate the path which the friendly rioters will move along as long as they are not disturbed.]])
		headline:setText([[Escort path]])
	elseif id == 6 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/stanceTut.material" }
		tBox:setText([[There are three different stances available. With one or more units selected, left click either of the three icons in the top right corner to toggle the stance for all the selected units.
		
		Red fist: Aggressive stance, selecting this stance will make your units attack any rioter in their vicinity.
		
		Yellow shield: Defensive stance, selecting this stance will make your units attack any rioters which attack you. This stance will also make rioters take a longer time to get angry.
		
		Green man: Passive stance, selecting this stance will make your units completely passive, ignoring any attacks or passing rioters.]])
		headline:setText([[Stances]])
	elseif id == 7 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/tearGasTut.material" }
		tBox:setText([[ Tear gas is a targeted area of effect attack only available for tear gas police. Pressing it will active a reticule which shows the area affected by firing the tear gas. Left clicking while the reticule is active will fire. Firing consumes stamina. Tear gas will make rioters angry but also makes them flee the area of effect.]])
		headline:setText([[Tear gas]])
	elseif id == 8 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/specialAbilityTut.material" }
		tBox:setText([[When a squad (or several) is seleceted, all the available special abilities for the selected units will be shown in the middle panel of the squad interface.

		Attack: Pressing attack and then left clicking a rioter group will make all the selected squads attack that group.

		Sprint: Pressing sprint will toggle sprinting on/off, making the unit sprint to their current target Sprinting consumes stamina.
		
		Halt: Pressing halt will make all selected squads stop at their current position.]])
		headline:setText([[Shared abilities]])
	elseif id == 9 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/overviewTut.material" }
		tBox:setText([[THIS IS AN OVERVIEW DESCRIPTION]])
		headline:setText([[Overview]])
	elseif id == 10 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/moodTut.material" }
		tBox:setText([[Depending on your actions, rioters will become angrier or calm themselves. This can be seen by holding the M button, giving the group an outline where the color shows their mood.
		
		Light blue: Neutral, this rioter is not angry or aggresive.
	
		Orange: Agitated, this rioter is starting to get angry.
	
		Red: Angry, this rioter is angry and will attack any hostile rioters or police within sight.
	
		White: Broken, this rioter has been broken and will retreat, leaving the map.]])
		headline:setText([[Moods]])
	elseif id == 11 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/cameraTut.material" }
		tBox:setText([[YOU CAN MOVE YOUR CAMERA HURR HURR DURR]])
		headline:setText([[Camera controls]])
	elseif id == 12 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/selectionTut.material" }
		tBox:setText([[Selecting units is done by either left clicking a unit or left clicking and holding to drag a selection box. Every squad within the box will be selected.
		
		Right clicking the ground with squads (or a single squad) selected will make them move to that position.
		
		Left clicking on the ground with a squads (or a single squad) selected will deselect all the selected units.]])
		headline:setText([[Basic controls]])
	elseif id == 13 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/formationTut.material" }
		tBox:setText([[With one or more units selected, right click and drag to place all the selected units in formation. The direction which you drag the mouse will impact the direction of the formation. Every unit's position is indicated by a green circle on the ground.
		
		Three different formations are available; line, semi-circle and circle formation. They can be find on the bottom right of the squad interface. Toggling the formation toggles it for all the selected units.]])
		headline:setText([[Formations]])
	elseif id == 14 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/objectivesTut.material" }
		tBox:setText([[The top left corner shows the objectives for the current level. Every objective which is not marked with "Optional" needs to be fulfilled to complete the level. Completing optional objectives may however grant bonuses such as more police units or less rioters spawning.]])
		headline:setText([[Objectives]])
	end 

	activeImage = id
	if id >= 1 and id <= 5 then
		tBox.yoffset= 350
	elseif id > 5 and id < 11 then
		tBox.yoffset = 130
	elseif id >= 11 then
		tBox.yoffset= 370
	else
	end

	if tutImage then
		tutImage:setPosition(500, 30)
	end
end

function TutorialMenu:AddButtons(tm, index)
	local x = 200
	local y = -342.5

	if mainGameTutButtons.policeSpawnButton then
		tm.gui:removeComponent(mainGameTutButtons.policeSpawnButton)
		tm.gui:removeComponent(mainGameTutButtons.buyInstruct)
		tm.gui:removeComponent(mainGameTutButtons.hostileSpawn)
		tm.gui:removeComponent(mainGameTutButtons.friendlySpawn)
		tm.gui:removeComponent(mainGameTutButtons.escortPath)
		
		mainGameTutButtons.policeSpawnButton:destroy()
		mainGameTutButtons.buyInstruct:destroy()
		mainGameTutButtons.hostileSpawn:destroy()
		mainGameTutButtons.friendlySpawn:destroy()
		mainGameTutButtons.escortPath:destroy()
		
		mainGameTutButtons.policeSpawnButton = nil
		mainGameTutButtons.buyInstruct = nil
		mainGameTutButtons.hostileSpawn = nil
		mainGameTutButtons.friendlySpawn = nil
		mainGameTutButtons.escortPath = nil

		tm.gui:removeComponent(mainGameTutButtons.forward)
		mainGameTutButtons.forward:destroy()
		mainGameTutButtons.forward = nil

		tm.gui:removeComponent(mainGameTutButtons.back)
		mainGameTutButtons.back:destroy()
		mainGameTutButtons.back = nil
		
	end

	if mainGameTutButtons.moodsButton then
		tm.gui:removeComponent(mainGameTutButtons.tearGasButton)
		tm.gui:removeComponent(mainGameTutButtons.stancesButton)
		tm.gui:removeComponent(mainGameTutButtons.specialAbilitiesButton)
		tm.gui:removeComponent(mainGameTutButtons.moodsButton)
		tm.gui:removeComponent(mainGameTutButtons.overviewButton)

		

		mainGameTutButtons.tearGasButton:destroy()
		mainGameTutButtons.stancesButton:destroy()
		mainGameTutButtons.specialAbilitiesButton:destroy()
		mainGameTutButtons.moodsButton:destroy()
		mainGameTutButtons.overviewButton:destroy()

		mainGameTutButtons.tearGasButton = nil
		mainGameTutButtons.specialAbilitiesButton = nil
		mainGameTutButtons.stancesButton = nil
		mainGameTutButtons.moodsButton = nil
		mainGameTutButtons.overviewButton = nil

		tm.gui:removeComponent(mainGameTutButtons.forward)
		mainGameTutButtons.forward:destroy()
		mainGameTutButtons.forward = nil

		tm.gui:removeComponent(mainGameTutButtons.back)
		mainGameTutButtons.back:destroy()
		mainGameTutButtons.back = nil
	end

	if mainGameTutButtons.selectingPoliceButton then
		tm.gui:removeComponent(mainGameTutButtons.cameraControlsButton)
		tm.gui:removeComponent(mainGameTutButtons.selectingPoliceButton)
		tm.gui:removeComponent(mainGameTutButtons.formationsButton)
		tm.gui:removeComponent(mainGameTutButtons.objectivesButton)

		mainGameTutButtons.cameraControlsButton:destroy()
		mainGameTutButtons.selectingPoliceButton:destroy()
		mainGameTutButtons.formationsButton:destroy()
		mainGameTutButtons.objectivesButton:destroy()

		mainGameTutButtons.cameraControlsButton = nil
		mainGameTutButtons.selectingPoliceButton = nil
		mainGameTutButtons.formationsButton = nil
		mainGameTutButtons.objectivesButton = nil

		tm.gui:removeComponent(mainGameTutButtons.forward)
		mainGameTutButtons.forward:destroy()
		mainGameTutButtons.forward = nil

		tm.gui:removeComponent(mainGameTutButtons.back)
		mainGameTutButtons.back:destroy()
		mainGameTutButtons.back = nil
	end
		
	if index == 0 then
		TutorialMenu:SetImage(1)

		mainGameTutButtons.policeSpawnButton = Button:new({
											matReleased = "assets/texture/ui/tutorial/police-spawn-release.material",
											matPressed = "assets/texture/ui/tutorial/police-spawn-press.material",
											matHover = "assets/texture/ui/tutorial/police-spawn-hover.material",
											anchor="NorthWest",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(1, o) end })

		mainGameTutButtons.buyInstruct = Button:new({
											matReleased = "assets/texture/ui/tutorial/buying-units-release.material",
											matPressed = "assets/texture/ui/tutorial/buying-units-press.material",
											matHover = "assets/texture/ui/tutorial/buying-units-hover.material",
											anchor="NorthWest",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(2, o) end })

		mainGameTutButtons.hostileSpawn = Button:new({
											matReleased = "assets/texture/ui/tutorial/hostile-spawn-release.material",
											matPressed = "assets/texture/ui/tutorial/hostile-spawn-press.material",
											matHover = "assets/texture/ui/tutorial/hostile-spawn-hover.material",
											anchor="NorthWest",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(3, o) end })

		mainGameTutButtons.friendlySpawn = Button:new({
											matReleased = "assets/texture/ui/tutorial/friendly-spawn-release.material",
											matPressed = "assets/texture/ui/tutorial/friendly-spawn-press.material",
											matHover = "assets/texture/ui/tutorial/friendly-spawn-hover.material",
											anchor="NorthWest",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(4, o) end })

		mainGameTutButtons.escortPath = Button:new({
											matReleased = "assets/texture/ui/tutorial/escort-release.material",
											matPressed = "assets/texture/ui/tutorial/escort-press.material",
											matHover = "assets/texture/ui/tutorial/escort-hover.material",
											anchor="NorthWest",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(5, o) end })
	
		tm.gui:addComponent(mainGameTutButtons.policeSpawnButton)
		tm.gui:addComponent(mainGameTutButtons.hostileSpawn)
		tm.gui:addComponent(mainGameTutButtons.friendlySpawn)
		tm.gui:addComponent(mainGameTutButtons.escortPath)
		tm.gui:addComponent(mainGameTutButtons.buyInstruct)
	elseif index == 1 then
		TutorialMenu:SetImage(6)

		mainGameTutButtons.stancesButton = Button:new({
											matReleased = "assets/texture/ui/tutorial/stances-release.material",
											matPressed = "assets/texture/ui/tutorial/stances-press.material",
											matHover = "assets/texture/ui/tutorial/stances-hover.material",
											anchor="NorthWest",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(6, o) end })

		mainGameTutButtons.tearGasButton = Button:new({
											matReleased = "assets/texture/ui/tutorial/teargas-release.material",
											matPressed = "assets/texture/ui/tutorial/teargas-press.material",
											matHover = "assets/texture/ui/tutorial/teargas-hover.material",
											anchor="NorthWest",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(7, o) end })

		mainGameTutButtons.specialAbilitiesButton = Button:new({
											matReleased = "assets/texture/ui/tutorial/shared-abilities-release.material",
											matPressed = "assets/texture/ui/tutorial/shared-abilities-press.material",
											matHover = "assets/texture/ui/tutorial/shared-abilities-hover.material",
											anchor="NorthWest",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(8, o) end })

		mainGameTutButtons.overviewButton = Button:new({
											matReleased = "assets/texture/ui/tutorial/overview-release.material",
											matPressed = "assets/texture/ui/tutorial/overview-press.material",
											matHover = "assets/texture/ui/tutorial/overview-hover.material",
											anchor="NorthWest",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(9, o) end })
		
		mainGameTutButtons.moodsButton = Button:new({
											matReleased = "assets/texture/ui/tutorial/moods-release.material",
											matPressed = "assets/texture/ui/tutorial/moods-press.material",
											matHover = "assets/texture/ui/tutorial/moods-hover.material",
											anchor="NorthWest",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(10, o) end })
		
		tm.gui:addComponent(mainGameTutButtons.stancesButton)
		tm.gui:addComponent(mainGameTutButtons.tearGasButton)
		tm.gui:addComponent(mainGameTutButtons.specialAbilitiesButton)
		tm.gui:addComponent(mainGameTutButtons.overviewButton)
		tm.gui:addComponent(mainGameTutButtons.moodsButton)
	elseif index == 2 then
		TutorialMenu:SetImage(11)

		mainGameTutButtons.cameraControlsButton = Button:new({
										matReleased = "assets/texture/ui/tutorial/camera-controls-release.material",
										matPressed = "assets/texture/ui/tutorial/camera-controls-press.material",
										matHover = "assets/texture/ui/tutorial/camera-controls-hover.material",
										anchor="NorthWest",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(11, o) end })


		mainGameTutButtons.selectingPoliceButton = Button:new({
										matReleased = "assets/texture/ui/tutorial/basic-controls-release.material",
										matPressed = "assets/texture/ui/tutorial/basic-controls-press.material",
										matHover = "assets/texture/ui/tutorial/basic-controls-hover.material",
										anchor="NorthWest",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(12, o) end })

		
		mainGameTutButtons.formationsButton = Button:new({
										matReleased = "assets/texture/ui/tutorial/formation-release.material",
										matPressed = "assets/texture/ui/tutorial/formation-press.material",
										matHover = "assets/texture/ui/tutorial/formation-hover.material",
										anchor="NorthWest",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(13, o) end })

		mainGameTutButtons.objectivesButton = Button:new({
											matReleased = "assets/texture/ui/tutorial/objective-release.material",
											matPressed = "assets/texture/ui/tutorial/objective-press.material",
											matHover = "assets/texture/ui/tutorial/objective-hover.material",
											anchor="NorthWest",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(14, o) end })

		tm.gui:addComponent(mainGameTutButtons.cameraControlsButton)
		tm.gui:addComponent(mainGameTutButtons.selectingPoliceButton)
		tm.gui:addComponent(mainGameTutButtons.formationsButton)
		tm.gui:addComponent(mainGameTutButtons.objectivesButton)
	end

	if index == 0 or index == 1  or index == 2 then
		mainGameTutButtons.forward = Button:new({
												matReleased = "assets/texture/ui/tutorial/next-release.material",
												matPressed = "assets/texture/ui/tutorial/next-press.material",
												matHover = "assets/texture/ui/tutorial/next-hover.material",
												anchor="East",xoffset = 0,yoffset= -100, onClick = function() activeImage = (activeImage + 1) % 15 TutorialMenu:SetImage(activeImage, o) end })
	
		tm.gui:addComponent(mainGameTutButtons.forward )

		mainGameTutButtons.back = Button:new({
												matReleased = "assets/texture/ui/tutorial/prev-release.material",
												matPressed = "assets/texture/ui/tutorial/prev-press.material",
												matHover = "assets/texture/ui/tutorial/prev-hover.material",
												anchor="East" ,xoffset = 0 ,yoffset= -100, onClick = function() activeImage = (activeImage - 1) % 15 TutorialMenu:SetImage(activeImage, o) end })
	
		tm.gui:addComponent(mainGameTutButtons.back )
	end
end

function TutorialMenu:update(delta)
end

function TutorialMenu:destroy()



    self.gui:destroy() 
end

return TutorialMenu
