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

function C:new(o)
    local o = o or {}

    self.width = core.config.initScreenWidth
    self.height = core.config.initScreenHeight

    self.px,self.py = mouse.getPosition()

    local function windowResizeCallback( width, height )
        self.width = width
        self.height = height 
    end
    window.registerWindowSizeCallback( windowResizeCallback )

        
    setmetatable( o, self )
    self.__index = self
    self.__gc = function()
        window.deregisterWindowSizeCallback( windowResizeCallback )
    end

    self.viewMatrix = camera:getView()

    return o
end

function C:getProjection()
    -- TODO: replace with callback.
    return mat4.perspective( 60, self.width/self.height, 0.1, 200 )
end

local unit_right = vec3.new(1,0,0)
local unit_up = vec3.new(0,1,0)
local mat_ident = mat4.new(1)

function C:getView()
    return self.viewMatrix
end

function C:update( dt )
    local delta = dt * 1

    if self.viewGoal ~= nil then
        self.interpolatePosition = self.interpolatePosition + delta
        if self.interpolatePosition > 1 then
            self.interpolatePosition = 1
        end
        self.viewMatrix = self.viewSource:interpolate( self.viewGoal, self.interpolatePosition )

        if self.interpolatePosition == 1 then
            self.viewMatrix = self.viewGoal
            self.viewGoal = nil
        end
    end

    local proj = self:getProjection()
    local view = self:getView()
    camera:setProjection( proj )
    camera:setView( view )
end

function C:setGoal( goalViewMatrix )
    self.viewSource = self:getView()
    self.viewGoal = goalViewMatrix
    self.interpolatePosition = 0
end
    
function C:setPosition( pos )
	self.position = pos	
end

return C
