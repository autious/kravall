local GUIComponent = require "gui/component/GUIComponent"
local GUI = require "gui/GUI"

local ScrollBar = require "gui/component/ScrollBar"
local Image = require "gui/component/Image"
local text = require"factories/text"
local AABB = require"collision/aabb"

local SimplePlacer = require "gui/placement/SimplePlacer"

local TextSelectList = 
                {
                    x=0,
                    y=0,
                    xoffset=0,
                    yoffset=0,
                    width=0,
                    height=0,
                    padding=4,
                    show=true,
                    elements = { {name = "default"}, {name = "default2"} },
                    onSelect = function() end
                }

function TextSelectList:new(o)
    o = o or {}
    setmetatable( o,self )
    self.__index = self

    o.onClick = o.onClick or function()  end

	o.minScroll = 1
	o.maxScroll = 1
	o.startOffset = 1
    o.textElements = {}
	
	o.scrollBar = ScrollBar:new( { 
									width=3, height=o.height, xoffset=0, yoffset=0,
									onScrollUp=function()
													if o.startOffset > o.minScroll then
														o.startOffset = o.startOffset - 1 
														o:updateList()
													end
												end, 
									onScrollDown=function()
													if o.startOffset < o.maxScroll then
														o.startOffset = o.startOffset + 1 
														o:updateList()
													end
												end
								} )
	o.scrollBar.xoffset = o.width - o.scrollBar.width - 1													

    o.GUIComponent = GUIComponent:new
                                        {
                                            x=o.x,
                                            y=o.y,
                                            width=o.width,
                                            height=o.height,
                                            onPress = function( x, y )  o:onPress( x, y ) end,
                                            onRelease = function() o:onRelease() end,
                                            onEnter = function() o:onEnter() end,
                                            onExit = function() o:onExit() end,
											toolTip = o.toolTip
                                        }

    o:setShow( o.show )
    o:updateList()

    return o
end

function TextSelectList:setShow( flag )
    self.show = flag

    self.GUIComponent:setShow( self.show )
	self.scrollBar:setShow( self.show )

    for _,v in pairs( self.textElements ) do
        v:show( self.show )
    end
end

function TextSelectList:updateList()
    for _,v in pairs( self.textElements ) do
        v:destroy()        
        v = nil
    end

    self.textElements = {}
     
    local heightOffset = 0
	local index = 0
	
    for k,v in pairs( self.elements ) do
		index = index + 1
		if index >= self.startOffset then
			local text1 = text( self.x, self.y, v.name )
			text1:show( self.show )
			local width, height = text1:getDim()
			
			if heightOffset + height <= self.height then
				text1:setPosition( self.x, self.y + heightOffset )
				heightOffset = heightOffset + height + self.padding

				text1.object = v
				self.textElements[#(self.textElements)+1] = text1
			else
				text1:destroy()
			end
		end
    end
	
	self.maxScroll = index
	
	if self.minScroll ~= 1 or self.maxScroll > #self.textElements then
		self.scrollBar:setShow( true )
	else
		self.scrollBar:setShow( false )
	end

    self:colorActive()
end

function TextSelectList:render()

end

function TextSelectList:setPosition( x, y )
    self.x = x
    self.y = y
    self.GUIComponent.x = self.x
    self.GUIComponent.y = self.y
	self.scrollBar:setPosition( self.x, self.y )
    self:updateList()
end

function TextSelectList:onPress( x, y ) 
    if self.show then
        local newActive = nil

        for k,v in pairs( self.textElements ) do
            local width, height = v:getDim()
            if AABB:new( { v.x, v.y, width, height } ):collides( x, y ) then
                newActive = v.object
            end
        end

        if newActive ~= nil and newActive ~= self.activeObject then
            self:setActive( newActive )
        end
    end
end

function TextSelectList:setActive( object )
    self.activeObject = object
    self.onSelect( object )
    self:colorActive()
end

function TextSelectList:colorActive()
    for k,v in pairs( self.textElements ) do
        if v.object == self.activeObject then
            v:setColor( "#f3ff2c" )
        else
            v:setColor( "#ffffff" )
        end
    end
end

function TextSelectList:onRelease()
end

function TextSelectList:onEnter()
end

function TextSelectList:onExit()
end

function TextSelectList:destroy()
    for _,v in pairs( self.textElements ) do
        v:destroy()        
        v = nil
    end

    self.textElements = nil
	self.scrollBar:destroy()
	self.scrollBar = nil
    self.GUIComponent:destroy()
end

return TextSelectList
