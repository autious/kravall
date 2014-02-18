local U = {}

local HEX_PATTERN = "^#%x+$"
--Converts all hex in table to glm vec4
function U.convertToUsefulConstants( table )
    local new = {}

    for i,v in pairs( table ) do
        if type( v ) == "string" then 
            if  v:match( HEX_PATTERN ) then
                if not (v:len() == 7 or v:len() == 9) then
                    core.log.warning( "The element: " .. i .. " which has the value :" .. v ..
                                        " is expected to be hexadecimal but has an invalid length." )
                end
                new[i] = core.glm.vec4.new(U.fromHtmlHex(v))
            else
                new[i] = v
            end
        else
            new[i] = v
        end
    end

    return new
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

return U
