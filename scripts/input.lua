
--function core.input.keyboard.onkey( key, scancode, action )
--    print( "onkey " .. key .. " " .. scancode ..  " " .. action )
--end
--
--function core.input.keyboard.onchar( codepoint, text )
--    print( "onchar " .. codepoint .. " " .. text )
--end
--
--function core.input.mouse.onbutton( button, action, mods )
--    print( "onbutton " .. button .. " " .. action .. " " .. mods ) 
--end
--


local input = {}
local onpositions = {}
local onscroll = {}

function core.input.mouse.onposition( x,y )
    for k,v in pairs( onpositions ) do
       k( x,y ) 
    end 
end

function core.input.mouse.onscroll( x, y )
    for k,v in pairs( onscroll ) do
       k( x,y ) 
    end 
end

function input.registerOnposition( func )
    onpositions[func] = true
end

function input.deregisterOnposition( func )
    onpositions[func] = nil
end

function input.registerOnScroll( func )
    onscroll[func] = true
end

function input.deregisterOnScroll( func )
    onscroll[func] = nil
end

return input
