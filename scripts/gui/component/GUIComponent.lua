local input = require"input"
local image = require"factories/image"
local AABB = require"collision/aabb"

local mouse = core.input.mouse
local GUIComponent = { x=0,y=0,width=0,height=0,inside = false, press = false }

function GUIComponent:new(o)
    o = o or {}
    setmetatable( o, self )
    self.__index = self

    if o.callbacks then
        error( "callbacks can't be a used value" )
    end

    o.callbacks = {}
    function o.callbacks.onPosition( x,y )
        if AABB:new({o.x,o.y,o.width,o.height}):collides( x, y ) then
            if o.inside == false then
                o.onEnter()  
            end
            o.inside = true
        else
            if o.inside then 
                o.onExit()
            end
            o.inside = false
        end
    end
    function o.callbacks.onButton( button, action, mods )
        if button == mouse.button.Left then
            if action == core.input.action.Press then
                if o.inside == true then
                    o.onPress() 
                    o.press = true
                end
            elseif action == core.input.action.Release then
                o.onRelease()         
                o.press = false
            end 
        end
    end

    input.registerOnButton(o.callbacks.onButton)
    input.registerOnPosition(o.callbacks.onPosition)

    return o
end

--Virtual functions to be overloaded.
function GUIComponent.onEnter()
    print( "enter" )
end

function GUIComponent.onExit()
    print( "exit" )
end

function GUIComponent.onPress()
    print( "press" )
end

function GUIComponent.onRelease()
    print( "release" )
end

function GUIComponent:setPosition(x,y)
    self.x = x
    self.y  = y
end

function GUIComponent:destroy()
    self.mesh:free()
    self.material:free()
    self.ent:destroy()
    input.deregisterOnButton(self.callbacks.onButton)
    input.deregisterOnPosition(self.callbacks.onPosition)
end

return GUIComponent
