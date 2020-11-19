# !bin/bash

WIDTH=$1
HEIGHT=$2
NUM_THREADS=$3

if [ ! -d "./tmp/" ]; then
    mkdir tmp
fi

# create pipe
write_pipe=/tmp/bash_to_c

if [[ ! -p $write_pipe ]]; then
    mkfifo $write_pipe
fi

USER_INPUT=""

while true; do
	read USER_INPUT
	echo $USER_INPUT > write_pipe
done