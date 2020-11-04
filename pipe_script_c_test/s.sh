#!/bin/bash

pipe=/tmp/myfifo

if [[ ! -p $pipe ]]; then
    echo "Reader not running"
    exit 1
fi

SIZEE=500

# echo ${#SIZEE} >$pipe
# echo $SIZEE >$pipe

echo "HELLOworld.job" > $pipe
echo "goodbye.job" > $pipe