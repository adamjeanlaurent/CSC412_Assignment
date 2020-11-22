# !bin/bash

# create temp dir if it doesn't exist
if [ ! -d "./tmp/" ]; then
    mkdir tmp
fi

# build version 1
g++ -Wall ../Version1/main.cpp ../Version1/gl_frontEnd.cpp ../Version1/validation.cpp -pthread -lGL -lglut -o ./tmp/cellv1

# build version 2
g++ -Wall ../Version2/main.cpp ../Version2/gl_frontEnd.cpp ../Version2/validation.cpp -pthread -lGL -lglut -o ./tmp/cellv2

# build version 3
g++ -Wall ../Version3/main.cpp ../Version3/gl_frontEnd.cpp ../Version3/validation.cpp -pthread -lGL -lglut -o ./tmp/cellv3