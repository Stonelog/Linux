#!/bin/bash

i=0

arr=("|" "/" "-" "\\")

str=' '
index=0

while [ $i -le 100 ]
do
	let index%=4;
	printf "[%-101s] [%d%%] [%c]\r"  "$str"  "$i"  "${arr[$index]}"
	str="="$str

	let i++
	let index++
	sleep 0.1

done


