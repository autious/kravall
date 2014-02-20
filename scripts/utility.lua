local U = {}

local HEX_PATTERN = "^#%x+$"

function U.expandMixxedHexToVec4(data)
    local function hextolist( string )
        if type(string) == "string" then
            if  string:match( HEX_PATTERN ) then
                if not (string:len() == 7 or string:len() == 9) then
                    core.log.warning( "The element: " .. i .. " which has the value :" .. v ..
                                        " is expected to be hexadecimal but has a weird length." )
                end
                return {U.fromHtmlHexVariable(string)}
            end
        end
        return nil
    end

    local val = {}
    local currentIndex = 1
    for i,v in ipairs( data ) do
        local tmp = hextolist( v )
        if tmp then
            for i2,v2 in ipairs( tmp ) do
                val[currentIndex] = v2
                currentIndex = currentIndex + 1 
            end
        else
            val[currentIndex] = v
            currentIndex = currentIndex + 1 
        end
    end

    return core.glm.vec4.new(unpack(val))
end

function U.fromHtmlHexVariable( str, fourthDefault )
    local returns ={}
    --Loop over, 2 hex signs at at time, expecting a # in the beginning
    for i=1,math.floor(((string.len(str))-1)/2),1 do 
        returns[#returns+1] = tonumber( string.sub( str, i*2,i*2+1), 16 )/255  
    end
    return unpack( returns )
end

function U.fromHtmlHex( str )
    local fourth = 255
    if string.len( str ) >= 9  then
        fourth = tonumber( string.sub( str, 8,9), 16 )/255  
    end 
    return tonumber( string.sub( str, 2,3 ),16 )/255, tonumber( string.sub( str, 4,5 ),16 )/255, tonumber( string.sub( str, 6,7 ),16 )/255,fourth/255
end

function U.smoothStep( x )
    return ((x) * (x) * (3 - 2 * (x)))
end

function U.ripairs(t)
  local max = 1
  while t[max] ~= nil do
    max = max + 1
  end
  local function ripairs_it(t, i)
    i = i-1
    local v = t[i]
    if v ~= nil then
      return i,v
    else
      return nil
    end
  end
  return ripairs_it, t, max
end

function U.tableTrueCount( table )
    local count = 0

    for _,_ in pairs(table) do 
        count = count + 1
    end

    return count
end

return U
