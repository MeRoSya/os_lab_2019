#!/bin/bash
for pid in $(pgrep udpserver)
do                         
    kill $pid 2> /dev/null
done
for pid in $(pgrep tcpserver)
do                         
    kill $pid 2> /dev/null
done