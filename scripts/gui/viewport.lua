local window = require "window"
local camera = core.camera.overlayCamera 
local mat4 = core.glm.mat4
local V = {}

camera:setProjection( mat4.ortho( 0, core.config.initScreenWidth, -core.config.initScreenHeight, 0,-1,1 ) )
camera:setView( mat4.new(1) )

window.registerWindowSizeCallback( function( width, height ) 
    camera:setProjection( mat4.ortho( 0, width, -height,0,-1,1) )
end)
