local O = {}

function O.new()
    local self = {}
    self.objectives = {}
    setmetatable( self, { __index = O } )
    return self
end

function O:update( delta )
    if core.console.isVisible() == false then
        for index ,obj in pairs( self.objectives ) do
            
            if obj.state == "success" then
                core.draw.drawText( 20, index * 20 + 20 + 150, "[X]" )
            else
                core.draw.drawText( 20, index * 20 + 20 + 150, "[ ]" )
            end
                
            core.draw.drawText( 55, index * 20 + 20 + 150, obj.title )
        end
    end
end

function O:addObjective( obj )
    self.objectives[#(self.objectives)+1] = obj
end

function O:removeObjective( obj )
    local newObjs = {}
    for k,v in pairs( self.objectives ) do
        if obj ~= v then
            newObjs[#newObjs+1] = v
        end
    end
    self.objectives = newObjs
end

function O:destroy()

end

return O
