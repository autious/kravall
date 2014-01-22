local window = {}
local windowSizeCallback = {}
local windowFocusCallback = {}

function core.window.windowSizeCallback( width, height )
    for k,v in pairs( windowSizeCallback ) do
        k(width,height)
    end
end

function core.window.windowFocusCallback( focus )
    for k,v in pairs( windowFocusCallback ) do
        k(focus)
    end
end

function window.registerWindowFocusCallback( func )
    windowFocusCallback[func] = true
end

function window.deregisterWindowFocusCallback( func )
    windowFocusCallback[func] = nil
end

function window.registerWindowSizeCallback( func )
    windowSizeCallback[func] = true
end

function window.deregisterWindowSizeCallback( func )
    windowSizeCallback[func] = nil
end

return window
