#!/bin/bash
while IFS=: read -r port ip
do
    ./server --port $port --tnum 5 &
done < servers
