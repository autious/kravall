local Prep = { name = "Prep" }
function Prep:new(o)
    o = o or {}
    setmetatable( o, self )
    self.__index = self

    return o
end

function Prep:destroy()

end

return Prep
