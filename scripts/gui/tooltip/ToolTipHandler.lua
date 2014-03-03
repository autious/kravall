local createTT = require "gui/tooltip/ToolTip"

local ToolTipHandler = 
                {
                    toolTip = nil
                }
				
ToolTipHandler.toolTip = createTT:new()

function ToolTipHandler:update()
	if self.toolTip ~= nil then
		self.toolTip.update()
	end
	
	if core.input.keyboard.isKeyDown(core.input.keyboard.key.K) then
		mouseX, mouseY = core.input.mouse.getPosition()
		
		print("MousePos: " .. mouseX .. ", " .. mouseY)
	end
end
				
function ToolTipHandler:register(newToolTip)
	self.toolTip = newToolTip
end

function ToolTipHandler:deregister()
	self.toolTip = nil
end
				
return ToolTipHandler