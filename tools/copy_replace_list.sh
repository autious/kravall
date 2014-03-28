#!/bin/bash

# Uses parameter one, to replace with string in paramter to in the following listed names and
# copies the files into new files with the new name
 
for i in "${@:3}"; do

    NEWFILE=$(echo $i | awk -v from=$1 -v to=$2 '{
        gsub( from, to, $1)
        printf( "%s\n", $1 )
    }')

    if [[ -e $NEWFILE ]]; then
        echo "Warning, file:" $NEWFILE "already exists, skipping"
    fi
    
    echo $i | awk -v from=$1 -v to=$2 '{
        printf( "%s ", $1 )
        gsub( from, to, $1)
        printf( "%s\0", $1 )
    }' | xargs cp -n
done


#for V in $VALUE; do
#    echo $V
#done
