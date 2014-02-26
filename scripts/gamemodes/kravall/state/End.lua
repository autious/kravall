local End = { name = "End" }
function End:new(o)
    o = o or {}
    setmetatable( o, self )
    self.__index = self


    

    return o
end

function End:update( delta )

end

function End:destroy()

end

return End
