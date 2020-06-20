#!/bin/bash
rm -rf build-armhf
mkdir build-armhf

docker build -t raspbian_opencv_build -f docker/Dockerfile-armhf .
docker run -it --rm --name extract -v "$PWD/build-armhf:/export" raspbian_opencv_build
docker image rm raspbian_opencv_build

chmod +x -R build-armhf
cp ./script/opencv.pc ./build-armhf
cp ./script/pi-setup.sh ./build-armhf

echo "Done building openCV for armhf!"
echo "Copy contents of ./build-armhf to raspberry"