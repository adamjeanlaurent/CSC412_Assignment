#!/bin/bash

IMAGE_LIBRARY=$1
LAST_CHAR_OF_IMAGE_LIBRARY_PATH="${IMAGE_LIBRARY: -1}"

if [ "$LAST_CHAR_OF_IMAGE_LIBRARY_PATH" != "/" ] ; then
    IMAGE_LIBRARY="${IMAGE_LIBRARY}/"
fi

IMAGE_LIBRARY_PATH="${IMAGE_LIBRARY}"

# build static library
ar rvs "${IMAGE_LIBRARY_PATH}lib/StaticImageLib.a" "${IMAGE_LIBRARY_PATH}lib/static/ImageIO.o" "${IMAGE_LIBRARY_PATH}lib/static/ImageIO_TGA.o" "${IMAGE_LIBRARY_PATH}lib/static/RasterImage.o" "${IMAGE_LIBRARY_PATH}lib/static/comp.o" "${IMAGE_LIBRARY_PATH}lib/static/crop.o" "${IMAGE_LIBRARY_PATH}lib/static/mirrorH.o" "${IMAGE_LIBRARY_PATH}lib/static/mirrorV.o" "${IMAGE_LIBRARY_PATH}lib/static/gray.o"

g++ -std=c++17 -I "${IMAGE_LIBRARY_PATH}include" "${IMAGE_LIBRARY_PATH}applications/comp.cpp" "${IMAGE_LIBRARY_PATH}lib/StaticImageLib.a" -o test