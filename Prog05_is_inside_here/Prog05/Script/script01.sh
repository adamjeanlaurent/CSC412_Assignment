#!/bin/bash

IMAGE_LIBRARY=$1
LAST_CHAR_OF_IMAGE_LIBRARY_PATH="${IMAGE_LIBRARY: -1}"

if [ "$LAST_CHAR_OF_IMAGE_LIBRARY_PATH" != "/" ] ; then
    IMAGE_LIBRARY="${IMAGE_LIBRARY}/"
fi

IMAGE_LIBRARY_PATH="${IMAGE_LIBRARY}"

# build static library

# build .o files from source
g++ -I "${IMAGE_LIBRARY_PATH}include/" -c "${IMAGE_LIBRARY_PATH}src/ImageIO.cpp" -o "${IMAGE_LIBRARY_PATH}lib/static/ImageIO.o"
g++ -I "${IMAGE_LIBRARY_PATH}include/" -c "${IMAGE_LIBRARY_PATH}src/ImageIO_TGA.cpp" -o "${IMAGE_LIBRARY_PATH}lib/static/ImageIO_TGA.o"
g++ -I "${IMAGE_LIBRARY_PATH}include/" -c "${IMAGE_LIBRARY_PATH}src/RasterImage.cpp" -o "${IMAGE_LIBRARY_PATH}lib/static/RasterImage.o"
g++ -I "${IMAGE_LIBRARY_PATH}include/" -c "${IMAGE_LIBRARY_PATH}src/utilities.cpp" -o "${IMAGE_LIBRARY_PATH}lib/static/utilities.o"

# build .a file 
ar rvs "${IMAGE_LIBRARY_PATH}lib/StaticImageLib.a" "${IMAGE_LIBRARY_PATH}lib/static/ImageIO.o" "${IMAGE_LIBRARY_PATH}lib/static/ImageIO_TGA.o" "${IMAGE_LIBRARY_PATH}lib/static/RasterImage.o" "${IMAGE_LIBRARY_PATH}lib/static/utilities.o"

# build applications against static library
g++ -I "${IMAGE_LIBRARY_PATH}include/" "${IMAGE_LIBRARY_PATH}applications/comp.cpp" "${IMAGE_LIBRARY_PATH}lib/StaticImageLib.a" -o comp
g++ -I "${IMAGE_LIBRARY_PATH}include/" "${IMAGE_LIBRARY_PATH}applications/crop.cpp" "${IMAGE_LIBRARY_PATH}lib/StaticImageLib.a" -o crop
g++ -I "${IMAGE_LIBRARY_PATH}include/" "${IMAGE_LIBRARY_PATH}applications/flipH.cpp" "${IMAGE_LIBRARY_PATH}lib/StaticImageLib.a" -o flipH
g++ -I "${IMAGE_LIBRARY_PATH}include/" "${IMAGE_LIBRARY_PATH}applications/flipV.cpp" "${IMAGE_LIBRARY_PATH}lib/StaticImageLib.a" -o flipV
g++ -I "${IMAGE_LIBRARY_PATH}include/" "${IMAGE_LIBRARY_PATH}applications/gray.cpp" "${IMAGE_LIBRARY_PATH}lib/StaticImageLib.a" -o gray
g++ -I "${IMAGE_LIBRARY_PATH}include/" "${IMAGE_LIBRARY_PATH}applications/rotate.cpp" "${IMAGE_LIBRARY_PATH}lib/StaticImageLib.a" -o rotate

# build shared library

#  build versions against static library

# build version 1
g++ ../Version1/main.cpp ../Version1/dispatcher.cpp ../Version1/job.cpp ../Version1/utility.cpp "${IMAGE_LIBRARY_PATH}lib/StaticImageLib.a"  -o v1


