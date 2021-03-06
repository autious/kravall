local U = {}

function U.getTotalDimHeight( components, paddingx, paddingy )
    local tw = 0
    local th = 0
    for _,c in pairs( components ) do     
        if not c.ignoreConstrict then
            if tw < c.width then
                tw = c.width
            end
            th = th + c.height + paddingy
        end
    end

    th = th - paddingy
    tw = tw
    
    return tw, th
end

function U.getTotalDimWidth( components, paddingx, paddingy )
    local tw = 0
    local th = 0
    for _,c in pairs( components ) do     
        if not c.ignoreConstrict then
            if th < c.height then
                th = c.height
            end
            tw = tw + c.width + paddingx
        end
    end

    th = th 
    tw = tw - paddingx
    
    return tw, th
end

return U
