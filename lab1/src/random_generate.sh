#!/bin/bash
for i in `seq 50`; 
do 
od -An -N1 -i /dev/random; 
done > numbers.txt
