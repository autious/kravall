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
    self.position = vec3.new( 0,10,0 )
    self.previousInterpolationPoint = self.position
    self.forwardVelocity = 0
    self.quatRotation = quat.new()
    self.accelerationFactor = 5
    self.deaccelerationFactor = 3.5
    self.mousePressLocation = nil
    self.movementSpeed = 0
    self.interpolationSpeed = 10
    self.interpolationFactor = 0
    self.interpolationPoints = {}
    self.previousInterpolationPoint = {position = self.position, rotation = self.quatRotation}

    self.inOverview = false
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
        if not self.inOverview then
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
    return self.quatRotation:mat4Cast() * mat_ident:translate( self.position * (-1))
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

        if self.previousInterpolationPoint.passCallback
           and not self.previousInterpolationPoint.calledPass  then
            self.previousInterpolationPoint:passCallback()
            self.previousInterpolationPoint.calledPass = true
        end

        if #(self.interpolationPoints) > 0 then
            if #(self.interpolationPoints) > 2 then
                --catmul spline                
                local p0 = self.previousInterpolationPoint.position
                local p1 = self.interpolationPoints[1].position
                local p2 = self.interpolationPoints[2].position
                local p3 = self.interpolationPoints[3].position

                local pointOne = {self.previousInterpolationPoint.position:get()}
                local pointTwo = {self.interpolationPoints[1].position:get()}
                local pointThree = {self.interpolationPoints[2].position:get()}
                local pointFour = {self.interpolationPoints[3].position:get()}

                self.interpolationPointsDistance = math.sqrt((pointThree[1] - pointTwo[1]) * (pointThree[1] - pointTwo[1]) + (pointThree[2] - pointTwo[2]) * (pointThree[2] - pointTwo[2]) + (pointThree[3] - pointTwo[3]) * (pointThree[3] - pointTwo[3]))

                self.interpolationFactor = self.interpolationFactor + (self.interpolationPoints[2].speed / self.interpolationPointsDistance) * delta
                self.interpolationFactor = math.min(1.0, self.interpolationFactor)

                local t = self.interpolationFactor
                local tt = t * self.interpolationFactor
                local ttt = tt * self.interpolationFactor

                self.position = ((p1 * 2) + (p0 * (-1) + p2) * t + (p0 * 2 -p1 * 5 + p2 * 4 - p3) * tt + (p0 * (-1) + p1 * 3 - p2 * 3 + p3) * ttt) * 0.5
                self.quatRotation = self.interpolationPoints[1].rotation:slerp(self.interpolationPoints[2].rotation, self.interpolationFactor) 

                if self.interpolationFactor >= 1.0 then
                    self.interpolationFactor = 0
                    self.previousInterpolationPoint = self.interpolationPoints[1]

                    table.remove(self.interpolationPoints, 1)
                end                
            else
                --smoothstep interpolation
                local function SmoothStep(x)
                    return x * x * (3 - 2 * x)
                end

                local pointOne = {self.previousInterpolationPoint.position:get()}
                local pointTwo = {self.interpolationPoints[1].position:get()}

                self.interpolationPointsDistance = math.sqrt((pointOne[1] - pointTwo[1]) * (pointOne[1] - pointTwo[1]) + (pointOne[2] - pointTwo[2]) * (pointOne[2] - pointTwo[2]) + (pointOne[3] - pointTwo[3]) * (pointOne[3] - pointTwo[3]))

                self.interpolationFactor = self.interpolationFactor + (self.interpolationPoints[1].speed / self.interpolationPointsDistance) * delta
                self.interpolationFactor = math.min(1.0, self.interpolationFactor)

                local factor = SmoothStep(SmoothStep(SmoothStep(self.interpolationFactor)))
                self.position = self.previousInterpolationPoint.position * (1.0 - factor) + self.interpolationPoints[1].position * factor
                self.quatRotation = self.previousInterpolationPoint.rotation:slerp(self.interpolationPoints[1].rotation, factor)               


                if self.interpolationFactor >= 1.0 then
                    self.interpolationFactor = 0
                    self.previousInterpolationPoint = self.interpolationPoints[1]

                    table.remove(self.interpolationPoints, 1)
                end                
            end

        else
            if not self.inOverview then            
                if keyboard.isKeyDown( key.W ) or keyboard.isKeyDown(key.Up )then
                    direction = direction + xzForward
                end
                if keyboard.isKeyDown( key.S ) or keyboard.isKeyDown(key.Down )then
                    direction = direction - xzForward
                end
                if keyboard.isKeyDown( key.A ) or keyboard.isKeyDown(key.Left )then
                    direction = direction - xzRight
                end
                if keyboard.isKeyDown( key.D ) or keyboard.isKeyDown(key.Right )then
                    direction = direction + xzRight 
                end
                if keyboard.isKeyDown( key.Q ) then
                    self.quatRotation = self.quatRotation:rotate( -cameraRotationSpeed*delta , vec3.new(0, 1, 0) )
                end
                if keyboard.isKeyDown( key.E ) then
                    self.quatRotation = self.quatRotation:rotate( cameraRotationSoeed*delta , vec3.new(0, 1, 0) )
                end
                if keyboard.isKeyDown( key.Space ) then
                    direction = direction + vec3.new(0,1,0)
                elseif keyboard.isKeyDown(key.Z) then
                    direction = direction - vec3.new(0,1,0)
                end
                
                if direction:length() > 0 then
                    local force = core.config.cameraForce - 0.1984 * self.movementSpeed * self.movementSpeed 
                    force = math.max(force, 0)
                    self.movementSpeed = self.movementSpeed + force * delta
                    direction:normalize()
                else
                    self.movementSpeed = 0
                end

                self.position = self.position + direction * delta * self.movementSpeed
                
                local x,y = mouse.getPosition()

                if mouse.isButtonDown( mouse.button.Middle ) then
                    
                    local tmp = self.quatRotation:rotate( (y-self.py) * core.config.cameraRotationSpeed * delta , camera:getRight() )
                    local _,y,_,_ = (tmp:mat4Cast() * core.glm.vec4.new( 0,1,0,0 )):get()

                    if y > 0 then
                        self.quatRotation = tmp 
                    else -- the following is  BEAUTIFUL hack to prevent the camera from jumping when it hits the limi.
                        -- the idea is that we iteratively move the camera to en end, doing one pixel movement at a time.
                        local _,fy,_,_ = camera:getForward():get()
                        if fy < 0 then
                            tmp = self.quatRotation:rotate( 1 * core.config.cameraRotationSpeed * delta , camera:getRight() )
                            _,y,_,_ = (tmp:mat4Cast() * core.glm.vec4.new( 0,1,0,0 )):get()

                            while y > 0  do
                                self.quatRotation = tmp 
                                
                                tmp = self.quatRotation:rotate( 1 * core.config.cameraRotationSpeed * delta , camera:getRight() )
                                _,y,_,_ = (tmp:mat4Cast() * core.glm.vec4.new( 0,1,0,0 )):get()
                            end
                        else
                            tmp = self.quatRotation:rotate( -1 * core.config.cameraRotationSpeed * delta , camera:getRight() )
                            _,y,_,_ = (tmp:mat4Cast() * core.glm.vec4.new( 0,1,0,0 )):get()

                            while y > 0  do
                                self.quatRotation = tmp 
                                
                                tmp = self.quatRotation:rotate( -1 * core.config.cameraRotationSpeed * delta , camera:getRight() )
                                _,y,_,_ = (tmp:mat4Cast() * core.glm.vec4.new( 0,1,0,0 )):get()
                            end
                        end
                    end

                    local _,y,_,_ = (self.quatRotation:mat4Cast() * core.glm.vec4.new( 0,1,0,0 )):get()
                    if y < 0 then
                        self.quatRotation = quat:new()
                    end

                    self.quatRotation = self.quatRotation:rotate( (x-self.px) * core.config.cameraRotationSpeed * delta , vec3.new(0, 1, 0) )
                end 

                if self.mousePressLocation ~= nil then
                    self.position = self.position + xzRight * (x - self.mousePressLocation.x) * 0.01 * delta
                    self.position = self.position - xzUp * (y - self.mousePressLocation.y) * 0.01 * delta
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

                if py > 250 then
                    px,py,pz = prevPosition:get()
                    self.forwardVelocity = 0
                    self.cantGoBack = true
                elseif py < 10 then
                    px,py,pz = prevPosition:get()
                    self.forwardVelocity = 0
                    self.cantGoForward = true
                end

                if self.cantGoForward and self.cantGoBackward then
                    py = 20 
                    self.cantGoForward = false
                    self.cantGoBackward = false
                end

                self.position = vec3.new( px,py,pz )

            else            
                if keyboard.isKeyDown( key.W ) or keyboard.isKeyDown(key.Up )then
                    direction = direction - core.glm.vec3.new(0, 0, 1)
                end
                if keyboard.isKeyDown( key.S ) or keyboard.isKeyDown(key.Down )then
                    direction = direction + core.glm.vec3.new(0, 0, 1)
                end
                if keyboard.isKeyDown( key.A ) or keyboard.isKeyDown(key.Left )then
                    direction = direction - core.glm.vec3.new(1, 0, 0)
                end
                if keyboard.isKeyDown( key.D ) or keyboard.isKeyDown(key.Right )then
                    direction = direction + core.glm.vec3.new(1, 0, 0) 
                end

                if direction:length() > 0 then
                    local force = self.acceleration - 0.1984 * self.movementSpeed * self.movementSpeed 
                    force = math.max(force, 0)
                    self.movementSpeed = self.movementSpeed + force * delta
                    direction:normalize()
                else
                    self.movementSpeed = 0
                end

                self.position = self.position + direction * delta * self.movementSpeed
            end
            self.previousInterpolationPoint = {position = self.position, rotation = self.quatRotation}
        end
        
        local proj = self:getProjection()
        local view = self:getView()
        camera:setProjection( proj )
        camera:setView( view )
    end
end

function C:setPosition( pos )
	self.position = pos	
    self.previousInterpolationPoint = {position = self.position, lookat = self.position + camera:getForward()}
end

function C:lookAt( position, target )
    camera:lookAt(position, target)
    self.position = camera:getPosition()
    --self.quatRotation = quat.new(camera:getView())
end


function C:addInterpolationPoint( position, rotation, speed, passCallback )
    speed = speed or self.interpolationSpeed
    assert( speed )
    table.insert(self.interpolationPoints, {position = position, rotation = rotation, speed = speed, passCallback = passCallback })
end

return C
