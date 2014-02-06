local AnchorPlacer = { padding = 10, defaultAnchor = "North" }

local validPlacements = { "Center", "North", "West", "East", "South", "SouthEast", "SouthWest", "NorthEast" }

AnchorPlacer.placers = {}
for _,v in pairs( validPlacements ) do
    AnchorPlacer.placers[v] = require( "gui/placement/" .. v .. "Placer" )  
end

function AnchorPlacer:new(o)
    o = o or {}

    setmetatable( o, self )
    self.__index = self

    return o
end

function AnchorPlacer:constrict( components, winWidth, winHeight )
    -- Bags for different regions
    local bags = {}
    
    -- Generate the bags
    for _,v in pairs( validPlacements ) do
        bags[v] = {}
    end

    -- Bag that all non anchored end up in
    local defaultBag = bags[self.defaultAnchor]

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
        self.placers[index]:constrict( bag, winWidth, winHeight )
    end
end

function AnchorPlacer:getTotalDim( bag )
    local tw = 0
    local th = 0
    for _,c in pairs( bag ) do     
        if tw < c.width then
            tw = c.width
        end
        th = th + c.height + self.padding
    end
    
    return tw, th
end

return AnchorPlacer
