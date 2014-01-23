local input = require"input"
local mouse = core.input.mouse
local GUIComponent = { x,y,width,height, material = "assets/material/ui/test.material" }

function GUIComponent:new(o)
    o = o or {}
    setmetatable( o, self )
    self.__index = self
    if o.callbacks then
        error( "callbacks can't be a used value" )
    end
    o.callbacks = {}
    local inside = false

    function o.callbacks.onPosition( x,y )
        if AABB:new({o.xpos,o.ypos,o.width,o.height}):collides( x, y ) then
            if inside == false then
                o.onEnter()  
            end
            inside = true
        else
            if inside then 
                o.onExit()
            end
            inside = false
        end
    end

    function o.callbacks.onButton( button, action, mods )
        if button == mouse.button.Left then
            if action == core.input.Press then
                o.onPress() 
            elseif action == core.input.Release then
                o.onRelease()         
            end    
        end
    end

    input.registerOnButton(o.callbacks.onButton)
    input.registerOnPosition(o.callbacks.onPosition)

    return o
end

--Virtual functions to be overloaded.
function GUIComponent:onHover()
    print( "hover" )
end

function GUIComponent:onPress( )
    print( "press" )
end

function GUIComponent:onRelease()
    print( "release" )
end

function GUIComponent:destroy()
    self.mesh:free()
    self.material:free()
    self.ent:destroy()
    input.deregisterOnButton(self.callbacks.onButton)
    input.deregisterOnPosition(self.callbacks.onPosition)
end

return GUIComponent
