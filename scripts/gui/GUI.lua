local GUI = { components = {} }
local viewport = require "gui/viewport"

function GUI:new(o)
    o = o or {}
    setmetatable( o, self )
    self.__index = self
    return o
end

function GUI:addComponent( comp )
    self.components[#(self.components)+1] = comp 
end

function GUI:removeComponent( comp )
    local newComponents = {}
    for k,v in pairs( self.components ) do
        if v ~= comp then
            newComponents[#newComponents+1] = v
        end
    end
    self.components = newComponents
end

function GUI:destroy()
    for k,v in pairs( self.components ) do
        v:destroy()
    end
    self.components = {}
end

function GUI:update(delta)
    
end

return GUI
