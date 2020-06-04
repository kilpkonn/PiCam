#!/bin/bash
rm -rf build
mkdir build

docker build -t raspbian_opencv_build .
docker run -it --rm --name extract -v "$PWD"/build:/extract raspbian_opencv_build
docker image rm raspbian_opencv_build