
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
--function core.input.mouse.onscroll( xoffset, yoffset )
--    print( "onscroll " .. xoffset .. yoffset )
--end
--

local input = {}
local onpositions = {}

function core.input.mouse.onposition( x,y )
    for k,v in pairs( onpositions ) do
       v( x,y ) 
    end 
end

function input.registerOnposition( func )
    onpositions[func] = true
end

function input.deregisterOnposition( func )
    onpositions[func] = nil
end

return input
