#!/bin/bash

IMAGE_LIBRARY=$1
LAST_CHAR_OF_IMAGE_LIBRARY_PATH="${IMAGE_LIBRARY: -1}"

if [ "$LAST_CHAR_OF_IMAGE_LIBRARY_PATH" != "/" ] ; then
    IMAGE_LIBRARY="${IMAGE_LIBRARY}/"
fi

IMAGE_LIBRARY_PATH="${IMAGE_LIBRARY}"

if [ ! -d "./tmp/" ]; then
    mkdir tmp
fi

# build static library

# build .o files from source
g++ -I "${IMAGE_LIBRARY_PATH}include/" -c "${IMAGE_LIBRARY_PATH}src/ImageIO.cpp" -o ./tmp/ImageIO.o
g++ -I "${IMAGE_LIBRARY_PATH}include/" -c "${IMAGE_LIBRARY_PATH}src/ImageIO_TGA.cpp" -o ./tmp/ImageIO_TGA.o
g++ -I "${IMAGE_LIBRARY_PATH}include/" -c "${IMAGE_LIBRARY_PATH}src/RasterImage.cpp" -o ./tmp/RasterImage.o
g++ -I "${IMAGE_LIBRARY_PATH}include/" -c "${IMAGE_LIBRARY_PATH}src/utilities.cpp" -o ./tmp/utilities.o

# build .a file 
ar rvs "${IMAGE_LIBRARY_PATH}lib/static/StaticImageLib.a" ./tmp/ImageIO.o ./tmp/ImageIO_TGA.o ./tmp/RasterImage.o ./tmp/utilities.o

# build applications against static library
g++ -I "${IMAGE_LIBRARY_PATH}include/" "${IMAGE_LIBRARY_PATH}applications/comp.cpp" "${IMAGE_LIBRARY_PATH}lib/static/StaticImageLib.a" -o ./tmp/comp
g++ -I "${IMAGE_LIBRARY_PATH}include/" "${IMAGE_LIBRARY_PATH}applications/crop.cpp" "${IMAGE_LIBRARY_PATH}lib/static/StaticImageLib.a" -o ./tmp/crop
g++ -I "${IMAGE_LIBRARY_PATH}include/" "${IMAGE_LIBRARY_PATH}applications/flipH.cpp" "${IMAGE_LIBRARY_PATH}lib/static/StaticImageLib.a" -o ./tmp/flipH
g++ -I "${IMAGE_LIBRARY_PATH}include/" "${IMAGE_LIBRARY_PATH}applications/flipV.cpp" "${IMAGE_LIBRARY_PATH}lib/static/StaticImageLib.a" -o ./tmp/flipV
g++ -I "${IMAGE_LIBRARY_PATH}include/" "${IMAGE_LIBRARY_PATH}applications/gray.cpp" "${IMAGE_LIBRARY_PATH}lib/static/StaticImageLib.a" -o ./tmp/gray
g++ -I "${IMAGE_LIBRARY_PATH}include/" "${IMAGE_LIBRARY_PATH}applications/rotate.cpp" "${IMAGE_LIBRARY_PATH}lib/static/StaticImageLib.a" -o ./tmp/rotate

# build version 1 against static library
g++ ../Version1/main.cpp ../Version1/dispatcher.cpp ../Version1/job.cpp ../Version1/utility.cpp "${IMAGE_LIBRARY_PATH}lib/static/StaticImageLib.a"  -o ./tmp/v1

# build version 2 against static library
g++ ../Version2/main.cpp ../Version2/dispatcher.cpp ../Version2/job.cpp ../Version2/utility.cpp "${IMAGE_LIBRARY_PATH}lib/static/StaticImageLib.a"  -o ./tmp/v2

# build version 3 against static library
g++ ../Version3/main.cpp ../Version3/dispatcher.cpp ../Version3/job.cpp ../Version3/utility.cpp ../Version3/pipe.cpp "${IMAGE_LIBRARY_PATH}lib/static/StaticImageLib.a"  -o ./tmp/v3

# version 3 resident dispatchers
g++ ../Version3/rotate_resident_dispatcher.cpp ../Version3/dispatcher.cpp ../Version3/job.cpp ../Version3/utility.cpp ../Version3/pipe.cpp "${IMAGE_LIBRARY_PATH}lib/static/StaticImageLib.a"  -o ./tmp/rotateResDis
g++ ../Version3/crop_resident_dispatcher.cpp ../Version3/dispatcher.cpp ../Version3/job.cpp ../Version3/utility.cpp ../Version3/pipe.cpp "${IMAGE_LIBRARY_PATH}lib/static/StaticImageLib.a"  -o ./tmp/cropResDis
g++ ../Version3/flipV_resident_dispatcher.cpp ../Version3/dispatcher.cpp ../Version3/job.cpp ../Version3/utility.cpp ../Version3/pipe.cpp "${IMAGE_LIBRARY_PATH}lib/static/StaticImageLib.a"  -o ./tmp/flipVResDis
g++ ../Version3/flipH_resident_dispatcher.cpp ../Version3/dispatcher.cpp ../Version3/job.cpp ../Version3/utility.cpp ../Version3/pipe.cpp "${IMAGE_LIBRARY_PATH}lib/static/StaticImageLib.a"  -o ./tmp/flipHResDis
g++ ../Version3/gray_resident_dispatcher.cpp ../Version3/dispatcher.cpp ../Version3/job.cpp ../Version3/utility.cpp ../Version3/pipe.cpp "${IMAGE_LIBRARY_PATH}lib/static/StaticImageLib.a"  -o ./tmp/grayResDis

# build .o files from source with -fPIC
g++ -I "${IMAGE_LIBRARY_PATH}include/" -fno-use-cxa-atexit -fPIC -c "${IMAGE_LIBRARY_PATH}src/ImageIO.cpp" -o ./tmp/ImageIO.o
g++ -I "${IMAGE_LIBRARY_PATH}include/" -fno-use-cxa-atexit -fPIC -c "${IMAGE_LIBRARY_PATH}src/ImageIO_TGA.cpp" -o ./tmp/ImageIO_TGA.o
g++ -I "${IMAGE_LIBRARY_PATH}include/" -fno-use-cxa-atexit -fPIC -c "${IMAGE_LIBRARY_PATH}src/RasterImage.cpp" -o ./tmp/RasterImage.o
g++ -I "${IMAGE_LIBRARY_PATH}include/" -fno-use-cxa-atexit -fPIC -c "${IMAGE_LIBRARY_PATH}src/utilities.cpp" -o ./tmp/utilities.o

# build shared library .so
ld -shared ./tmp/ImageIO.o ./tmp/ImageIO_TGA.o ./tmp/RasterImage.o ./tmp/utilities.o  -o "${IMAGE_LIBRARY_PATH}lib/shared/SharedImageLib.so"