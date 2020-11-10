#!/bin/bash

pipe=/tmp/myfifo

# create pipe if it doesn't exist

if [[ ! -p $pipe ]]; then
    mkfifo $pipe
fi

if read line <$pipe; then
    echo "$line"
fi


if read line <$pipe; then
    echo "$line"
fi


# echo "Hello from bash script" >$pipe

echo "done reading"