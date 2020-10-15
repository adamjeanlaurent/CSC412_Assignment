# get params
EXEC_NAME=$1
PATH_TO_IMAGES="$2"
PATH_TO_PATTERNS="$3"
FINAL_OUTPUT_DIR="$4"

# temp directories to hold output files
TEMP_DIR_FOR_IMAGES="../temp_dir_images"
TEMP_DIR_FOR_PATTERNS="../temp_dir_patterns"

# get last character of paths
LAST_CHAR_OF_IMAGES_PATH="${PATH_TO_IMAGES: -1}"
LAST_CHAR_OF_PATTERNS_PATH="${PATH_TO_PATTERNS: -1}"
LAST_CHAR_OF_OUTPUT_PATH="${FINAL_OUTPUT_DIR: -1}"

# if last char of path is not a / , append / to the path
if [ "$LAST_CHAR_OF_IMAGES_PATH" != "/" ] ; then
    PATH_TO_IMAGES="${PATH_TO_IMAGES}/"
fi

if [ "$LAST_CHAR_OF_PATTERNS_PATH" != "/" ] ; then
    PATH_TO_PATTERNS="${PATH_TO_PATTERNS}/"
fi

if [ "$LAST_CHAR_OF_OUTPUT_PATH" != "/" ] ; then
    FINAL_OUTPUT_DIR="${FINAL_OUTPUT_DIR}/"
fi

# create temp directory for patterns
mkdir "${TEMP_DIR_FOR_PATTERNS}"

for FILE in "$PATH_TO_PATTERNS"*.pat ; do
    parsedFileName=$(basename -- $FILE)
    outputFileName="${TEMP_DIR_FOR_PATTERNS}/${parsedFileName}"
    touch $outputFileName

    while read line || [ -n "$line" ];
     do
        echo ${line} | tr -s '\r' '\n'

    done < "$FILE" >"${outputFileName}" 
done

# create temp directory for images
mkdir "${TEMP_DIR_FOR_IMAGES}"

for FILE in "$PATH_TO_IMAGES"*.img ; do
    parsedFileName=$(basename -- $FILE)
    outputFileName="${TEMP_DIR_FOR_IMAGES}/${parsedFileName}"
    touch $outputFileName

    while read line || [ -n "$line" ];
     do 
        echo ${line} | tr -s '\r' '\n'

    done < "$FILE" >"${outputFileName}" 
done

# copy pattern files into orig dir
for FILE in "$TEMP_DIR_FOR_PATTERNS"/*.pat ; do
    parsedFileName=$(basename -- $FILE)
    fileToReplace="${PATH_TO_PATTERNS}${parsedFileName}"
    cat $FILE > $fileToReplace
done

# copy images files into orig dir
for FILE in "$TEMP_DIR_FOR_IMAGES"/*.img ; do
    parsedFileName=$(basename -- $FILE)
    fileToReplace="${PATH_TO_IMAGES}${parsedFileName}"
    cat $FILE > $fileToReplace
done

# delete temp directories
rm -r $TEMP_DIR_FOR_IMAGES
rm -r $TEMP_DIR_FOR_PATTERNS

#  build c program
gcc -Wall -o $EXEC_NAME main.c fileHandler.c patternMatching.c

if [ ! -d "$FINAL_OUTPUT_DIR" ]; then
    # create output directory if doesn't exist
    mkdir $FINAL_OUTPUT_DIR
fi

# run program for all patterns concurrently
for FILE in "$PATH_TO_PATTERNS"*.pat ; do
    ./main $FILE $PATH_TO_IMAGES $FINAL_OUTPUT_DIR &
done