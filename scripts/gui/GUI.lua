local viewport = require "gui/viewport"
local window = require "window"

local GUI = { 
                winWidth = core.config.initScreenWidth, 
                winHeight = core.config.initScreenHeight,
                xoffset = 0,
                yoffset = 0,
                show = true,
            }

window.registerFramebufferSizeCallback( function( width, height ) 
    GUI.winWidth, GUI.winHeight = width, height
end)

function GUI:setShow( flag )
    self.show = flag
    for _,v in  pairs( self.components ) do
        v:setShow( flag )
    end  
end

function GUI:new(o)
    o = o or {}

    setmetatable( o, self )
    self.__index = self

    o.components = {}
    o.placers = {}
    o.width = o.width or o.winWidth
    o.height = o.height or o.winHeight

    -- Set our position to enable/disable the 
    -- constrict on resize call
    o:setPosition( o.x, o.y )

    o:setShow( o.show )
    return o
end

function GUI:addComponents( components )
    for _,v in pairs( components ) do
        self:addComponent( v )
    end
end

function GUI:addComponent( comp )
    assert( comp ~= nil, "Component is nil" )
    self.components[#(self.components)+1] = comp 
    self:constrict( )
    comp:setShow( self.show )
end

function GUI:addPlacementHandler( handler )
    assert( handler ~= nil, "Given placer is nil" )
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

-- Can be called with nil-values to reactivate automatic window resizing
function GUI:setPosition(x,y)
    self.x = x
    self.y = y

    -- Callback function to move gui components.
    -- We ignore the window dimensions if we have our own specific placement
    if self.x == nil and self.y == nil and self.resizeCallback == nil then
        self.resizeCallback = function( width, height )
            self.width = width
            self.height = height 
            self:constrict()
        end

        window.registerWindowSizeCallback( self.resizeCallback )
    end

    --Disable the screen resize callback as we now have a specific position in the world
    if self.x ~= nil and self.y ~= nil and self.resizeCallback then
        window.deregisterWindowSizeCallback( self.resizeCallback ) 
        self.resizeCallback = nil
    end

    self:constrict()
end

function GUI:setDimensions(width,height)
    self.width = width
    self.height = height
    self:constrict()
end

function GUI:constrict()
    for _,v in pairs( self.placers ) do
        v( self.components, self.width, self.height, self.x, self.y )
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

    if self.resizeCallback then
        window.deregisterWindowSizeCallback( self.resizeCallback )
    end
end

function GUI:renderDebug(delta)
    if core.config.debugDrawGUIBorders then 
        core.draw.drawRectangle( self.x, self.y, self.width, self.height, false )
    end

    for k,v in pairs( self.components ) do
        if v.renderDebug then
            v:renderDebug( delta )
        end
    end
end

function GUI:update()
    print( "GUI:update(): WARNING THIS FUNCTION IS NO LONGER IN ACTIVE USE I REFER TO renderDebug(delta) instead")
end

return GUI
