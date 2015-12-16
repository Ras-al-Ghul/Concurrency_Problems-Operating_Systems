#! /bin/sh

MAX_LINES_PER_CHUNK=70000000
ORIGINAL_FILE=$1
SORTED_FILE=$2
NUM_OF_LINES_IN_FILE=$(wc -l < $ORIGINAL_FILE)

usage ()
{
     echo Merge sort
     echo usage: mergesortfiles file1 file2
     echo Sorts text file file1 and stores the output in file2
}

# test if we have two arguments on the command line
if [ $# != 2 ]
then
    usage
    exit
fi

#Cleanup any lefover files
#rm -f $1?* > /dev/null

commandsort()
{
	sort $1 > $1.sorted &
	return
}

merge_sort()
{
	if [ $2 -le $MAX_LINES_PER_CHUNK ]
	then
		 commandsort $1
		 return
	elif [ $2 -gt $MAX_LINES_PER_CHUNK ]
	then
		HALF_LINE_VALUE=$(($2/2))
		split -l $HALF_LINE_VALUE $1 $1

		for file in $1??*
		do
			TEMP_NUM_OF_LINES=$(wc -l < $file)
			merge_sort $file $TEMP_NUM_OF_LINES
		done
		wait
	fi
}

merge_sort $ORIGINAL_FILE $NUM_OF_LINES_IN_FILE
sort -m $1*.sorted > $SORTED_FILE

rm -f $1?* > /dev/null
