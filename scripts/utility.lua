local U = {}


function U.fromHtmlHex( str )
return tonumber( string.sub( str, 2,3 ),16 )/255, tonumber( string.sub( str, 4,5 ),16 )/255, tonumber( string.sub( str, 6,7 ),16 )/255,1
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
