#!/bin/bash

# get params
PATH_TO_DATA_FILES="$1"
OUTPUT_FILE_PATH="$2"

# temp directory to hold files created during execution
TEMP_DIR="../Source/Version2/temp/"

# create temp directory
mkdir "${TEMP_DIR}"

# get last char of param directories
LAST_CHAR_OF_PATH_TO_DATA_FILES="${PATH_TO_DATA_FILES: -1}"

# if last character of path is not a / , append / to the path
if [ "$LAST_CHAR_OF_PATH_TO_DATA_FILES" != "/" ] ; then
    PATH_TO_DATA_FILES="${PATH_TO_DATA_FILES}/"
fi

GREATEST_INDEX=0

# loop through .txt files in path to data files
for FILE in "$PATH_TO_DATA_FILES"*.txt ; do
    # get index of file
    FILE_INDEX=$(grep -o '^[0-9]*' $FILE)
    # if index of file is greatest than greatest index so far, overwrite it with index of file
    if [ $FILE_INDEX -gt $GREATEST_INDEX ] ;
    then
        GREATEST_INDEX=$FILE_INDEX
    fi
done

# build all execs
sh ./build.sh

# create output file
touch $OUTPUT_FILE_PATH

# run version 1
../Source/Version2/v2 $GREATEST_INDEX $PATH_TO_DATA_FILES $OUTPUT_FILE_PATH

# delete temp dir
# rm -r $TEMP_DIR

# test ./script02.sh /Users/ajean-laurent/Desktop/CSC_412_Work/Prog04/Data-Sets/Data-Set-1/ /Users/ajean-laurent/Desktop/CSC_412_Work/Prog04/Source/Version2/outputfile.c
