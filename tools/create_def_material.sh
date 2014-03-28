#!/bin/bash

find ./ -name "*png" | awk '{
    n = split( $1, arr, "." )

    for( i = 1; i < n; i++ ){
        printf( "%s.", arr[i] )
    }

    printf( "material" )

    printf("\n")
}'
