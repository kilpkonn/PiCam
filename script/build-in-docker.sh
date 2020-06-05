#!/bin/bash
rm -rf build
mkdir build

docker build -t raspbian_opencv_build .
docker run -it --rm --name extract -v "$PWD"/build:/extract raspbian_opencv_build
docker image rm raspbian_opencv_build

chmod +x -R build
cp ./script/opencv.pc ./build

echo "Done building openCV for armhf!"
echo "Copy contents of ./build to raspberry"