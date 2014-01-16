local MainMenu = {}

local GUIBase = require "gui/component/GUIBase"
require "gui/viewport"

function MainMenu:destroy()
    self.t:destroy()
end

function MainMenu.new()
    local self = {}
    self.t = GUIBase.new()

    setmetatable( self,  { __index = MainMenu } )

    return self
end


return MainMenu
