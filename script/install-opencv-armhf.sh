#!/bin/bash
apt update && apt upgrade -y
apt install -y apt-utils

# Enable armh for x86 machines
dpkg --add-architecture armhf
dpkg --print-foreign-architectures

# add-apt-repository universe
# sed -i 's/deb/deb [arch=i386,amd64]/g' /etc/apt/sources.list
# echo "deb [arch=armhf] http://ports.ubuntu.com/ trusty main universe" >> /etc/apt/sources.list
# echo "deb-src [arch=armhf] http://ports.ubuntu.com/ trusty main universe" >> /etc/apt/sources.list

# cat /etc/apt/sources.list

apt update && apt install -y qemu-user-static


# Python libs
apt install -y python3-dev
apt install -y python3-numpy
apt install -y python-dev
apt install -y python-numpy

apt install -y libpython2-dev:armhf
apt install -y libpython3-dev:armhf

# GUI libs, can be ignored for headless
apt install -y libgtk-3-dev:armhf libcanberra-gtk3-dev:armhf

# OpenCV libs
apt install -y libtiff-dev:armhf zlib1g-dev:armhf
apt install -y libjpeg-dev:armhf libpng-dev:armhf
apt install -y libavcodec-dev:armhf libavformat-dev:armhf libswscale-dev:armhf libv4l-dev:armhf
apt-get install -y libxvidcore-dev:armhf libx264-dev:armhf

# Cross compilers
apt install -y crossbuild-essential-armhf
apt install -y gfortran-arm-linux-gnueabihf

# Tools (Cmake, git, pkg-config, wget)
apt install -y cmake git pkg-config wget

apt-get install -f

# Download OpenCV
cd /

mkdir opencv_all && cd opencv_all || exit
wget -O opencv.tar.gz https://github.com/opencv/opencv/archive/4.3.0.tar.gz
tar xf opencv.tar.gz

wget -O opencv_contrib.tar.gz https://github.com/opencv/opencv_contrib/archive/4.3.0.tar.gz
tar xf opencv_contrib.tar.gz
rm *.tar.gz

# Env vars
export PKG_CONFIG_PATH=/usr/lib/arm-linux-gnueabihf/pkgconfig:/usr/share/pkgconfig
export PKG_CONFIG_LIBDIR=/usr/lib/arm-linux-gnueabihf/pkgconfig:/usr/share/pkgconfig

# Generate build scripts
cd /opencv_all/opencv-4.3.0 || exit
mkdir build && cd build || exit
cmake -D CMAKE_BUILD_TYPE=RELEASE \
        -D CMAKE_INSTALL_PREFIX=/opt/opencv-4.3.0 \
        -D CMAKE_TOOLCHAIN_FILE=../platforms/linux/arm-gnueabi.toolchain.cmake \
        -D OPENCV_EXTRA_MODULES_PATH=/opencv_all/opencv_contrib-4.3.0/modules \
        -D OPENCV_ENABLE_NONFREE=ON \
        -D ENABLE_NEON=ON \
        -D ENABLE_VFPV3=ON \
        -D BUILD_TESTS=OFF \
        -D BUILD_DOCS=OFF \
        -D PYTHON2_INCLUDE_PATH=/usr/include/python2.7 \
        -D PYTHON2_LIBRARIES=/usr/lib/arm-linux-gnueabihf/libpython2.7.so \
        -D PYTHON2_NUMPY_INCLUDE_DIRS=/usr/lib/python2/dist-packages/numpy/core/include \
        -D PYTHON3_INCLUDE_PATH=/usr/include/python3.7m \
        -D PYTHON3_LIBRARIES=/usr/lib/arm-linux-gnueabihf/libpython3.7m.so \
        -D PYTHON3_NUMPY_INCLUDE_DIRS=/usr/lib/python3/dist-packages/numpy/core/include \
        -D BUILD_OPENCV_PYTHON2=ON \
        -D BUILD_OPENCV_PYTHON3=ON \
        -D BUILD_EXAMPLES=OFF ..

# cat /opencv_all/opencv-4.3.0/build/CMakeFiles/CMakeError.log

# Build
make -j16

#Install
make install/strip

# Fix lib names for armhf
cd /opt/opencv-4.3.0/lib/python3.7/dist-packages/cv2/python-3.7/ || exit
cp cv2.cpython-37m-x86_64-linux-gnu.so cv2.so

# Compress
cd /opt || exit
tar -cjvf /opencv-4.3.0-armhf.tar.bz2 /export/opencv-4.3.0
cd /
exit