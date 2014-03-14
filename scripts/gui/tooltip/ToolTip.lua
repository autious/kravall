local text = require "factories/text"

local ToolTip = 
                {
					x=0,
					y=0,
					show=true,
					width=30,
					height=10
                }

local textOff = { x = 2, y = 0 }

function ToolTip:new( o )
    o = o or {}
    setmetatable( o, self )
    self.__index = self
	
	o.text = text( 1, 1, o.text or "Empty ToolTip", { 0, 0, 0, 1}, "assets/font/toolTip.font" )
	local textDimX, textDimY = o.text:getDim()
	o.width = textDimX + textOff.x * 2
	o.height = 24
	o.show = false
	o.text:show( false )
	
	return o
end

function ToolTip:update()
	--						  pos.x, pos.y, dim.x, dim.y, solid
	--core.draw.drawRectangle(float, float, float, float, bool)
	
	core.draw.drawRectangle( self.x, self.y, self.width, self.height, true, 0.66, 0.66, 0.66, 0.75 )
end

function ToolTip:setShow( flag )
    self.show = flag
	self.text:show(flag)
end

function ToolTip:setPosition( x, y )
	x = x or 0
	y = y or 0
	
	self.x, self.y = x, y + 20
	screenWidth, screenHeight = core.window.getSize()
	-- TODO: Insert screenWidth and screenHeight when width works
	if self.x + self.width > screenWidth then
		self.x = screenWidth - self.width
	end
	
	if self.y + self.height > screenHeight then
		self.y = screenHeight - self.height
	end
	
	self.text:setPosition( self.x + textOff.x + 2, self.y + textOff.y - 4)
end

function ToolTip:destroy()
    self.text:destroy()
end

return ToolTip