local input = require "input" 
local window = require "window"
local math = require "math"

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
    self.forwardVelocity = 0
    self.accelerationFactor = 1
    self.acceleration = 1.0
    self.deaccelerationFactor = 0.5
    self.mousePressLocation = nil
    self.movementSpeed = 0


    self.width = core.config.initScreenWidth
    self.height = core.config.initScreenHeight
    self.infocus = true

    --mouse.setPosition( self.width/2, self.height/2 );

    self.px,self.py = mouse.getPosition()

    local function windowResizeCallback( width, height )
        self.width = width
        self.height = height 
    end
    window.registerWindowSizeCallback( windowResizeCallback )
    
    local function focus( f )
        self.infocus = (f ~= 0)
    end
    window.registerWindowFocusCallback( focus )

    local function onscroll( x, y )
        local forward = camera:getForward()
        --self.position = self.position + forward * y * 0.5;

        self.forwardVelocity = self.forwardVelocity + y * self.accelerationFactor;

        if self.forwardVelocity > 0 and not self.cantGoForward then
            self.cantGoBack = false
        elseif self.forwardVelocity < 0 and not self.cantGoBack then
            self.cantGoForward = false
        else
            self.forwardVelocity = 0
        end
    end

    input.registerOnScroll( onscroll )
        
    local function destruct( )
        window.deregisterWindowSizeCallback( windowResizeCallback )
        window.deregisterWindowFocusCallback( focus )
        input.deregisterOnScroll( onscroll )
    end
        
    local metatable = { __index = C, __gc = destruct }
    setmetatable( self, metatable )

    return self
end

function C:getProjection()
    return mat4.perspective( core.config.initCameraFieldOfView, self.width/self.height, core.config.initCameraNearClipDistance, core.config.initCameraFarClipDistance )
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

    if core.console.isVisible() == false and self.infocus == true then
        local prevPosition = self.position
        local ux,uy,uz = camera:getUp():get()
        local rx,ry,rz = camera:getRight():get()
        local fx,fy,fz = camera:getForward():get()

        local xzUp = vec3.new(0,0,0)
        local xzRight = vec3.new(0,0,0)
        local forward = camera:getForward()

        if ux ~= 0 or uz ~= 0 then
             xzUp = vec3.new( ux,0,uz ):normalize()
        end

        if fx ~= 0 or fz ~= 0 then
             xzForward = vec3.new( fx,0,fz ):normalize()
        end

        if rx ~= 0 or rz ~= 0 then
             xzRight= vec3.new( rx,0,rz ):normalize()
        end
       
        local direction = vec3.new(0,0,0)

        if keyboard.isKeyDown( key.W ) then
            direction = direction + xzForward
        end
        if keyboard.isKeyDown( key.S ) then
            direction = direction - xzForward
        end
        if keyboard.isKeyDown( key.A ) then
            direction = direction - xzRight
        end
        if keyboard.isKeyDown( key.D ) then
            direction = direction + xzRight 
        end
        if keyboard.isKeyDown( key.Space ) then
            direction = direction + vec3.new(0,1,0)
        end
        if keyboard.isKeyDown( key.Left_control ) then
            direction = direction - vec3.new(0,1,0) 
        end
        
        if direction:length() > 0 then
            local force = self.acceleration - 0.0884 * self.movementSpeed * self.movementSpeed 
            self.movementSpeed = self.movementSpeed + force * delta
            direction:normalize()
        else
            self.movementSpeed = 0
        end

        self.position = self.position + direction * delta * self.movementSpeed
        
        local x,y = mouse.getPosition()

        if mouse.isButtonDown( mouse.button.Middle ) then
            self.pitch = self.pitch + (y-self.py) * 0.3
            self.yaw = self.yaw + (x-self.px) * 0.3
        end 

        if self.mousePressLocation ~= nil then
            self.position = self.position + xzRight * (x-self.mousePressLocation.x) * 0.01 * delta
            self.position = self.position - xzUp * (y-self.mousePressLocation.y) * 0.01 * delta
            if mouse.isButtonDown( mouse.button[5] ) == false then
                self.mousePressLocation = nil
            end
        else
            if mouse.isButtonDown( mouse.button[5] ) then
                self.mousePressLocation = {x=x,y=y}
            end
        end

        
        self.position = self.position + forward * self.forwardVelocity * delta;
    
        if self.forwardVelocity > 0 then
            self.forwardVelocity = self.forwardVelocity - self.deaccelerationFactor * delta;
            if self.forwardVelocity < 0 then
                self.forwardVelocity = 0
            end
        elseif self.forwardVelocity < 0 then
            self.forwardVelocity = self.forwardVelocity + self.deaccelerationFactor * delta;
            if self.forwardVelocity > 0 then
                self.forwardVelocity = 0
            end
        end
        
        if x < 20 then 
            self.position = self.position - xzRight  * core.config.cameraScrollingSpeed * delta
        end
        if self.width-x < 20 then
            self.position = self.position + xzRight  * core.config.cameraScrollingSpeed * delta
        end
        if y < 20 then
            self.position = self.position + xzUp * core.config.cameraScrollingSpeed * delta
        end
        if self.height-y < 20 then
            self.position = self.position - xzUp * core.config.cameraScrollingSpeed * delta
        end

        self.px = x
        self.py = y
        
        local px,py,pz = self.position:get()

        if py > 10000 then
            px,py,pz = prevPosition:get()
            self.forwardVelocity = 0
            self.cantGoBack = true
        elseif py < 10 then
            px,py,pz = prevPosition:get()
            self.forwardVelocity = 0
            self.cantGoForward = true
        end
        self.position = vec3.new( px,py,pz )

        local proj = self:getProjection()
        local view = self:getView()
        camera:setProjection( proj )
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
	--self.yaw = math.atan2( y, x ) * 180 / math.pi
	self.pitch = math.asin( -y ) * 180 / math.pi
	
	self.px, self.py = mouse.getPosition()
	
	local proj = self:getProjection()
    local view = self:getView()
    camera:setProjection( proj )
    camera:setView( view )
end

return C
