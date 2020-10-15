#!/bin/bash

# The following script will take in two arguments
# 1: path
# 2: ext
# The script will echo all files with the given extension in the given folder in the following format
# Looking for files with extension .txt in folder
#   /usr/jyh/CSC402/Prog02/TestFolder:
# 3 files found:
#   myFile.txt
#   someFile.txt
#   anotherFile.txt

# Check that the program was given exactly two arguments
if [ $# -gt 2 ] || [ $# -lt 2 ] ; then
    echo "usage: ./script02.sh <path> <extension>"
    exit 
fi

# Will hold array of files in directory
FILES=()

# last character of the folder path given
LastChar="${1: -1}"

# Script supports paths ending in / and those that don't
# If the path does not end in / then when looping through the directory, add /
if [ "$LastChar" != "/" ]
then
    # Path does not end in / , add / to loop statement
    for FILE in $"$1"/*."$2" ; do
        # parse filename to remove full path
        parsedFileName=$(basename -- $FILE)
        # if valid filename, add to array of files
        if [ $parsedFileName != "*.$2" ]
        then
            FILES+=( "$parsedFileName" )
        fi
    done

 else
    # path does end in / then loop through it
    for FILE in $"$1"*."$2" ; do
        # parse filename to remove full path
        parsedFileName=$(basename -- $FILE)
        # if valid filename, add to array of files
        if [ $parsedFileName != "*.$2" ]
        then
            FILES+=( "$parsedFileName" )
        fi
    done
fi

echo "Lookng for files with extension .${2} in folder"
echo -e ' \t ' $1:

# holds length of files array
LEN=${#FILES[@]}

if [ $LEN -lt 1 ]
    # no files found
then
    echo "No file found."

elif [ $LEN -lt 2 ]
    # one file found
then
    echo "1 file found:"

else
    # multiple files found
    echo $LEN "files found:"
fi

# if 1 or multiple files found, loop through FILES array, and print each file name
for FILE in ${FILES[@]} ; do
    echo -e ' \t ' $FILE
done
