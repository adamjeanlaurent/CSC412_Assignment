# !bin/bash

# build version 1
g++ -std=c++17 -Wall ../Version1/main.cpp ../Version1/gl_frontEnd.cpp ../Version1/validation.cpp -lGL -lglut -o cell