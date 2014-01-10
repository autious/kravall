local Camera = require "camera" 

-- Called on each frame
local camera = Camera.new()

function core.update( delta )
    camera:update( delta )
end

-- Called when program starts
function core.init() 
    print( "Program starting in lua" )
    openscenario( "test" )
    showSys()
end

-- Called when program end
function core.stop()
    print "Program stopping in lua" 
end
