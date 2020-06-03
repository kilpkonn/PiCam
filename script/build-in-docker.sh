#!/bin/bash

docker build -t raspbian_opencv_build .
docker create --name extract raspbian_opencv_build
docker cp extract:/opencv-4.3.0 ./
docker rm extract