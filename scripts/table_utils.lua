local M = {}

function M.count( table )
    local i = 0
        
    for _,_ in pairs( table ) do
        i = i + 1
    end

    return i
end

return M
