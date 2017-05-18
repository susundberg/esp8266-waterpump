#!/bin/bash
for i in `seq 100`
do
   echo "Welcome $i times"
   curl $1
done

