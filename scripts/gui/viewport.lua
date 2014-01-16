local window = require "window"
local camera = core.camera.overlayCamera 
local mat4 = core.glm.mat4
local V = {}

camera:setProjection( mat4.ortho( 0, core.config.initScreenWidth, 0, core.config.initScreenHeight,0,100 ) )
camera:setView( mat4.new(1) )

window.registerWindowSizeCallback( function( width, height ) 
    camera:setProjection( mat4.ortho( 0, width, 0, height,0,100 ))
end)
