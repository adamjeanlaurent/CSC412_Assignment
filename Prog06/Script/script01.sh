# !bin/bash

# build version 1
g++ -Wall ../Version1/main.cpp ../Version1/gl_frontEnd.cpp ../Version1/validation.cpp -pthread -lGL -lglut -o cell