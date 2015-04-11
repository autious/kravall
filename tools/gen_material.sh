#!/bin/bash

echo "args"

for VAR; do
    MATERIAL=$(echo $VAR | awk '{
        n = split( $1, arr, "." )

        for( i = 1; i < n; i++ ){
            printf( "%s.", arr[i] )
        }

        printf( "material" )
    }')

    if [[ -e $MATERIAL ]]; then
        echo "Already exists"
    else
        echo $VAR | awk '{
            n = split( $1, arr, "/" ) 

            printf( "diffuse = \"assets/texture/ui/%s\"\n", arr[n] )
            printf( "shader = \"Overlay\"\n" )
        }' > $MATERIAL
    fi
done


