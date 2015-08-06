#!/bin/bash

MIN=20
MAX=21

for i in `seq $1 $2`;
do
    echo Depth $i >> timing.log
    { time ./test -t 12 -d $i; } 2>> timing.log
    echo '' >> timing.log
done
