local U = {}


function U.fromHtmlHex( str )
return tonumber( string.sub( str, 2,3 ),16 )/255, tonumber( string.sub( str, 4,5 ),16 )/255, tonumber( string.sub( str, 6,7 ),16 )/255,1
end

return U
