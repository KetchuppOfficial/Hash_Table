#!/bin/bash

# $1 - program to run
# $2 - file to write results into

make OPT=-O1
> $2
for ((i = 1; i <= 10; i++))
do
echo "Running measurement $i..."
/usr/bin/time -ao $2 -f "%e\t%U" $1
done
