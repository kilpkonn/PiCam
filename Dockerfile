FROM ubuntu

RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y apt-utils

# Enable armh for x86 machines
RUN apt-get update && apt-get install -y qemu-user-static
RUN dpkg --add-architecture armhf
RUN dpkg --print-foreign-architectures
RUN apt-get update



# Python libs
RUN apt-get install -y python3-dev python3-numpy python-dev python-numpy
# RUN apt-get install -y libpython2-dev:armhf
RUN apt-get install -y libpython3-dev:armhf

# GUI libs, can be ignored for headless
RUN apt-get install -y libgtk-3-dev:armhf libcanberra-gtk3-dev:armhf

# OpenCV libs
RUN apt-get install -y libtiff-dev:armhf zlib1g-dev:armhf
RUN apt-get install -y libjpeg-dev:armhf libpng-dev:armhf
RUN apt-get install -y libavcodec-dev:armhf libavformat-dev:armhf libswscale-dev:armhf libv4l-dev:armhf
RUN apt-get install -y libxvidcore-dev:armhf libx264-dev:armhf

# Cross compilers
RUN apt-get install -y crossbuild-essential-armhf
RUN apt-get install -y gfortran-arm-linux-gnueabihf

# Tools (Cmake, git, pkg-config, wget)
RUN apt-get install -y cmake git pkg-config wget

# Download OpenCV
WORKDIR /

RUN mkdir opencv_all && cd opencv_all
RUN wget -O opencv.tar.gz https://github.com/opencv/opencv/archive/4.3.0.tar.gz
RUN tar xf opencv.tar.gz

RUN wget -O opencv_contrib.tar.gz https://github.com/opencv/opencv_contrib/archive/4.3.0.tar.gz
RUN tar xf opencv_contrib.tar.gz
RUN rm *.tar.gz

# Env vars
RUN export PKG_CONFIG_PATH=/usr/lib/arm-linux-gnueabihf/pkgconfig:/usr/share/pkgconfig
RUN export PKG_CONFIG_LIBDIR=/usr/lib/arm-linux-gnueabihf/pkgconfig:/usr/share/pkgconfig

# Generate build scripts
WORKDIR /opencv_all/opencv-4.3.0
RUN mkdir build && cd build
RUN cmake -D CMAKE_BUILD_TYPE=RELEASE \
        -D CMAKE_INSTALL_PREFIX=/opt/opencv-4.3.0 \
        -D CMAKE_TOOLCHAIN_FILE=../platforms/linux/arm-gnueabi.toolchain.cmake \
        -D OPENCV_EXTRA_MODULES_PATH=~/opencv_all/opencv_contrib-4.3.0/modules \
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

# Build
RUN make -j16

#Install
RUN sudo make install/strip

# Fic lib names for armh
WORKDIR /opt/opencv-4.3.0/lib/python3.7/dist-packages/cv2/python-3.7/
RUN cp cv2.cpython-37m-x86_64-linux-gnu.so cv2.so

# Compress
WORKDIR /opt
RUN tar -cjvf /opencv-4.3.0-armhf.tar.bz2 opencv-4.3.0
WORKDIR /
