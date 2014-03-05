local ToolTipHandler = 
                {
                    toolTip = nil
                }

function ToolTipHandler:update()
	if self.toolTip ~= nil then
		self.toolTip:update()
	end
	
	
end

function ToolTipHandler:mouseMove(newX, newY) 
	if self.toolTip ~= nil then
		self.toolTip:update()
	end
end
			
function ToolTipHandler:register( newToolTip )
	if self.toolTip ~= nil then
		self:deregister( self.toolTip )
	end
	
	self.toolTip = newToolTip
	self.toolTip:setShow( true )
	self.toolTip:setPosition( core.input.mouse.getPosition() )
end

function ToolTipHandler:deregister( )
	if self.toolTip ~= nil then
		self.toolTip:setShow( false )
		self.toolTip = nil
	end
end
				
return ToolTipHandler