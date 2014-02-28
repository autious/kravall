local input = require"input"
local image = require"factories/image"
local AABB = require"collision/aabb"

local mouse = core.input.mouse
local GUIComponent = { x=0,y=0,width=0,height=0,inside = false, press = false, show = true }

function GUIComponent:new(o)
    o = o or {}
    setmetatable( o, self )
    self.__index = self

    if o.callbacks then
        error( "callbacks can't be a used value" )
    end

    o.callbacks = {}
    local curr_mouse_x = 0
    local curr_mouse_y = 0
    function o.callbacks.onPosition( x,y )
        curr_mouse_x = x
        curr_mouse_y = y
        if AABB:new({o.x,o.y,o.width,o.height}):collides( x, y ) and o.show then
            if o.inside == false then
                o.onEnter()  
            end
            o.inside = true

            if o.press then
                o.drag = true
            end
        else
            if o.inside then 
                o.onExit()
            end
            o.inside = false
        end

        if o.drag then
            o.onDrag( x,y )
        end
    
    end
    function o.callbacks.onButton( button, action, mods )
        local consumed = false

        if button == mouse.button.Left then
            if action == core.input.action.Press then
                if o.inside == true then

                    o.onPress( curr_mouse_x, curr_mouse_y ) 
                    o.press = true
                    consumed = true -- we assume that this is consumed, so game can ignore
                end
            elseif action == core.input.action.Release then
                if o.inside == true then
                    o.onRelease()         
                end
                o.press = false
                o.drag = false
            end 
        end
        return consumed
    end

    input.registerOnButton(o.callbacks.onButton, "GUI")
    input.registerOnPosition(o.callbacks.onPosition)

    return o
end

function GUIComponent:setShow( flag )
    self.show = flag
end

--Virtual functions to be overloaded.
function GUIComponent.onEnter()
    --print( "enter" )
end

function GUIComponent.onExit()
    --print( "exit" )
end

function GUIComponent.onPress()
    --print( "press" )
end

function GUIComponent.onRelease()
   --print( "release" )
end

function GUIComponent.onDrag(x,y)
    --print( "drag" )
end

function GUIComponent:setPosition(x,y)
    self.x = x
    self.y = y
end

function GUIComponent:destroy()
    input.deregisterOnButton(self.callbacks.onButton)
    input.deregisterOnPosition(self.callbacks.onPosition)
end

return GUIComponent
