local input = {}
local onpositions = {}
local onscroll = {}
local onbutton = {DEFAULT = {}, GUI = {}, GAME = {}}
local onchar = {}
local onkey = {}

local onbutton_groups = { "DEFAULT", "GUI", "GAME" }

function core.input.mouse.onPosition( x,y )
    for k,v in pairs( onpositions ) do
       k( x,y ) 
    end 
end

function core.input.mouse.onScroll( x, y )
    for k,v in pairs( onscroll ) do
       k( x,y ) 
    end 
end

function core.input.mouse.onButton( button, action, mods )
    local consumed = false
    local tmp
    for k,v in pairs( onbutton["GUI"] ) do
       tmp = k( button, action, mods, consumed )
       consumed = consumed or tmp or false
    end 

    for k,v in pairs( onbutton["GAME"] ) do
        tmp = k( button, action, mods, consumed )
        consumed = consumed or tmp  or false
    end 

    for k,v in pairs( onbutton["DEFAULT"] ) do
        tmp = k( button, action, mods, consumed ) 
        consumed = consumed or tmp  or false
    end 
end

function core.input.keyboard.onChar( codepoint, text )
    for k,v in pairs( onchar ) do
       k( codepoint, text )
    end 
end

function core.input.keyboard.onKey( key, scancode, action )
    for k,v in pairs( onkey ) do
       k( key, scancode, action )
    end 
end

function input.registerOnPosition( func )
    onpositions[func] = true
end

function input.deregisterOnPosition( func )
    onpositions[func] = nil
end

function input.registerOnScroll( func )
    onscroll[func] = true
end

function input.deregisterOnScroll( func )
    onscroll[func] = nil
end

function input.registerOnButton( func, group )

    if group then
        local exist = false
        for _,v in pairs( onbutton_groups ) do
            if v == group then
                exist = true
            end 
        end
        assert( exist, "Given group is not valid" )
        onbutton[group][func] = true   
    else 
        onbutton["DEFAULT"][func] = true
    end
end

function input.deregisterOnButton( func )
    for _,v in pairs(onbutton) do
        v[func] = nil
    end
end

function input.registerOnChar( func )
    onchar[func] = true
end

function input.deregisterOnChar( func )
    onchar[func] = nil
end

function input.registerOnKey( func )
    onkey[func] = true
end

function input.deregisterOnKey( func )
    onkey[func] = nil
end

return input
