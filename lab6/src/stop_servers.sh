#!/bin/bash
for pid in $(pgrep server) #я писал этот код в vscode и там, для вычленения процессов я писал $(pgrep server --port). Здесь
do                         #он так сделать не даёт
    kill $pid 2> /dev/null
done