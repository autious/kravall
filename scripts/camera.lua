local input = require "input" 
local window = require "window"
local C = {}

local vec3 = core.glm.vec3
local mat4 = core.glm.mat4
local quat = core.glm.quat
local keyboard = core.input.keyboard
local mouse = core.input.mouse
local camera= core.camera.gameCamera
local key = keyboard.key

function C.new( )
    local self = {}
    self.position = vec3.new( 0,0,0 )
    self.pitch = 0
    self.yaw = 0
	
	self.near = core.config.initCameraNearClipDistance
	self.far = core.config.initCameraFarClipDistance

    self.width = core.config.initScreenWidth
    self.height = core.config.initScreenHeight

    --mouse.setPosition( self.width/2, self.height/2 );

    self.px,self.py = mouse.getPosition()

    local function windowResizeCallback( width, height )
        self.width = width
        self.height = height 
    end
    window.registerWindowSizeCallback( windowResizeCallback )

    local function destruct( )
        window.deregisterWindowSizeCallback( windowResizeCallback )
    end
        
    local metatable = { __index = C, __gc = destruct }
    setmetatable( self, metatable )

    return self
end

function C:getProjection()
    -- TODO: replace with callback.
    return mat4.perspective( core.config.initCameraFieldOfView, self.width/self.height, self.near, self.far )
end

local unit_right = vec3.new(1,0,0)
local unit_up = vec3.new(0,1,0)
local mat_ident = mat4.new(1)

function C:getView()
    local qp = quat.angleAxis( self.pitch, unit_right  )
    qp = qp:rotate( self.yaw, unit_up )

    return qp:mat4Cast() * mat_ident:translate( self.position * (-1))
end

function C:update( dt )
    local delta = dt * 30

    if core.console.isVisible() == false then
    
        if keyboard.isKeyDown( key.W ) then
            self.position = self.position + camera:getForward() * delta
        end
        if keyboard.isKeyDown( key.S ) then
            self.position = self.position - camera:getForward() * delta
        end
        if keyboard.isKeyDown( key.A ) then
            self.position = self.position - camera:getRight() * delta
        end
        if keyboard.isKeyDown( key.D ) then
            self.position = self.position + camera:getRight() * delta
        end
        if keyboard.isKeyDown( key.Space ) then
            self.position = self.position + camera:getUp() * delta
        end
        if keyboard.isKeyDown( key.Left_control ) then
            self.position = self.position - camera:getUp() * delta
        end
        
        local x,y = mouse.getPosition()

        if mouse.isButtonDown( mouse.button.Left ) then
            self.pitch = self.pitch + (y-self.py) * 0.3
            self.yaw = self.yaw + (x-self.px) * 0.3
        end

        self.px = x
        self.py = y

        local proj = self:getProjection()
        local view = self:getView()
        camera:setProjection( proj, self.near, self.far )
        camera:setView( view )
    end
end

    
function C:setPosition( pos )
	self.position = pos	
end

function C:lookAt( position, target )
	self.position = position
	
	local dir = core.glm.vec3.normalize( core.glm.vec3.subtract( target, position ) )
	local x, y, z = core.glm.vec3.get(dir)
	self.yaw = math.atan( x/-y ) * 180 / math.pi
	self.pitch = math.asin( -y ) * 180 / math.pi
	
	self.px, self.py = mouse.getPosition()
	
	local proj = self:getProjection()
    local view = self:getView()
    camera:setProjection( proj )
    camera:setView( view )
end

return C
