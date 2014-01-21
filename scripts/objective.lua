local O = {}

function O.new( title )
    local self = {}

    self.title = title
    self.active = true
    self.state = ""
   
    return self
end

return O
