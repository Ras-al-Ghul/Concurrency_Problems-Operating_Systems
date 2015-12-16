#!/bin/bash

stress -i 50 -t 11000 &
while(true)
do
    3>>HogIO.log perf stat --log-fd 3 -e cpu-clock,task-clock,context-switches -p 3033 sleep 10
done
