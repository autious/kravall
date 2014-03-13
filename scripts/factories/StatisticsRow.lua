
local StatisticsRow =	
					{ 
						title="Statistics title",
						maxResult=-1,
						achievedResult=-1
					}

function StatisticsRow:new(o)
    o = o or {}

    setmetatable( o, self )
    self.__index = self
    
    return o
end

return StatisticsRow
