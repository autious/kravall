local padding = 10
local defaultAnchor = "North"

local validPlacements = { "Center", "North", "West", "East", "South", "SouthEast", "SouthWest", "NorthEast" }

local placers = {}
for _,v in pairs( validPlacements ) do
    placers[v] = require( "gui/placement/" .. v .. "Placer" )  
end

return function( components, winWidth, winHeight, posx, posy )
    -- Bags for different regions
    local bags = {}
    
    -- Generate the bags
    for _,v in pairs( validPlacements ) do
        bags[v] = {}
    end

    -- Bag that all non anchored end up in
    local defaultBag = bags[defaultAnchor]

    -- Fill bags
    for _,comp in pairs( components ) do
        local bag = bags[comp.anchor]
        if type(bag) ~= "table" then
            defaultBag[#defaultBag+1] = comp  
        else
            bag[#bag+1] = comp
        end
    end 

    for index,bag in pairs(bags) do
        placers[index]( bag, winWidth, winHeight, posx, posy )
    end
end
