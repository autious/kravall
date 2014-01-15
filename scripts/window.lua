
local window = {}
local windowSizeCallback = {}

function core.window.windowSizeCallback( width, height )
    for k,v in pairs( windowSizeCallback ) do
        k(width,height)
    end
end

function window.registerWindowSizeCallback( func )
    windowSizeCallback[func] = true
end

function window.deregisterWindowSizeCallback( func )
    windowSizeCallback[func] = nil
end

return window
