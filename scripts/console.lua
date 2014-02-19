function toggleLightVolumes()
    if core.config.debugLightVolumes then
        core.config.debugLightVolumes = false
    else
        core.config.debugLightVolumes = true
    end
end

function showSys()
    if core.config.showSystems then
        core.config.showSystems = false
    else
        core.config.showSystems = true
    end
end

-- Toggles which fbo to render
-- Fullscreen lit scene (final mode) : -1
-- Fullscreen lit scene + miniature render targets: 0
-- Fullscreen rendertarget n : (1-4)
function showFBO(which)
    core.config.showFramebuffers = which
end

function help()
    print "The following functions exist:" 
    print "showSys()" 
    print "showFBO(which)" 
	print "showMesh()" 
    print "print_env()" 
    print "toggleMenu()"
end

-- Loads a scenario from the scenario folder and returns the assembly.
function doscenario( name )
    print( "Use 'openscenario()' instead:" )
    --return dofile( "scripts/scenarios/" .. name .. ".lua" ) 
end

function currentscenario()
    print( current_scenario_name )
end

function shh()
    core.config.consoleOutputLevel = "fatal"
end

function getCompleteTable( table, prefix, input_table )
    input_table = input_table or {}

    if prefix == nil then
        prefix = ""
    end

    if table == nil then
       table = core
       prefix = "core"
    end  

    for k,v in  pairs( table ) do
        if type( v ) == "function" then
            input_table[#input_table+1] = prefix .. "." .. k .. "("
        elseif type( v ) == "table" then
            input_table[#input_table+1] = prefix .. "." .. k 
                print_env( v, prefix .. "." .. k, input_table )
        else
            input_table[#input_table+1] = prefix .. "." .. k 
        end
    end

    return input_table
end

-- Prints all keys and sub-keys for given table, useful if you want to see what core looks like.
function print_env( table, prefix)
    if prefix == nil then
        prefix = ""
    end

    if table == nil then
       table = core
       prefix = "core"
    end  

    for k,v in  pairs( table ) do
        if type( v ) == "function" then
            print( prefix .. "." .. k .. "()" )
        elseif type( v ) == "table" then
            print( prefix .. "." .. k .. " [table]")
                print_env( v, prefix .. "." .. k )
        else
            print( prefix .. "." .. k )
        end
    end
end

function print_test()
    for _,v in pairs( core.system.squad.getPossibleAbilities(1) ) do
        print(v)
    end
end



function print_loaded()
    for k,v in pairs( package.loaded ) do
        print( k )
    end
end

function errorHandler( errorMessage )
    return errorMessage .. "\n"  .. debug.traceback()
end

function print( ... )
    core.log.info:print( ... )
end

       
