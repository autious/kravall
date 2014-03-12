local GUIComponent = require "gui/component/GUIComponent"
local GUI = require "gui/GUI"

local Button = require "gui/component/Button"

local SimplePlacer = require "gui/placement/SimplePlacer"

local ScrollBar = 
                {
                    x=0,
                    y=0,
                    xoffset=0,
                    yoffset=0,
                    width=0,
                    height=0,
                    show=false,
					onScrollUp=function() end,
					onScrollDown=function() end
                }

function ScrollBar:new(o)
    o = o or {}
    setmetatable( o,self )
    self.__index = self
	
	o.topButton = Button:new (
							{
								xoffset=0, yoffset=0,
								matReleased="assets/texture/ui/scrollbar-top-button-release.material",
								matPressed="assets/texture/ui/scrollbar-top-button-press.material",
								matHover="assets/texture/ui/scrollbar-top-button-hover.material",
								onClick=function() o:scrollUp() end
							} )
	o.bottomButton = Button:new (
								{
									xoffset=0, yoffset=200,
									matReleased="assets/texture/ui/scrollbar-bottom-button-release.material",
									matPressed="assets/texture/ui/scrollbar-bottom-button-press.material",
									matHover="assets/texture/ui/scrollbar-bottom-button-hover.material",
									onClick=function() o:scrollDown() end
								} )
	o.bottomButton.yoffset = o.height - o.bottomButton.height - 1
	o.width = o.bottomButton.width
	
	o.gui = GUI:new( { o.width, o.height } )
	
	o.gui:addComponent( o.bottomButton )
	o.gui:addComponent( o.topButton )
	
	o.gui:addPlacementHandler( SimplePlacer )
	
	o.GUIComponent = GUIComponent:new
                                        {
                                            x=o.x,
                                            y=o.y,
                                            width=o.width,
                                            height=o.height,
                                            onPress = function( x, y ) end,
                                            onRelease = function() end,
                                            onEnter = function() end,
                                            onExit = function() end,
											toolTip = o.toolTip
                                        }
	o:setShow( o.show )
    return o
end

function ScrollBar:setShow( flag )
    self.show = flag
	self.gui:setShow( self.show )
end

function ScrollBar:scrollUp()
	self.onScrollUp()
end

function ScrollBar:scrollDown()
	self.onScrollDown()
end

function ScrollBar:setPosition( x, y )
	self.x = x + self.xoffset
	self.y = y + self.yoffset
	self.GUIComponent:setPosition( self.x , self.y )
	self.gui:setPosition( self.x, self.y )
end

function ScrollBar:destroy()
    self.topButton = nil
	self.bottomButton = nil
	self.gui:destroy()
end

return ScrollBar
