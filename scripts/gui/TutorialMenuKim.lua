local GUI = require "gui/GUI"

--local TextLabel = require "gui/component/TextLabel"
local TextBox = require "gui/component/TextBox"
local Image = require "gui/component/Image"
local Button = require "gui/component/Button"

local AnchorPlacer = require "gui/placement/AnchorPlacer"
local SimplePlacer = require "gui/placement/SimplePlacer"

local Util = require "gui/placement/util"

local TutorialMenu = 
					{}

local tutPageWidth = 640
local tutPageHeight = 500

-- TODO: Return the Preparation Sub-GUI and put the tutorial Sub-GUIs in the table
function TutorialMenu:makePreparationTable()
	table.insert( self.prepTable, Checkbox:new( 
												{
													doStateSwitchOnPress=false,
													anchor="North",
													checked=false,
													xoffset=0,
													yoffset=0,
													matOpen="assets/texture/ui/tutorial/police-spawn-release.material",
													matSelected="assets/texture/ui/tutorial/police-spawn-press.material",
													matHoverOpen="assets/texture/ui/tutorial/police-spawn-hover.material",
													matHoverSelected="assets/texture/ui/tutorial/police-spawn-hover.material",
													--onClick=o.showSubmenu( police-spawn )
												} ) )
	table.insert( self.prepTable, Checkbox:new( 
												{
													doStateSwitchOnPress=false,
													anchor="North",
													checked=false,
													xoffset=0,
													yoffset=0,
													matOpen="assets/texture/ui/tutorial/buying-units-release.material",
													matSelected="assets/texture/ui/tutorial/buying-units-press.material",
													matHoverOpen="assets/texture/ui/tutorial/buying-units-hover.material",
													matHoverSelected="assets/texture/ui/tutorial/buying-units-hover.material",
													--onClick=o.showSubmenu( buying-units )
												} ) )
	table.insert( self.prepTable, Checkbox:new( 
												{
													doStateSwitchOnPress=false,
													anchor="North",
													checked=false,
													xoffset=0,
													yoffset=0,
													matOpen="assets/texture/ui/tutorial/hostile-spawn-release.material",
													matSelected="assets/texture/ui/tutorial/hostile-spawn-press.material",
													matHoverOpen="assets/texture/ui/tutorial/hostile-spawn-hover.material",
													matHoverSelected="assets/texture/ui/tutorial/hostile-spawn-hover.material",
													--onClick=o.showSubmenu( hostile-spawn )
												} ) )
	table.insert( self.prepTable, Checkbox:new( 
												{
													doStateSwitchOnPress=false,
													anchor="North",
													checked=false,
													xoffset=0,
													yoffset=0,
													matOpen="assets/texture/ui/tutorial/friendly-spawn-release.material",
													matSelected="assets/texture/ui/tutorial/friendly-spawn-press.material",
													matHoverOpen="assets/texture/ui/tutorial/friendly-spawn-hover.material",
													matHoverSelected="assets/texture/ui/tutorial/friendly-spawn-hover.material",
													--onClick=o.showSubmenu( friendly-spawn )
												} ) )
	table.insert( self.prepTable, Checkbox:new( 
												{
													doStateSwitchOnPress=false,
													anchor="North",
													checked=false,
													xoffset=0,
													yoffset=0,
													matOpen="assets/texture/ui/tutorial/escort-release-release.material",
													matSelected="assets/texture/ui/tutorial/escort-release-press.material",
													matHoverOpen="assets/texture/ui/tutorial/escort-release-hover.material",
													matHoverSelected="assets/texture/ui/tutorial/escort-release-hover.material",
													--onClick=o.showSubmenu( escort-release )
												} ) )
end					

function TutorialMenu:makeGamePlayTable()
	table.insert( self.gpTable, Checkbox:new( 
												{
													doStateSwitchOnPress=false,
													anchor="North",
													checked=false,
													xoffset=0,
													yoffset=0,
													matOpen="assets/texture/ui/tutorial/objective-release.material",
													matSelected="assets/texture/ui/tutorial/objective-press.material",
													matHoverOpen="assets/texture/ui/tutorial/objective-hover.material",
													matHoverSelected="assets/texture/ui/tutorial/objective-hover.material",
													--onClick=o.showSubmenu( objective )
												} ) )
	table.insert( self.gpTable, Checkbox:new( 
												{
													doStateSwitchOnPress=false,
													anchor="North",
													checked=false,
													xoffset=0,
													yoffset=0,
													matOpen="assets/texture/ui/tutorial/basic-controls-release.material",
													matSelected="assets/texture/ui/tutorial/basic-controls-press.material",
													matHoverOpen="assets/texture/ui/tutorial/basic-controls-hover.material",
													matHoverSelected="assets/texture/ui/tutorial/basic-controls-hover.material",
													--onClick=o.showSubmenu( basic-controls )
												} ) )
	table.insert( self.gpTable, Checkbox:new( 
												{
													doStateSwitchOnPress=false,
													anchor="North",
													checked=false,
													xoffset=0,
													yoffset=0,
													matOpen="assets/texture/ui/tutorial/formation-release.material",
													matSelected="assets/texture/ui/tutorial/formation-press.material",
													matHoverOpen="assets/texture/ui/tutorial/formation-hover.material",
													matHoverSelected="assets/texture/ui/tutorial/formation-hover.material",
													--onClick=o.showSubmenu( formation )
												} ) )
	table.insert( self.gpTable, Checkbox:new( 
												{
													doStateSwitchOnPress=false,
													anchor="North",
													checked=false,
													xoffset=0,
													yoffset=0,
													matOpen="assets/texture/ui/tutorial/stances-release.material",
													matSelected="assets/texture/ui/tutorial/stances-press.material",
													matHoverOpen="assets/texture/ui/tutorial/stances-hover.material",
													matHoverSelected="assets/texture/ui/tutorial/stances-hover.material",
													--onClick=o.showSubmenu( stances )
												} ) )
	table.insert( self.gpTable, Checkbox:new( 
												{
													doStateSwitchOnPress=false,
													anchor="North",
													checked=false,
													xoffset=0,
													yoffset=0,
													matOpen="assets/texture/ui/tutorial/special-abilities-release.material",
													matSelected="assets/texture/ui/tutorial/special-abilities-press.material",
													matHoverOpen="assets/texture/ui/tutorial/special-abilities-hover.material",
													matHoverSelected="assets/texture/ui/tutorial/special-abilities-hover.material",
													--onClick=o.showSubmenu( special-abilities )
												} ) )
	table.insert( self.gpTable, Checkbox:new( 
												{
													doStateSwitchOnPress=false,
													anchor="North",
													checked=false,
													xoffset=0,
													yoffset=0,
													matOpen="assets/texture/ui/tutorial/moods-release.material",
													matSelected="assets/texture/ui/tutorial/moods-press.material",
													matHoverOpen="assets/texture/ui/tutorial/moods-hover.material",
													matHoverSelected="assets/texture/ui/tutorial/moods-hover.material",
													--onClick=o.showSubmenu( moods )
												} ) )												
end

function TutorialMenu:makePageSubGUI( title, imageMatName, text )
{
	local pageGUI = GUI:new( { width=tutPageWidth, height=tutPageHeight } )
	
	pageGUI:addComponent( TextBox:new( { 
											anchor="NorthWest", width=tutPageWidth, height=900, body=title
										} ) )
	pageGUI:addComponent( Image:new( { anchor="NorthWest", mat="assets/texture/tutorial/" .. imageMatName } ) )
	pageGUI:addComponent( TextBox:new( { 
											anchor="NorthWest", width=tutPageWidth, height=900, body=text
										} ) )
	
	pageGUI:addPlacementHandler( AnchorPlacer )
	
	return pageGUI
}

function TutorialMenu:new(o)
    o = o or {}
    setmetatable(o,self)
    self.__index = self
	
	o.prepTable={}
	o.gpTable={}
	
	o.gui = GUI:new( { } )
	
	print( o.gui.height )
	tutPageHeight = o.gui.height
	-----------------
	o.btnCategories = GUI:new( { width=212, height=o.gui.height } )
	
	o.btnsPreparation = GUI:new( { width=212, height=o.gui.height } )
	o.makePreparationTable()
	for i,v in pairs( o.prepTable ) do
		o.btnsPreparation:addComponent( v )
	end
	
	o.btnsPreparation:addPlacementHandler( AnchorPlacer )
	-----------------
	o.btnsGamePlay = GUI:new( { width=212, height=o.gui.height } )
	o.makeGamePlayTable()
	for i,v in pairs( o.prepTable ) do
		o.btnsGamePlay:addComponent( v )
	end
	
	o.btnsGamePlay:addPlacementHandler( AnchorPlacer )
	-----------------
		
	o.btnsKeybind = GUI:new( { width=212, height=o.gui.height } )
	
	o.btnCategories:addComponent( Checkbox:new( 
												{
													doStateSwitchOnPress=false,
													checked=false,
													xoffset=0,
													yoffset=0,
													matOpen="assets/texture/ui/tutorial/preparation-release.material",
													matSelected="assets/texture/ui/tutorial/preparation-press.material",
													matHoverOpen="assets/texture/ui/tutorial/preparation-hover.material",
													matHoverSelected="assets/texture/ui/tutorial/preparation-hover.material",
													onClick=o.showSubmenu( o.btnsPreparation )
												} ) )
	o.btnCategories:addComponent( Checkbox:new( 
												{
													doStateSwitchOnPress=false,
													checked=false,
													xoffset=0,
													yoffset=0,
													matOpen="assets/texture/ui/tutorial/gameplay-release.material",
													matSelected="assets/texture/ui/tutorial/gameplay-press.material",
													matHoverOpen="assets/texture/ui/tutorial/gameplay-hover.material",
													matHoverSelected="assets/texture/ui/tutorial/gameplay-hover.material",
													onClick=o.showSubmenu( o.btnsGameplay )
												} ) )
	o.btnCategories:addComponent( Checkbox:new( 
												{
													doStateSwitchOnPress=false,
													checked=false,
													xoffset=0,
													yoffset=0,
													matOpen="assets/texture/ui/tutorial/keybinding-release.material",
													matSelected="assets/texture/ui/tutorial/keybinding-press.material",
													matHoverOpen="assets/texture/ui/tutorial/keybinding-hover.material",
													matHoverSelected="assets/texture/ui/tutorial/keybinding-hover.material",
													onClick=o.showSubmenu( o.btnsKeybind )
												} ) )
	
	o.gui:addComponent( o.btnCategories )
	
	o.gui:addPlacementHandler( SimplePlacer )
	
    return o
end

function TutorialMenu:showSubmenu( menu )
	hideAllSubmenus()
	menu:setShow( true )
end

function TutorialMenu:hideAllSubmenus()
	self.btnsPreparation:setShow( false )
end

function TutorialMenu:destroy()
    self.gui:destroy()  
    self.gui = nil
end

return TutorialMenu
