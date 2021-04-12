#!/bin/bash
for pid in $(pgrep server)
do                         
    kill $pid 2> /dev/null
done