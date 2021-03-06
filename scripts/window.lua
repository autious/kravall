local window = {}
local windowSizeCallback = {}
local windowFocusCallback = {}
local framebufferSizeCallback = {}

window.width,window.height = core.window.getSize()
if window.width == nil or window.height == nil then
    window.width,window.height = core.config.initScreenWidth, core.config.initScreenHeight
end

function core.window.windowSizeCallback( width, height )
    window.width = width
    window.height = height
    for k,v in pairs( windowSizeCallback ) do
        k(width,height)
    end
end

function core.window.windowFocusCallback( focus )
    for k,v in pairs( windowFocusCallback ) do
        k(focus)
    end
end

function core.window.framebufferSizeCallback( width, height )
    window.fwidth = width
    window.fheight = height
    for k,v in pairs( framebufferSizeCallback ) do
        k(width,height)
    end
end

function window.registerWindowFocusCallback( func )
    assert( func ~= nil )
    windowFocusCallback[func] = true
end

function window.deregisterWindowFocusCallback( func )
    assert( func ~= nil )
    windowFocusCallback[func] = nil
end

function window.registerWindowSizeCallback( func )
    assert( func ~= nil )
    windowSizeCallback[func] = true
end

function window.deregisterWindowSizeCallback( func )
    assert( func ~= nil )
    windowSizeCallback[func] = nil
end

function window.registerFramebufferSizeCallback( func )
    assert( func ~= nil )
    framebufferSizeCallback[func] = true 
end

function window.deregisterFramebufferSizeCallback( func )
    assert( func ~= nil )
    framebufferSizeCallback[func] = true 
end

return window
