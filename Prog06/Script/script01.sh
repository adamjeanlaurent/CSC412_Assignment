# !bin/bash

WIDTH=$1
HEIGHT=$2
NUM_THREADS=$3

# ensure we have 3 arguments
if [ "$#" -ne 3 ]; then
	echo "usage: ./script01.sh <WIDTH> <HEIGHT> <NUMTHREADS>"
	exit 1
fi

# create pipe
write_pipe=/tmp/bash_to_c

if [[ ! -p $write_pipe ]]; then
    mkfifo $write_pipe
fi

USER_INPUT=""

# launch cellular automation process version 1
./tmp/cellv1 $WIDTH $HEIGHT $NUM_THREADS &

while true; do
	read USER_INPUT
	echo $USER_INPUT > $write_pipe

	# quit script if user typed in "end"
	if [ "${USER_INPUT}" == "end" ]; then
  		exit 1
	fi
done