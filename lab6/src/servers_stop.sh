#!/bin/bash
for pid in $(pgrep server --port)
do
    kill $pid 2> /dev/null
done