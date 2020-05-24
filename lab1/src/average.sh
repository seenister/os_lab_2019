#!/bin/bash

sum=0
for (( number = 1; number <= $#; number++ ))
do
sum=$[$sum+${!number}]
done

echo $[$sum/$#]
echo $#
