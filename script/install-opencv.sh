#!/bin/bash

echo "Installing OpenCV"
# Define OpenCV Version to install
cvVersion="master"

mkdir -p ~/install/opencv

# Clean build directories
rm -rf ~/install/opencv
rm -rf ~/install/opencv_contrib

rm -rf /opt/opencv-4.3.0

sudo apt update && sudo apt upgrade -y

sudo apt -y remove x264 libx264-dev

## Install dependencies
sudo apt -y install build-essential checkinstall cmake pkg-config yasm
sudo apt -y install git gfortran
sudo apt -y install libjpeg8-dev libpng-dev

sudo apt -y install software-properties-common
sudo add-apt-repository "deb http://security.ubuntu.com/ubuntu xenial-security main"
sudo apt -y update

sudo apt -y install libjasper1
sudo apt -y install libtiff-dev

sudo apt -y install libavcodec-dev libavformat-dev libswscale-dev libdc1394-22-dev
sudo apt -y install libxine2-dev libv4l-dev
cd /usr/include/linux || exit
sudo ln -s -f ../libv4l1-videodev.h videodev.h
cd ~/install || exit

sudo apt -y install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev
sudo apt -y install libgtk2.0-dev libtbb-dev qt5-default
sudo apt -y install libatlas-base-dev
sudo apt -y install libfaac-dev libmp3lame-dev libtheora-dev
sudo apt -y install libvorbis-dev libxvidcore-dev
sudo apt -y install libopencore-amrnb-dev libopencore-amrwb-dev
sudo apt -y install libavresample-dev
sudo apt -y install x264 v4l-utils

# Optional dependencies
sudo apt -y install libprotobuf-dev protobuf-compiler
sudo apt -y install libgoogle-glog-dev libgflags-dev
sudo apt -y install libgphoto2-dev libeigen3-dev libhdf5-dev doxygen

# OpenCV
git clone https://github.com/opencv/opencv.git
cd ~/install/opencv || exit
git checkout $cvVersion
cd ..

git clone https://github.com/opencv/opencv_contrib.git
cd ~/install/opencv_contrib || exit
git checkout $cvVersion
cd ..

mkdir -p ~/install/opencv/build/
cd ~/install/opencv/build || exit

cmake -D CMAKE_BUILD_TYPE=RELEASE \
  -D CMAKE_INSTALL_PREFIX=~/install/opencv-4.3.0 \
  -D INSTALL_C_EXAMPLES=ON \
  -D INSTALL_PYTHON_EXAMPLES=ON \
  -D WITH_TBB=ON \
  -D WITH_V4L=ON \
  -D WITH_QT=ON \
  -D WITH_OPENGL=ON \
  -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules \
  -D BUILD_EXAMPLES=ON ..
  # -D OPENCV_PYTHON3_INSTALL_PATH=~/OpenCV-$cvVersion-py3/lib/python3.5/site-packages \

make install
sudo chmod +x -R ~/install/opencv-4.3.0
sudo mv ~/install/opencv-4.3.0 /opt/opencv-4.3.0
sudo mv ./conf/opencv.conf /etc/ld.so.conf.d/opencv.conf

echo "Done installing OpenCV!"

echo "Cleaning up folders..."
rm -rf ~/install
echo "Done."
