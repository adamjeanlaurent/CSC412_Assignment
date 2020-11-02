#!bin/bash

DROP_FOLDER="$1"
DATA_FOLDER="$2"
OUTPUT_FOLDER="$3"

PATHS_ARE_SAME_ERROR_MSG="Error: All Paths Must Be Different!"

# Ensure paths are different for passed directories
if [[ DROP_FOLDER -ef DATA_FOLDER ]]; then 
	echo $PATHS_ARE_SAME_ERROR_MSG
	exit 1
fi

if [[ DROP_FOLDER -ef OUTPUT_FOLDER ]]; then 
	echo $PATHS_ARE_SAME_ERROR_MSG
	exit 1
fi

if [[ DATA_FOLDER -ef OUTPUT_FOLDER ]]; then 
	echo $PATHS_ARE_SAME_ERROR_MSG
	exit 1
fi

# create directories if they don't already exist
if [ ! -d "$OUTPUT_FOLDER" ]; then
    mkdir $OUTPUT_FOLDER
fi

if [ ! -d "$DATA_FOLDER" ]; then
    mkdir $DATA_FOLDER
fi

if [ ! -d "$DROP_FOLDER" ]; then
    mkdir $DROP_FOLDER
fi

# watch drop folder
inotifywait -m $DROP_FOLDER -e create -e moved_to |
    while read path action file; do
        if [[ "$file" =~ .*txt$ ]]; then # Does the file end with .xml?
            echo "txt file" # If so, do your thing here!
        fi
    done