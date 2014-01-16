local Camera = require "camera" 
local MainMenu = require "gui/MainMenu"

-- Called on each frame
camera = Camera.new()

function core.update( delta )
    camera:update( delta )
end

-- Called when program starts
function core.init() 
    print( "Program starting in lua" )
    showSys()
    toggleMenu()
    openscenario( "test" )    
end

menuState = nil
function toggleMenu()
    if menuState == nil then
        menuState = MainMenu.new()
    else
        menuState:destroy()
        menuState = nil
    end
end

-- Called when program end
function core.stop()
    print "Program stopping in lua" 
end
