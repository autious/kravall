
local StatisticsRow =	
					{ 
						title="Statistics title",
						resultTitle="No Result",
						maxResult=0,
						achievedResult=0
					}

function StatisticsRow:new(o)
    o = o or {}

    setmetatable( o, self )
    self.__index = self
    
    return o
end

return StatisticsRow
