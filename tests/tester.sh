#!/bin/bash

for f in $(ls *.in | sort -n); do

    echo $f
    
    ..//build//origami.exe < $f > ori.out
    
    echo ${f%.in}.out
    
    if diff -wq ori.out ${f%.in}.out; then
        echo Accepted!
    else
        break
    fi
    
done
