
ents = {}

for i = -100, 100 do
    for j = -100, 100 do
        local e = core.entity.create( core.componentType.WorldPositionComponent )

        ents[e] = { position = {i,j,i*j} }

        e:set( core.componentType.WorldPositionComponent, ents[e] )
       
    end
end

        
for k,v in pairs( ents ) do
    local s = k:get( core.componentType.WorldPositionComponent, ents[e] )

    print "is?"
    for i = 1, 4 do 
        if s.position[i] ~= v.position[i] then
            print "B0rk";
        end
    end
end
