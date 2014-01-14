local input = require "input" 
local C = {}

local vec3 = core.glm.vec3
local mat4 = core.glm.mat4
local quat = core.glm.quat
local keyboard = core.input.keyboard
local mouse = core.input.mouse
local window = core.window
local camera = core.camera
local key = keyboard.key

function C.new( )
    local self = {}
    self.position = vec3.new( 0,0,0 )
    self.pitch = 0
    self.yaw = 0

    self.width = core.config.initScreenWidth
    self.height = core.config.initScreenHeight

    --mouse.setPosition( self.width/2, self.height/2 );

    self.px,self.py = mouse.getPosition()

    local metatable = { __index = C, __gc = destruct }
    setmetatable( self, metatable )

    return self
end

function C:getProjection()
    -- TODO: replace with callback.
    return mat4.perspective( 60, self.width/self.height, 0.1, 1000 )
end

function C:getView()
    local qp = quat.angleAxis( self.pitch, vec3.new(1,0,0) )
    qp = qp:rotate( self.yaw, vec3.new(0,1,0) )

    return qp:mat4Cast() * mat4.new(1):translate( self.position * (-1))
end

function C:update( dt )

    local delta = dt * 30
    if keyboard.iskeydown( key.W ) then
        self.position = self.position + camera.getForward() * delta
    end
    if keyboard.iskeydown( key.S ) then
        self.position = self.position - camera.getForward() * delta
    end
    if keyboard.iskeydown( key.A ) then
        self.position = self.position - camera.getRight() * delta
    end
    if keyboard.iskeydown( key.D ) then
        self.position = self.position + camera.getRight() * delta
    end
    if keyboard.iskeydown( key.Space ) then
        self.position = self.position + camera.getUp() * delta
    end
    if keyboard.iskeydown( key.Left_control ) then
        self.position = self.position - camera.getUp() * delta
    end
    
    local x,y = mouse.getPosition()

    if mouse.isbuttondown( mouse.button.Left ) then
        self.pitch = self.pitch + (y-self.py) * delta
        self.yaw = self.yaw + (x-self.px) * delta
    end

    self.px = x
    self.py = y

    local proj = self:getProjection()
    local view = self:getView()
    camera.setProjection( proj  )
    camera.setView( view )
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
    camera.setProjection( proj  )
    camera.setView( view )
end
	
	
	
return C
