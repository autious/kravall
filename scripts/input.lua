local input = {}
local onpositions = {}
local onscroll = {}
local onbutton = {}
local onchar = {}
local onkey = {}

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
    for k,v in pairs( onbutton ) do
       k( button, action, mods )
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

function input.registerOnButton( func )
    onbutton[func] = true
end

function input.deregisterOnButton( func )
    onbutton[func] = nil
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
