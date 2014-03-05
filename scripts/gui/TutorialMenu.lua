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
local activeImage = 1
local prevActiveImageIndex = -1
function TutorialMenu:new(o, menuState)
    o = o or {}
    o.gui = GUI:new()
	tutGUI = GUI:new()
	--back to main menu
    o.gui:addComponent(Button:new({
                                    matReleased = "assets/texture/ui/back-button-release.material",
                                    matPressed = "assets/texture/ui/back-button-press.material",
                                    matHover = "assets/texture/ui/back-button-hover.material",
                                    anchor="SouthWest",xoffset=0,yoffset=0, onClick = function() if tutImage then tutImage:destroy() end  menuState.goMain() end }))

	--prep screen tutorials
	o.gui:addComponent(Button:new({
                                    matReleased = "assets/texture/ui/back-button-release.material",
                                    matPressed = "assets/texture/ui/back-button-press.material",
                                    matHover = "assets/texture/ui/back-button-hover.material",
                                    anchor="NorthWest",xoffset=0,yoffset=0, onClick = function() if tutImage then tutImage:destroy() end TutorialMenu:AddButtons(o, 0) end }))
	
	--gameplay tutorials
	o.gui:addComponent(Button:new({
                                    matReleased = "assets/texture/ui/back-button-release.material",
                                    matPressed = "assets/texture/ui/back-button-press.material",
                                    matHover = "assets/texture/ui/back-button-hover.material",
                                    anchor="NorthWest",xoffset=0,yoffset=0, onClick = function() if tutImage then tutImage:destroy() end TutorialMenu:AddButtons(o, 1) end }))

	--keybind image
	o.gui:addComponent(Button:new({
                                    matReleased = "assets/texture/ui/back-button-release.material",
                                    matPressed = "assets/texture/ui/back-button-press.material",
                                    matHover = "assets/texture/ui/back-button-hover.material",
                                    anchor="NorthWest",xoffset=0,yoffset=0, onClick = function()  if tutImage then tutImage:destroy() end TutorialMenu:SetImage(0, o) end }))

	tBox = TextBox:new({ ignoreConstrict = true, body=[[]], width=700, height=600, xoffset = 500, yoffset= 450, anchor="Center"})
	o.gui:addComponent(tBox)

    o.gui:addPlacementHandler( AnchorPlacer )

    setmetatable( o, self )
    self.__index = self
    return o
end

function TutorialMenu:SetImage(id, o)
	if tutImage then
		tutImage:destroy()
	end

	if id == 0 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/keybindTut.material" }
		tBox:setText([[THESE ARE SOME KEYBINDINGS, WHATEVER]])
	elseif id == 1 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/policeSpawnTut.material" }
		tBox:setText([[POLICE CAN SPAWN HERE, I GUESS]])
	elseif id == 2 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/buyingUnitsTut.material" }
		tBox:setText([[WHAT'RE YA BUYIN?]])
	elseif id == 3 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/hostileSpawnTut.material" }
		tBox:setText([[ENEMIES SPAWN HERE]])
	elseif id == 4 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/friendlySpawnTut.material" }
		tBox:setText([[FRIENDLY PACIFISTS, DO NOT HURT]])
	elseif id == 5 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/friendlyPathTut.material" }
		tBox:setText([[PACIFISTS GOING ON A STROLL]])
	elseif id == 6 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/objectivesTut.material" }
		tBox:setText([[YOU MUST ACCOMPLISH THIS]])
	elseif id == 7 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/selectionTut.material" }
		tBox:setText([[PICK YOUR POISON]])
	elseif id == 8 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/formationTut.material" }
		tBox:setText([[ATTENTION]])
	elseif id == 9 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/stanceTut.material" }
		tBox:setText([[STANCES]])
	elseif id == 10 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/specialAbilityTut.material" }
		tBox:setText([[GAS IT UP]])
	elseif id == 11 then
		tutImage = Image:new { ignoreConstrict=true, mat="assets/texture/tutorial/moodTut.material" }
		tBox:setText([[ANGRY AS HELL]])
	end 
	activeImageIndex = id
	tutImage:setPosition(500, 100)
end

function TutorialMenu:AddButtons(tm, index)
	local x = 160
	local y = -50

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
		
		tm.gui:removeComponent(mainGameTutButtons.objectivesButton)
		tm.gui:removeComponent(mainGameTutButtons.selectingPoliceButton)
		tm.gui:removeComponent(mainGameTutButtons.formationsButton)
		tm.gui:removeComponent(mainGameTutButtons.stancesButton)
		tm.gui:removeComponent(mainGameTutButtons.specialAbilitiesButton)
		tm.gui:removeComponent(mainGameTutButtons.moodsButton)

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
											matReleased = "assets/texture/ui/back-button-release.material",
											matPressed = "assets/texture/ui/back-button-press.material",
											matHover = "assets/texture/ui/back-button-hover.material",
											anchor="West",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(1, o) end })

		mainGameTutButtons.buyInstruct = Button:new({
											matReleased = "assets/texture/ui/back-button-release.material",
											matPressed = "assets/texture/ui/back-button-press.material",
											matHover = "assets/texture/ui/back-button-hover.material",
											anchor="West",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(2, o) end })

		mainGameTutButtons.hostileSpawn = Button:new({
											matReleased = "assets/texture/ui/back-button-release.material",
											matPressed = "assets/texture/ui/back-button-press.material",
											matHover = "assets/texture/ui/back-button-hover.material",
											anchor="West",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(3, o) end })

		mainGameTutButtons.friendlySpawn = Button:new({
											matReleased = "assets/texture/ui/back-button-release.material",
											matPressed = "assets/texture/ui/back-button-press.material",
											matHover = "assets/texture/ui/back-button-hover.material",
											anchor="West",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(4, o) end })

		mainGameTutButtons.escortPath = Button:new({
											matReleased = "assets/texture/ui/back-button-release.material",
											matPressed = "assets/texture/ui/back-button-press.material",
											matHover = "assets/texture/ui/back-button-hover.material",
											anchor="West",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(5, o) end })
	
		tm.gui:addComponent(mainGameTutButtons.policeSpawnButton)
		tm.gui:addComponent(mainGameTutButtons.buyInstruct)
		tm.gui:addComponent(mainGameTutButtons.hostileSpawn)
		tm.gui:addComponent(mainGameTutButtons.friendlySpawn)
		tm.gui:addComponent(mainGameTutButtons.escortPath)

	elseif index == 1 then
		TutorialMenu:SetImage(6)

		mainGameTutButtons.objectivesButton = Button:new({
											matReleased = "assets/texture/ui/back-button-release.material",
											matPressed = "assets/texture/ui/back-button-press.material",
											matHover = "assets/texture/ui/back-button-hover.material",
											anchor="West",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(6, o) end })

		mainGameTutButtons.selectingPoliceButton = Button:new({
											matReleased = "assets/texture/ui/back-button-release.material",
											matPressed = "assets/texture/ui/back-button-press.material",
											matHover = "assets/texture/ui/back-button-hover.material",
											anchor="West",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(7, o) end })

		mainGameTutButtons.formationsButton = Button:new({
											matReleased = "assets/texture/ui/back-button-release.material",
											matPressed = "assets/texture/ui/back-button-press.material",
											matHover = "assets/texture/ui/back-button-hover.material",
											anchor="West",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(8, o) end })

		mainGameTutButtons.stancesButton = Button:new({
											matReleased = "assets/texture/ui/back-button-release.material",
											matPressed = "assets/texture/ui/back-button-press.material",
											matHover = "assets/texture/ui/back-button-hover.material",
											anchor="West",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(9, o) end })

		mainGameTutButtons.specialAbilitiesButton = Button:new({
											matReleased = "assets/texture/ui/back-button-release.material",
											matPressed = "assets/texture/ui/back-button-press.material",
											matHover = "assets/texture/ui/back-button-hover.material",
											anchor="West",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(10, o) end })
		
		mainGameTutButtons.moodsButton = Button:new({
											matReleased = "assets/texture/ui/back-button-release.material",
											matPressed = "assets/texture/ui/back-button-press.material",
											matHover = "assets/texture/ui/back-button-hover.material",
											anchor="West",xoffset = x,yoffset=y, onClick = function() TutorialMenu:SetImage(11, o) end })

		tm.gui:addComponent(mainGameTutButtons.objectivesButton)
		tm.gui:addComponent(mainGameTutButtons.selectingPoliceButton)
		tm.gui:addComponent(mainGameTutButtons.formationsButton)
		tm.gui:addComponent(mainGameTutButtons.stancesButton)
		tm.gui:addComponent(mainGameTutButtons.specialAbilitiesButton)
		tm.gui:addComponent(mainGameTutButtons.moodsButton)
	end

	mainGameTutButtons.forward = Button:new({
											matReleased = "assets/texture/ui/back-button-release.material",
											matPressed = "assets/texture/ui/back-button-press.material",
											matHover = "assets/texture/ui/back-button-hover.material",
											anchor="East",xoffset = 0,yoffset= -100, onClick = function() activeImage = (activeImage + 1) % 11 TutorialMenu:SetImage(activeImage, o) end })
	
	tm.gui:addComponent(mainGameTutButtons.forward )

	mainGameTutButtons.back = Button:new({
											matReleased = "assets/texture/ui/back-button-release.material",
											matPressed = "assets/texture/ui/back-button-press.material",
											matHover = "assets/texture/ui/back-button-hover.material",
											anchor="East" ,xoffset = 0 ,yoffset= -100, onClick = function() activeImage = (activeImage - 1) % 11 TutorialMenu:SetImage(activeImage, o) end })
	
	tm.gui:addComponent(mainGameTutButtons.back )
end

function TutorialMenu:update(delta)
end

function TutorialMenu:destroy()
    self.gui:destroy() 
end

return TutorialMenu
