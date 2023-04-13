#!/bin/bash
touch report.txt
echo "" > report.txt
num_width=1.0
num_of_children=100
test_file="integral"
i=0
while [ $i -le 2 ]
do
	echo "scale=10; $num_width/10" >> report.txt
	echo "100" >> report.txt
	./$test_file $num_width $num_of_children >> report.txt
	num_width=$(echo "scale=10; $num_width/10" | bc -l)
	i=$(( $i + 1 ))
done

