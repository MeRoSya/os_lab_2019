#!/bin/bash
#запускает несколько серверов, с перечисленными в файле портами
while IFS=: read -r port ip
do
    ./server --port $port --tnum 100 &
done < servers
