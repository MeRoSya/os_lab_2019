#!/bin/bash
sum=0
for param in "$@"
do
sum=$(($sum+$param))
done
echo Average is $(($sum/$#))
