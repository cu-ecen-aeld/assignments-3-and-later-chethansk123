#!/bin/sh
#Tester script for assignment 1 
#Author: Chethan Siriganahalli Karibasappa

filesdir=$1
searchstr=$2
if [ $# -eq 2 ]
then
	if [ -d "$filesdir" ]
	then 
		echo "The directory exsist"
		numfile=$(ls -1 "$filesdir"| wc -l)
		matching_lines=$(grep -r "$searchstr" "$filesdir" | wc -l)
		echo "The number of files are $numfile and the number of matching lines are $matching_lines"
		exit 0
	else
		echo "The directory does not exsist"
		exit 1
	fi
else
	echo "please enter two arguments"
	exit 1
fi

