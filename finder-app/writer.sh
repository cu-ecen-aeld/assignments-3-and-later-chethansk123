#!/bin/bash

writefile=$1
writestr=$2

if [ $# -eq 2 ]
then
	dir=$(dirname "$writefile")
	if ! mkdir -p "$dir"
	then 
		echo "failed to create the directory path $dir"
		exit 1
	fi

	if ! echo "$writestr" > "$writefile"
	then 
		echo "failed to write the string in the file $writefile"
		exit 1
	else
		echo "String $writestr is written successfully in $writefile"
		exit 0
	fi

else
	echo "please provide both file path and the string to write"
	exit 1
fi
