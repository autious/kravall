local O = {}

function O:new(o)
    local o = o or {}
    o.objectives = {}
    o.state = ""
    setmetatable( o, self )
    self.__index = self
    return o
end

function O:isWin()
    return self.state == "success"
end

function O:isLoss()
    return self.state == "fail"
end

function O:update( delta )
    if core.console.isVisible() == false then
        for index ,obj in pairs( self.objectives ) do
            
            if obj.state == "success" then
                core.draw.drawText( 20, index * 20 + 20 + 150, "[/]" )
            elseif obj.state == "fail" then
                core.draw.drawText( 20, index * 20 + 20 + 150, "[x]" )
            else  
                core.draw.drawText( 20, index * 20 + 20 + 150, "[ ]" )
            end
                
            core.draw.drawText( 55, index * 20 + 20 + 150, obj.title )
        end
    end
    if self.state == "" then
        local won = true
        for i,o in pairs( self.objectives ) do
            if o.state ~= "success" then
                won = false  
            end
        end
        if won then
            self.state = "success" 
        end
        
        local fail = true
        for i,o in pairs( self.objectives ) do
            if o.state ~= "fail" then
                fail = false  
            end
        end
        if fail then
            self.state = "fail"
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
