
local function SetPosition(position, entity)
	local wpc;
    wpc = entity:get(core.componentType.WorldPositionComponent)
	wpc.position[1] = position[1]
	wpc.position[2] = position[2]
	wpc.position[3] = position[3]
	entity:set(core.componentType.WorldPositionComponent, wpc)
end


local function GetPosition(entity)
	local wpc;
    wpc = entity:get(core.componentType.WorldPositionComponent)
	return {wpc.position[1], wpc.position[2], wpc.position[3]}
	
end

local function SetLightIntensity(intensity, entity)
	local lc;
    lc = entity:get(core.componentType.LightComponent)
	lc.intensity = intensity
	entity:set(core.componentType.LightComponent, lc)
end

local function SetRotationZ(rot, entity)
	local rc;
    rc = entity:get(core.componentType.RotationComponent)
	rc.rotation = { 0,0, math.sin( rot * 0.5 ), math.cos(rot * 0.5) }
	entity:set(core.componentType.RotationComponent, rc)
end

local interface = { 
	SetPosition = SetPosition,
	GetPosition = GetPosition,
	SetLightIntensity = SetLightIntensity,
	SetRotationZ = SetRotationZ
 }
return interface