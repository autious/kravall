
local FlyBy = {}


function FlyBy:new(o)
    
    assert( o.scen, "FlyBy requires a scenario" )

    while scen.cameras["startfly_"..camindex] do
        local c = scen.cameras["startfly_"..camindex]
        scen.gamemode.camera:addInterpolationPoint(c.translation,c.quaternion,1)
    end
end

function FlyBy:destroy()

end

return FlyBy
