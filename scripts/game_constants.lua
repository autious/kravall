local vec4 = require( "utility" ).expandMixxedHexToVec4
return 
{
    standardPolice = 
    {
        maxHealth                       = 100,
        maxStamina                      = 100,
        maxMorale                       = 2.0,
        damageOutline                   = vec4{"#ff0000",3.0},
        selectionPrimaryOutline         = vec4{"#b2c441", 5.0},
        selectionOutline                = vec4{"#19e54c", 1.0},
        selectionBoxHoverOutline        = vec4{"#ffe614", 10.0},
        selectionBoxHoverPrimaryOutline = vec4{"#ffe614",10.0},
    },
    tearGasPolice = 
    {
        maxHealth                       = 100,
        maxStamina                      = 100,
        maxMorale                       = 2.0,
        damageOutline                   = vec4{"#ff0000",3.0},
        selectionPrimaryOutline         = vec4{"#b2c441", 5.0},
        selectionOutline                = vec4{"#19e54c", 1.0},
        selectionBoxHoverOutline        = vec4{"#ffe614", 10.0},
        selectionBoxHoverPrimaryOutline = vec4{"#ffe614",10.0},
        tearGasRange                    = 20,
    },
    guiBehaviour = 
    {
        damageBlinkingLinger = 5
    }
}
