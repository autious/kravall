local viewport = require "gui/viewport"
local window = require "window"

local GUI = { winWidth = core.config.initScreenWidth, winHeight = core.config.initScreenHeight }

window.registerWindowSizeCallback( function( width, height ) 
    GUI.winWidth, GUI.winHeight = width, height
end)


function GUI:new(o)
    o = o or {}
    o.components = {}
    o.placers = {}

    setmetatable( o, self )
    self.__index = self

    -- Callback function to move gui components.
    self.resizeCallback = function( width, height )
        for _,v in pairs( o.placers ) do
            v:constrict( o.components, width, height )
        end
    end

    window.registerWindowSizeCallback( self.resizeCallback )

    return o
end

function GUI:addComponent( comp )
    self.components[#(self.components)+1] = comp 
    self:constrict( )
end

function GUI:addPlacementHandler( handler )
    self.placers[#(self.placers)+1] = handler
    self:constrict( )
end

function GUI:removeComponent( comp )
    local newComponents = {}
    for k,v in pairs( self.components ) do
        if v ~= comp then
            newComponents[#newComponents+1] = v
        end
    end
    self.components = newComponents
    self:constrict( )
end

function GUI:constrict()
    for _,v in pairs( self.placers ) do
        v:constrict( self.components, self.winWidth, self.winHeight )
    end
end

function GUI:removePlacementHandler( handler )
    local newPlacers = {}
    for _,v in pairs( self.placers ) do
        if v ~= handler then
            newPlacers[#newPlacers+1] = v
        end
    end
    self.placers = newPlacers
end

function GUI:destroy()
    for k,v in pairs( self.components ) do
        v:destroy()
    end

    self.components = nil
    self.placers = nil
    window.deregisterWindowSizeCallback( self.resizeCallback )
end

function GUI:update(delta)
end

return GUI
