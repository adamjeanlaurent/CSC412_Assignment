#!/bin/bash

gcc -Wall -o ../Source/Version1/v1 ../Source/Version1/main_V1.c ../Source/fileSystemHandler.c ../Source/processing.c ../Source/distributor.c ../Source/splitWork.c

gcc -Wall -o ../Source/Version2/v2 ../Source/Version2/main_V2.c ../Source/fileSystemHandler.c ../Source/processing.c ../Source/distributor.c ../Source/splitWork.c