#!/bin/bash

IFS=$'\n';

for i in $(ls *png | awk '
BEGIN{
    base = 50
}
{
    if( NR == 1 ){
        for( i = 1; i < base; i++ ){
            printf( "%s ordered/%.5d.png\n", $1, NR+i)
        }
    }

    prev = $1
    printf( "%s ordered/%.5d.png\n", $1, NR+base)
}
END{
    for( i = 1; i < base; i++ ){
        printf( "%s ordered/%.5d.png\n", prev, NR+i+base)
    }
            
}'); 
do 
    stamp=$(echo $i | awk '{
        n=split($i,arr,"-")

        print arr[1] "-" arr[2] "-" arr[3] "-" substr(arr[4],1,2) "00"
    }')

    from=$(echo $i | awk '{ print $1 }')
    to=$(echo $i | awk '{ print $2 }')

    echo $stamp
    echo $from
    echo $to

    cp $from $to
#    convert $from -gravity south \
#              -stroke '#000C' -strokewidth 2 -pointsize 16 -annotate 0 "$stamp" \
#              -stroke  none   -fill white    -annotate 0 "$stamp" \
#              $to
done

#convert dragon.gif -gravity south \
#          -stroke '#000C' -strokewidth 2 -annotate 0 'Faerie Dragon' \
#          -stroke  none   -fill white    -annotate 0 'Faerie Dragon' \
#          anno_outline.jpg
