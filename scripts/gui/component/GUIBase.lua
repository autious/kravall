local GUIBase = {}

function GUIBase:destroy()
    self.mesh:free()
    self.material:free()
    self.ent:destroy()
end

function GUIBase:newindex( index, value )
    error( "Nope" )
end

function GUIBase.new()
    local self = {}

    setmetatable( self, { __gc = GUIBase.destroy, __index = GUIBase} )

    return self
end

return GUIBase
