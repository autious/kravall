local O = 
{ 
    onObjectiveChange = function() end,
    title = "Default Title",
    active = true,
    state = "",
}

function O:new(o)
    o = o or {}
    
    setmetatable( o, self )

    return o
end

function O:__index( key )
    return rawget( self, "i_" .. key )
end

function O:__newindex( key, value )
    if key == "title" or key == "state" or key == "active" then
        rawset( self, "i_" .. key, value )
        rawget( self,"onObjectiveChange" )(self)
    else
        error("Invalid index: " .. key) 
    end
end

function O:setTitle( title )
    
end

return O
