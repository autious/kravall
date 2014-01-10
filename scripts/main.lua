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
--function core.input.mouse.onposition( x,y )
--    print( "onposition " .. x .. " " .. y  )
--end

-- Called on each frame
function core.update( delta )

end

-- Called when program starts
function core.init() 
    print( "Program starting in lua" )
end

-- Called when program end
function core.stop()
    print "Program stopping in lua" 
end
