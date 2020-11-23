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
read_pipe=/tmp/c_to_bash

if [[ ! -p $write_pipe ]]; then
    mkfifo $write_pipe
fi

if [[ ! -p $read_pipe ]]; then
    mkfifo $read_pipe
fi

USER_INPUT=""

# launch cellular automation process version 1
# ./tmp/cellv1 $WIDTH $HEIGHT $NUM_THREADS &

while true; do
	echo "-------------------------------" 
	echo "Command: "
	# read user input from command line
	read USER_INPUT

	# send user input through pipe to CA
	echo $USER_INPUT > $write_pipe

	echo "running command......"

	# read from pipe, no need to do anything, we just want to wait for a result before we accept another command
	if read line <$read_pipe; then
		echo "command ran!"
	fi
	
	# quit script if user typed in "end"
	if [ "${USER_INPUT}" == "end" ]; then
  		exit 1
	fi
done