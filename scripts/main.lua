local Camera = require "camera" 
local gui_component = require "gui/gui_component"

-- Called on each frame
camera = Camera.new()

function core.update( delta )
    camera:update( delta )
end

comp = nil
-- Called when program starts
function core.init() 
    print( "Program starting in lua" )
    showSys()
    openscenario( "test" )    
end

function toggle()
    if comp == nil then
        comp = gui_component.new() 
    else
        for k in pairs( comp ) do
            print( k )
        end 
        comp:destroy()
        comp = nil
    end
end
-- Called when program end
function core.stop()
    print "Program stopping in lua" 
end
