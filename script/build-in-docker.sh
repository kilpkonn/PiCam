#!/bin/bash

docker build -t raspbian_opencv_build .
docker run -it --rm --name extract -v build:/extract raspbian_opencv_build
docker image rm raspbian_opencv_build