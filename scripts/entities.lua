local E = {}
function E.get( name )
    return dofile ("scripts/entities/" .. name .. ".lua")
end

return E
