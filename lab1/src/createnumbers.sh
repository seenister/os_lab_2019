
#!/bin/bash

for (( number = 1; number <= 150; number++ ))
do
	str=$str" $RANDOM"
done

echo $str > numbers.txt
